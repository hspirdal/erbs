#include "beziersurface.h"

BezierSurface::BezierSurface(const GMlib::DMatrix<GMlib::Vector<float, 3> >& c, float u_start, float u, float u_end, float v_start, float v, float v_end)
{
  init();

  // Generate the control points
  GMlib::DMatrix<float> bu, bv;
  GMlib::EvaluatorStatic<float>::evaluateBhp(bu, c.getDim1()-1, (u - u_start) / (u_end - u_start), float(1)/(u_end-u_start));
  GMlib::EvaluatorStatic<float>::evaluateBSp(bv, c.getDim2()-1, (u - v_start) / (v_end - v_start), float(1)/(v_end-v_start));
  bu.invert();
  bv.invert();
  bv.transpose();

  c_ = bu * (c^bv);

  for(int i = 0; i < c.getDim1(); i++)
    for(int j = 0; j < c.getDim2(); j++)
      c_[i][j] -= c(0)(0);

  this->translateGlobal(c(0)(0));
}

BezierSurface::BezierSurface(const BezierSurface &copy)
  : PSurf<float,3>(copy)
{
  init();
  c_ = copy.c_;
}

void BezierSurface::init()
{
  this->_type_id = GMlib::GM_SO_TYPE_SURFACE_BEZIER;

  closed_u_ = false;
  closed_v_ = false;
  start_u_ = 1.0f;
  start_v_ = 1.0f;
}

bool BezierSurface::isClosedU() const
{
  return closed_u_;
}

bool BezierSurface::isClosedV() const
{
  return closed_v_;
}

float BezierSurface::getEndPU()
{
  return 1.0f;
}

float BezierSurface::getEndPV()
{
  return 1.0f;
}

float BezierSurface::getStartPU()
{
  return 0.0f;
}

float BezierSurface::getStartPV()
{
  return 0.0f;
}
