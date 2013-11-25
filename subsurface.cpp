#include "subsurface.h"
#include <QDebug>

SubSurface::SubSurface(GMlib::PSurf<float, 3>* surf, float u_start, float u, float u_end, float v_start, float v, float v_end)
{
  this->_dm = GMlib::GM_DERIVATION_EXPLICIT;
  set(surf, u_start, u, u_end, v_start, v, v_end);

  GMlib::DMatrix<GMlib::Vector<float, 3> > tr =  surf_->evaluateParent(q_, 0);
  trans_ = tr[0][0];
  this->DisplayObject::translate(trans_);
}

bool SubSurface::isClosedU() const
{
  return false;
}

bool SubSurface::isClosedV() const
{
  return false;
}

void SubSurface::eval(float u, float v, int d1, int d2, bool lu, bool lv)
{
  this->_p.setDim(3, 3);
  GMlib::DMatrix<GMlib::Vector<float, 3> > p = surf_->evaluateParent(u, v, d1, d2);
  this->_p[0][0] = p[0][0] - trans_;
  //qDebug() << _p[0][0][0] << _p[0][0][1] << _p[0][0][2];
}

float SubSurface::getEndPU()
{
  return u_end_;
}

float SubSurface::getEndPV()
{
  return v_end_;
}

float SubSurface::getStartPU()
{
  return u_start_;
}

float SubSurface::getStartPV()
{
  return v_start_;
}

void SubSurface::set(GMlib::PSurf<float, 3>* surf, float u_start, float u, float u_end, float v_start, float v, float v_end)
{
  surf_ = surf;
  u_start_ = u_start;
  u_ = u;
  u_end_ = u_end;
  v_start_ = v_start;
  v_ = v;
  v_end_ = v_end;


  q_ = GMlib::Point<float, 2>(u, v);
}
