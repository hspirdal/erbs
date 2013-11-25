#ifndef ERBSSURFACE_H
#define ERBSSURFACE_H

#include <gmParametricsModule>
#include "knotvector.h"

class ERBSSurface : public GMlib::PSurf<float, 3>
{
  GM_SCENEOBJECT(ERBSSurface)
public:
  ERBSSurface(GMlib::PSurf<float, 3>* c, int num_u, int num_v);
  virtual ~ERBSSurface();
  ERBSSurface(const ERBSSurface& copy); // nocopy

//  GMlib::DVector<float>& getKnotU();
//  GMlib::DVector<float>& getKnotV();

//  virtual void edit(SceneObject* obj);
  virtual bool isClosedU() const;
  virtual bool isClosedV() const;
//  virtual void preSample(int m1, int m2, int d1, int d2, float s_u, float s_v, float e_u, float e_v);
//  virtual void replot(int m1, int m2, int d1, int d2);

protected:
  bool closed_u_;
  bool closed_v_;
  GMlib::DVector<float> v_;
  GMlib::DVector<float> u_;
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
  void createKnotVectors(GMlib::DVector<float>& knotVector, int size, float delta_size);
  void padKnotVector(GMlib::DVector<float>& knotVector, bool isClosed);
  void insertPatch(GMlib::PSurf<float, 3>* patch);

  void getB(GMlib::DVector<float>& B, const GMlib::DVector<float>& kv, int tk, float t, int d);


};
#endif // ERBSSURFACE_H
