#ifndef EFFECT_H
#define EFFECT_H

#include <gmCoreModule>
#include <gmParametricsModule>

class Effect
{
public:
  Effect() {}
  Effect(GMlib::DMatrix<GMlib::PSurf<float, 3>* >& surfs) : c_(surfs) {}
  //virtual ~Effect();

  virtual void setSurfs(GMlib::DMatrix<GMlib::PSurf<float, 3>* >& surfs) { c_ = surfs; }

  virtual void init() {}
  virtual void update(double dt) = 0;
  virtual void end() {}


protected:
  GMlib::DMatrix<GMlib::PSurf<float, 3>* > c_;
};

#endif // EFFECT_H
