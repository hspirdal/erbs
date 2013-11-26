#include "scenario.h"

// local
#include "testtorus.h"
// GMlib
#include <gmParametricsModule>
#include "erbssurface.h"
#include "subsurface.h"


Scenario::Scenario() {}

Scenario::~Scenario() {}

void Scenario::setupScenario() {

  using namespace GMlib;

  PSurfNormalsVisualizer<float,3> *normals_visu = new PSurfNormalsVisualizer<float,3>;

  TestTorus *torus = new TestTorus();
//  torus->setMaterial(GMmaterial::PolishedCopper);
//  torus->toggleDefaultVisualizer();
////  //torus->insertVisualizer(normals_visu);
//  torus->replot(50, 50, 2, 2);
//  insert(torus);

    Point<float, 3> p = Point<float, 3>(0, 0, 0);
    Vector<float, 3> u = Vector<float, 3>(10, 0, 0);
    Vector<float, 3> v = Vector<float, 3>(0, 0, 10);


    PPlane<float>* plane = new PPlane<float>(p, u, v);
    plane->toggleDefaultVisualizer();
    plane->replot(2, 2, 2, 2);
//    insert(plane);

    PCylinder<float>* cyl = new PCylinder<float>(3, 3, 25);
//    cyl->enableDefaultVisualizer();
//    cyl->replot(20, 20, 1, 1);
//    insert(cyl);

//    SubSurface* subsurf = new SubSurface(cyl, 0, 5, 10, 0, 5, 10);
//    subsurf->enableDefaultVisualizer();
//    subsurf->replot(2, 2, 1, 1);
//    insert(subsurf);

    ERBSSurface* erbsurf = new ERBSSurface(torus, 4, 4);
    //ERBSSurface* erbsurf = new ERBSSurface(plane, 4, 4);
    erbsurf->toggleDefaultVisualizer();
    //  erbsurf->insertVisualizer(normals_visu);
    erbsurf->setMaterial(GMmaterial::Emerald);
    erbsurf->replot(20, 20, 1, 1);
    insert(erbsurf);


//    GMlib::PERBSSurf<float>* realerb = new GMlib::PERBSSurf<float>(torus, 20, 20, 1, 1);
//    realerb->toggleDefaultVisualizer();

//    realerb->setMaterial(GMmaterial::PolishedCopper);
//    realerb->replot(20, 20, 1, 1);
//    realerb->showLocalPatches();
//    insert(realerb);

}

void Scenario::cleanupScenario()
{

}
