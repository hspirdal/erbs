#ifndef BEZIERSURFACE_H
#define BEZIERSURFACE_H

#include <gmParametricsModule>

class BezierSurface : public GMlib::PSurf<float, 3>
{
  GM_SCENEOBJECT(BezierSurface)
public:
  BezierSurface(const GMlib::DMatrix<GMlib::Vector<float, 3> >& c, float u_start, float u, float u_end, float v_start, float v, float v_end);
  BezierSurface(const BezierSurface& copy);
  virtual ~BezierSurface() {}

  virtual bool  isClosedU() const;
  virtual bool  isClosedV() const;

protected:

  virtual void  eval(float u, float v, int d1, int d2, bool lu, bool lv);
  virtual float getEndPU();
  virtual float getEndPV();
  virtual float getStartPU();
  virtual float getStartPV();

  //void set(GMlib::PSurf<float, 3>* surf, float u_start, float u, float u_end, float v_start, float v, float v_end);
  void init();


  GMlib::DMatrix<GMlib::Vector<float, 3> > c_;
  GMlib::Vector<float, 3> trans_;
  GMlib::Vector<float, 2> q_;
  bool closed_u_;
  bool closed_v_;
  float start_u_;
  float start_v_;
};

#endif // BEZIERSURFACE_H
