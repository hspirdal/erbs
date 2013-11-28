#ifndef SUBSURFACE_H
#define SUBSURFACE_H

#include <gmParametricsModule>


class SubSurface : public GMlib::PSurf<float, 3>
{
  GM_SCENEOBJECT(SubSurface)
public:
  SubSurface(GMlib::PSurf<float, 3>* surf, float u_start, float u, float u_end, float v_start, float v, float v_end);
  virtual ~SubSurface() {}


  virtual bool  isClosedU() const;
  virtual bool  isClosedV() const;

protected:
  SubSurface(const SubSurface& copy);
  virtual void  eval(float u, float v, int d1, int d2, bool lu, bool lv);
  virtual float getEndPU();
  virtual float getEndPV();
  virtual float getStartPU();
  virtual float getStartPV();

  void set(GMlib::PSurf<float, 3>* surf, float u_start, float u, float u_end, float v_start, float v, float v_end);

  GMlib::PSurf<float, 3>* surf_;
  float u_start_;
  float u_;
  float u_end_;
  float v_start_;
  float v_;
  float v_end_;
  GMlib::Vector<float, 3> trans_;
  GMlib::Vector<float, 2> q_;
  GMlib::Angle origAngle_;









};

#endif // SUBSURFACE_H
