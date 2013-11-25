#ifndef SCENARIO_H
#define SCENARIO_H

#include <scenarioplugininterface.h>

#include <QtCore>

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
};

#endif // SCENARIO_H
