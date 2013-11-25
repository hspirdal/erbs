#ifndef TESTTORUS_H
#define TESTTORUS_H


#include <parametrics/gmPTorus>

class TestTorus : public GMlib::PTorus<float>  {
  GM_SCENEOBJECT(TestTorus)
public:
  TestTorus() {}

protected:
  void localSimulate(double dt) {

    using namespace GMlib;
    rotate( Angle(90) * dt, Vector<float,3>(0.0f,0.0f,1.0f) );
    rotate( Angle(45) * dt, Vector<float,3>(0.0f,1.0f,0.0f) );
  }
};

#endif // TESTTORUS_H
