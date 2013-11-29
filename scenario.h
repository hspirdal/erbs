#ifndef SCENARIO_H
#define SCENARIO_H

#include <scenarioplugininterface.h>
#include <memory>
#include <QtCore>
#include <gmParametricsModule>
// local
#include "testtorus.h"
#include "erbssurface.h"
#include "subsurface.h"
#include "effects/RotationThingy.h"
#include "effects/PlaneEqualizerEffect.h"
#include "equalizervisualizer.h"

class Scenario : public ScenarioPluginInterface {

  Q_INTERFACES(ScenarioPluginInterface)
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "com.gmlib.GMStudio.scenario.testscenario01" FILE "scenario.json")

public:
  Scenario();
  ~Scenario();

protected:

  /* virtual from ScenarioPluginInterface */
  void setupScenario();
  void cleanupScenario();

private:
  std::shared_ptr<ERBSSurface> erbssurf_;
  std::shared_ptr<RotationThingy> eff_rotationThingy_;
  std::shared_ptr<PlaneEqualizerEffect> eff_equalizerEffect_;
  std::shared_ptr<EqualizerVisualizer> eq_visulizer_;
  std::shared_ptr<PSurfNormalsVisualizer<float, 3> > norm_visualizer_;

  std::shared_ptr<PPlane<float>> plane_;
  std::shared_ptr<GMlib::PCylinder<float> > cyl_;
  std::shared_ptr<PSphere<float>> sphere_;
  std::shared_ptr<TestTorus> torus_;
};

#endif // SCENARIO_H
