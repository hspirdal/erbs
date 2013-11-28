#ifndef ERBSSURFACE_H
#define ERBSSURFACE_H

#include <gmParametricsModule>
#include "knotvector.h"
#include <vector>
#include "effects/effect.h"

typedef GMlib::DMatrix<GMlib::Vector<float, 3> > DMat3F;
typedef GMlib::Point<float, 2> Point2F;

class ERBSSurface : public GMlib::PSurf<float, 3>
{
  GM_SCENEOBJECT(ERBSSurface)
public:
  ERBSSurface(GMlib::PSurf<float, 3>* c, int num_u, int num_v);
  virtual ~ERBSSurface();

//  GMlib::DVector<float>& getKnotU();
//  GMlib::DVector<float>& getKnotV();

//  virtual void edit(SceneObject* obj);
  virtual bool isClosedU() const;
  virtual bool isClosedV() const;
  virtual void localSimulate(double dt);
//  virtual void preSample(int m1, int m2, int d1, int d2, float s_u, float s_v, float e_u, float e_v);
//  virtual void replot(int m1, int m2, int d1, int d2);

  void addEffect(Effect* effect);

protected:
  bool closed_u_;
  bool closed_v_;
//  GMlib::DVector<float> v_;
//  GMlib::DVector<float> u_;
  KnotVector u_;
  KnotVector v_;
  GMlib::DMatrix<GMlib::PSurf<float, 3>* > c_;
  GMlib::ERBSEvaluator<long double>* evaluator_;
  GMlib::PSurf<float, 3>* surf_;

  virtual void  eval(float u, float v, int d1, int d2, bool lu, bool lv);
  virtual float getEndPU();
  virtual float getEndPV();
  virtual float getStartPU();
  virtual float getStartPV();

  void init();
  GMlib::DMatrix< GMlib::Vector<float, 3> > getC(float u, float v, int uk, int vk, float du, float dv);

private:
  ERBSSurface(const ERBSSurface& copy); // nocopy
  void insertPatch(GMlib::PSurf<float, 3>* patch);
  void getB(GMlib::DVector<float>& B, const GMlib::DVector<float>& kv, int tk, float t, int d);

  /* helpers */
  int nextKnotIntervalIndex(KnotVector& kv, float val);
  void computePascalTriangleNumbers(GMlib::DVector<float>& B, GMlib::DVector<float>& a, DMat3F& s0, DMat3F& s1);

  std::vector<Effect*> effects_;


};
#endif // ERBSSURFACE_H
