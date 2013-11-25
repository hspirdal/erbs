#include "erbssurface.h"
#include "subsurface.h"
#include "Constants.h"
#include <QDebug>
#include <complex>

ERBSSurface::ERBSSurface(GMlib::PSurf<float, 3>* c, int num_u, int num_v)
{
  this->_type_id = GMlib::GM_SO_TYPE_SURFACE_ERBS;

  init();

  //surf_ = c;

  closed_u_ = c->isClosedU();
  closed_v_ = c->isClosedV();

  num_u += closed_u_ ? 1 : 0;
  num_v += closed_v_ ? 1 : 0;
  c_.setDim(num_u, num_v);

  createKnotVectors(u_, num_u, c->getParDeltaU());
  createKnotVectors(v_, num_v, c->getParDeltaV());

  // Generate the padding on both ends of the knotvectors.
  padKnotVector(u_, closed_u_);
  padKnotVector(v_, closed_v_ );

  // Split the surface up in smaller patches
  int numSubsurfs = 0;
  int uk =  closed_u_?  1 : 0;
  int vk = closed_v_ ? 1 : 0;

  for(int i = 0; i < num_u-uk; i++)
  {
    for(int j = 0; j < num_v-vk; j++)
    {
        c_[i][j] = new SubSurface(c, u_[i], u_[i+1], u_[i+2], v_[j], v_[j+1], v_[j+2]);
        insertPatch(c_[i][j]);
        numSubsurfs++;

        qDebug() << "i,j" << i << "," << j;
    }
  }

  if(closed_u_)
  {
    for(int i = 0; i < num_v-1; i++)
      c_[num_u-1][i] = c_[0][i];
  }

  if(closed_v_)
  {
    for(int i = 0; i < num_u; i++)
    {
      c_[i][num_v-1] = c_[i][0];
    }

  }
  else if(closed_u_)
  {
    c_[num_u-1][num_v-1] = c_[0][num_v-1];
  }

  qDebug() << "Num Subsurfs:" << numSubsurfs;
}

ERBSSurface::ERBSSurface(const ERBSSurface &copy)
{

}

ERBSSurface::~ERBSSurface()
{
}

void ERBSSurface::init()
{
  _no_sam_u = 20;
  _no_sam_v = 20;
  _no_der_u = 1;
  _no_der_v = 1;

  // evaluator.. etc
  evaluator_ = GMlib::ERBSEvaluator<long double>::getInstance();

}

void ERBSSurface::createKnotVectors(GMlib::DVector<float>& knotVector, int size, float delta_size)
{
  knotVector.setDim(size+2);

  for(int i = 1; i <= size; i++)
  {
    knotVector[i] = (i - 1) * delta_size / (size-1);
  }
//  qDebug() << "knotcreate:";
//  for(int i = 0; i < knotVector.getDim(); i++)
//    qDebug() << knotVector[i];
}

void ERBSSurface::padKnotVector(GMlib::DVector<float> &knotVector, bool isClosed)
{
  if(isClosed)
  {
    knotVector[0] = knotVector[1] - (knotVector[knotVector.getDim()-2] - knotVector[knotVector.getDim()-3]);
    knotVector[knotVector.getDim()-1] = knotVector[knotVector.getDim()-2] + knotVector[2] - knotVector[1];

  }
  else
  {
    knotVector[0] = knotVector[1];
    knotVector[knotVector.getDim()-1] = knotVector[knotVector.getDim()-2];
  }
  qDebug() << "knotpad:";
  for(int i = 0; i < knotVector.getDim(); i++)
    qDebug() << knotVector[i];
}

void ERBSSurface::insertPatch(GMlib::PSurf<float, 3> *patch)
{
  patch->enableDefaultVisualizer();
  patch->replot(10, 10, 1, 1);

  insert(patch);
}

void  ERBSSurface::eval(float u, float v, int d1, int d2, bool lu, bool lv)
{
  _p.setDim(1,1);
  _p[0][0] = GMlib::Vector<float,3>(0,0,0);

  // Find knot indices u_k and v_k
  int uk, vk;
  qDebug() << "u:v" << u << "," << v;
  for(uk = 1; uk < u_.getDim()-2; ++uk)
  {
    if(u < u_[uk+1])
    {
      qDebug() << "u break";
      break;
    }
  }

  if(uk == u_.getDim()-2)
    uk--;


  for(vk = 1; vk < v_.getDim()-2; ++vk)
  {
    if(v < v_[vk+1])
    {
      qDebug() << "v break";
      break;
    }
  }

  if(vk == v_.getDim()-2)
    vk--;

  qDebug() << "uk:vk" << uk << "," << vk;

  // Evaluation from right in the matrix, find first knot
  if(!lu) while(std::abs(u_[uk] - u_[uk-1]) < Constants::AlmostZero) --uk;
  if(!lv) while(std::abs(v_[vk] - v_[vk-1]) < Constants::AlmostZero) --vk;

  qDebug() << "uk:vk" << uk << "," << vk;

  // Get result of inner loop for first patch in v
  GMlib::DMatrix<GMlib::Vector<float, 3> > s0 = getC(u, v, uk, vk, d1, d2);

  // If placed on a knot, return only first patch result
  if(std::abs(v - v_[vk]) < 1e-5)
  {
    this->_p = s0;
    return;
  }
  else
  {
    // Get result of inner loop for second patch in v
    GMlib::DMatrix<GMlib::Vector<float, 3> > s1 = getC(u, v, uk, vk+1, d1, d2);

    // Evaluate ERBS-basis in v direction
    GMlib::DVector<float> B;
    getB(B, v_, vk, v, d2);

    // Compute "Pascals triangle" numbers and correct patch matrix
    GMlib::DVector<float> a(B.getDim());
    s0 -= s1;
    s0.transpose(); s1.transpose();
    for(int i = 0; i < B.getDim(); i++)
    {
      a[i] = 1;
      for(int j = i-1; j > 0; j--)
        a[j] += a[j-1];   // compute "pascal's triangle" numbers

      for(int j = 0; j <= i; j++)
        s1[i] += (a[j]*B[j]) * s0[i-j]; // "column += scalar x column"
    }
    s1.transpose();

    this->_p = s1;
  }

}

GMlib::DMatrix< GMlib::Vector<float, 3> > ERBSSurface::getC(float u, float v, int uk, int vk, float du, float dv)
{

  // init indexes and get local u/v values
  const int cu = uk-1;
  const int cv = vk-1;

  assert(cu >= 0);
  assert(cv >= 0);

  qDebug() << "cu, cv:" << cu << "," << cv;
  //c_[cu][cv]

  const GMlib::Point<float,2>& lm = c_[cu][cv]->getLocalMapping( GMlib::Point<float,2>(u,v),
                                   GMlib::Point<float,2>( u_[uk-1], v_[vk-1]),
                                   GMlib::Point<float,2>( u_[uk+1], v_[vk+1]));

  assert(cu < c_.getDim1() && cv < c_.getDim2() && "out of bounds on c_");

  // Evaluate first local patch.
  GMlib::DMatrix<GMlib::Vector<float,3> > c0 = c_[cu][cv]->evaluateParent(lm(0), lm(1), du, dv);

  // If on a interpolation point, return only first patch evaluation.
  if(std::abs(u - uk) < Constants::AlmostZero)
    return c0;

  // Select next local patch in u direction.
  uk++;
  qDebug() << "incr uk:" << uk;

  // Init indexes and get local u/v values
  const int cu2 = uk-1;
  const int cv2 = vk-1;
  const GMlib::Point<float, 2>& lm2 = c_[cu2][cv2]->getLocalMapping(GMlib::Point<float, 2>(u, v),
                                                             GMlib::Point<float, 2>(u_[uk-1], v_[vk-1]),
                                                             GMlib::Point<float, 2>(u_[uk+1], v_[vk+1]));

  // Evaluate second local patch
  GMlib::DMatrix<GMlib::Vector<float, 3> > c1 = c_[cu2][cv2]->evaluateParent(lm2(0), lm2(1), du, dv);
  GMlib::DVector<float> a(du+1);

  // Evaluate ERBS-basis in u direction
  GMlib::DVector<float> B;
  getB(B, u_, uk-1, u, du);

  // Compute "Pascals triangle" -numbers and correct patch matrix
  c0 -= c1;
  for(int i = 0; i < B.getDim(); i++)
  {
    a[i] = 1;
    for(int j = i-1; j > 0; j--)
      a[j] += a[j-1];
    for(int j = 0; j <= i; j++)
      c1[i] += (a[j] * B[j]) * c0[i-j];
  }
  return c1;
}

void ERBSSurface::getB(GMlib::DVector<float> &B, const GMlib::DVector<float> &kv, int tk, float t, int d)
{
  B.setDim(d+1);

  evaluator_->set(kv(tk), kv(tk+1) - kv(tk));
  B[0] = 1 - evaluator_->operator ()(t);
  B[1] = - evaluator_->getDer1();
  B[2] = - evaluator_->getDer2();
}

float ERBSSurface::getEndPU()
{
  return u_[u_.getDim()-2];
}

float ERBSSurface::getEndPV()
{
  return v_[v_.getDim()-2];
}

float ERBSSurface::getStartPU()
{
  return u_[1];
}

float ERBSSurface::getStartPV()
{
  return v_[1];
}

bool ERBSSurface::isClosedU() const
{
  return closed_u_;
}

bool ERBSSurface::isClosedV() const
{
  return closed_v_;
}






