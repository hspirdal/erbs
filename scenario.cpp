#include "scenario.h"




Scenario::Scenario() {}

Scenario::~Scenario() {}

void Scenario::setupScenario() {

  using namespace GMlib;

  PSurfNormalsVisualizer<float,3> *normals_visu = new PSurfNormalsVisualizer<float,3>;
  eq_visulizer_ = std::make_shared<EqualizerVisualizer>();

  torus_ = std::make_shared<TestTorus>();
  torus_->insertVisualizer(normals_visu);

  const float Size = 10;

  Point<float, 3> p = Point<float, 3>(0, 0, 0);
  Vector<float, 3> u = Vector<float, 3>(0, 0, Size);
  Vector<float, 3> v = Vector<float, 3>(Size, 0, 0);


  plane_ = std::make_shared<PPlane<float>>(p, u, v);
  sphere_ = std::make_shared<PSphere<float>>(10);
  cyl_ = std::make_shared<PCylinder<float>>(3, 3, 25);


  eff_rotationThingy_ = std::make_shared<RotationThingy>();
  eff_equalizerEffect_ = std::make_shared<PlaneEqualizerEffect>();


  erbssurf_ = std::make_shared<ERBSSurface>(plane_.get() , 64, 64);
  //erbssurf_->toggleDefaultVisualizer();
  erbssurf_->insertVisualizer(eq_visulizer_.get());
  //erbssurf_->insertVisualizer(normals_visu);
  erbssurf_->setMaterial(GMmaterial::Ruby);
  erbssurf_->replot(20, 20, 1, 1);
  insert(erbssurf_.get());

  erbssurf_->setLocalPatchesCollapsed(false);
  erbssurf_->addEffect(eff_equalizerEffect_.get());
}

void Scenario::cleanupScenario()
{

}
