// scenarios.h
#pragma once
#include "arrays.h"
#include "linkedlist.h"
#include "queue.h"
#include "tree.h"
#include "graph.h"
#include "hash.h"
#include "monitor.h"

class ScenarioRunner {
public:
	void runCascadingFire();       // Scenario 1 - fire in Zone 3 spreads to 4 and 6
	void runSensorFailure();       // Scenario 2 - Zone 2 sensor reconstruction
	void runMultiFactorAnomaly();  // Scenario 3 - combined wildlife, fire, human anomaly
	void runSystemOverload();      // Scenario 4 - high load balancing and recovery
	void runGlobalEmergency();     // Scenario 5 - multi-zone emergency synchronization
	void runFullSimulation();      // Run all 5 scenarios in sequence
};