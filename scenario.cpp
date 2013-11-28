#include "scenario.h"

// local
#include "testtorus.h"
// GMlib
#include <gmParametricsModule>
#include "erbssurface.h"
#include "subsurface.h"

#include "effects/RotationThingy.h"
#include "effects/PlaneEqualizerEffect.h"

Scenario::Scenario() {}

Scenario::~Scenario() {}

void Scenario::setupScenario() {

  using namespace GMlib;

  PSurfNormalsVisualizer<float,3> *normals_visu = new PSurfNormalsVisualizer<float,3>;

  TestTorus *torus = new TestTorus();
////  //torus->insertVisualizer(normals_visu);

    const float Size = 10;

    Point<float, 3> p = Point<float, 3>(0, 0, 0);
    Vector<float, 3> u = Vector<float, 3>(Size, 0, 0);
    Vector<float, 3> v = Vector<float, 3>(0, 0, Size);


    PPlane<float>* plane = new PPlane<float>(p, u, v);
//    PSphere<float>* sphere = new PSphere<float>(10);
//    PCylinder<float>* cyl = new PCylinder<float>(3, 3, 25);


    //RotationThingy* effRotThingy = new RotationThingy();
    PlaneEqualizerEffect* effEqual = new PlaneEqualizerEffect();


    ERBSSurface* erbsurf = new ERBSSurface(plane, 8, 8);
    //ERBSSurface* erbsurf = new ERBSSurface(plane, 4, 4);
    erbsurf->toggleDefaultVisualizer();
    //  erbsurf->insertVisualizer(normals_visu);
    erbsurf->setMaterial(GMmaterial::Ruby);
    erbsurf->replot(20, 20, 1, 1);
    insert(erbsurf);

    erbsurf->addEffect(effEqual);


//    GMlib::PERBSSurf<float>* realerb = new GMlib::PERBSSurf<float>(sphere, 20, 20, 1, 1);
//    realerb->toggleDefaultVisualizer();

//    realerb->setMaterial(GMmaterial::PolishedCopper);
//    realerb->replot(20, 20, 1, 1);
//    realerb->showLocalPatches();
//    insert(realerb);

}

void Scenario::cleanupScenario()
{

}
