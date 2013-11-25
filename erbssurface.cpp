#include "erbssurface.h"
#include "subsurface.h"
#include "Constants.h"
#include <QDebug>
#include <complex>
#include "util.h"


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

  // Generate and pad the knotvectors.
  u_.create(num_u, c->getParDeltaU());
  v_.create(num_v, c->getParDeltaV());
  u_.pad(closed_u_); v_.pad(closed_v_);

  // Split the surface up in smaller patchesbuild
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

        //qDebug() << "i,j" << i << "," << j;
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

  //qDebug() << "Num Subsurfs:" << numSubsurfs;
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

void ERBSSurface::insertPatch(GMlib::PSurf<float, 3> *patch)
{
  patch->enableDefaultVisualizer();
  patch->setCollapsed(true);
  patch->replot(10, 10, 1, 1);

  insert(patch);
}

void  ERBSSurface::eval(float u, float v, int d1, int d2, bool lu, bool lv)
{
//  _p.setDim(1,1);
//  _p[0][0] = GMlib::Vector<float,3>(0,0,0);

  // Find the index for current knot-interval for u_k and v_k.
  int uk = nextKnotIntervalIndex(u_, u);
  int vk = nextKnotIntervalIndex(v_, v);
//  std::cout <<  "uk: " << uk << ". ";  Util::writeDebugValues(u_.data(), "u_vec:");
//  std::cout <<  "vk: " << vk << ". ";  Util::writeDebugValues(v_.data(), "v_vec:");

  // Evaluation from right in the matrix, find first knot
  if(!lu) while(std::abs(u_[uk] - u_[uk-1]) < Constants::AlmostZero) --uk;
  if(!lv) while(std::abs(v_[vk] - v_[vk-1]) < Constants::AlmostZero) --vk;

  GMlib::DMatrix<GMlib::Vector<float, 3> > s0 = getC(u, v, uk, vk, d1, d2);


  // return only first patch result if we're on a knot.
  if(std::abs(v - v_[vk]) < Constants::AlmostZero)
  {
    _p = s0;
    return;
  }
  // Blend the patches
  GMlib::DMatrix<GMlib::Vector<float, 3> > s1 = getC(u, v, uk, vk+1, d1, d2);
  GMlib::DVector<float> B;
  getB(B, v_.data(), vk, v, d2);
  GMlib::DVector<float> a(B.getDim());
  s0 -= s1;

  // compute for columns. (column += scalar x column)
  s0.transpose(); s1.transpose();
  computePascalTriangleNumbers(B, a, s0, s1);
  s1.transpose();
  _p = s1;
}

void ERBSSurface::localSimulate(double dt)
{
  replot();
}

void ERBSSurface::computePascalTriangleNumbers(GMlib::DVector<float>& B, GMlib::DVector<float>& a, DMat3F& s0, DMat3F& s1)
{
  for(int i = 0; i < B.getDim(); i++)
  {
    a[i] = 1;
    for(int j = i-1; j > 0; j--)
      a[j] += a[j-1]; // compute Pascal's Triangle numbers.
    for(int j = 0; j <= i; j++)
      s1[i] += (a[j]*B[j]) * s0[i-j];
  }
}

int ERBSSurface::nextKnotIntervalIndex(KnotVector& kv, float val)
{
  int k=0;
  for(k=1; k < kv.getDim()-2; ++k)
    if(val < kv[k+1])
      return k;

  // Handling edges.
  return k-1;
}

GMlib::DMatrix< GMlib::Vector<float, 3> > ERBSSurface::getC(float u, float v, int uk, int vk, float du, float dv)
{
  // init indexes and get local u/v values
  const int cu = uk-1;
  const int cv = vk-1;
  assert(cu >= 0 && cv >= 0);

  const Point2F& lm = c_[cu][cv]->getLocalMapping(Point2F(u, v), Point2F(u_[uk-1], v_[vk-1]), Point2F(u_[uk+1], v_[vk+1]));

  // Result evaluating local patch - (k_u, k_v)
  DMat3F c0 = c_[cu][cv]->evaluateParent(lm(0), lm(1), du, dv);

  // If on an interpolation point, return only first patch evaluation
  if(std::abs(u - u_[uk]) < Constants::AlmostZero)
    return c0;

  // Select next local patch in U direction.
  uk++;

  // init indexes and get local u/v values
  const int cu2 = uk-1;
  const int cv2 = vk-1;

  // Result evaluating 2nd local patch
  const Point2F& lm2 = c_[cu2][cv2]->getLocalMapping(Point2F(u, v),Point2F(u_[uk-1], v_[vk-1]), Point2F(u_[uk+1], v_[vk+1]));

  DMat3F c1 = c_[cu2][cv2]->evaluateParent(lm2(0), lm2(1), du, dv);

  // For numbers - "Pascals triangle".
  GMlib::DVector<float> a(du+1);

  // Result evaluating ERBS-basis (alg10).
  GMlib::DVector<float> B;
  getB(B, u_.data(), uk-1, u, du);

  // The matrix c0 is now c^0 expanded (5.7).
  c0 -= c1;

  computePascalTriangleNumbers(B, a, c0, c1);
  return c1;
}

void ERBSSurface::getB(GMlib::DVector<float> &B, const GMlib::DVector<float>& kv, int tk, float t, int d)
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






