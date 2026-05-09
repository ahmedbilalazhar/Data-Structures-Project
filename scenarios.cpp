// scenarios.cpp
#include "scenarios.h"

// Scenario 1: fire starts in Zone 3, spreads to Zone 4 and Zone 6
// Uses: arrays, linked list, queue, graph, tree, hash
void ScenarioRunner::runCascadingFire() {
	cout << "SCENARIO 1: Cascading Fire and Resource Conflict Resolution" << endl;

	Baseline base;
	base.set(3, 25.0, 60.0, 0.0);
	base.set(4, 25.0, 60.0, 0.0);
	base.set(6, 25.0, 60.0, 0.0);
	cout << "Baseline set for zones 3, 4, 6." << endl;

	SensorStream stream;
	stream.add(75.0, 85.0, 15.0);
	stream.add(65.0, 78.0, 18.0);
	stream.add(55.0, 60.0, 25.0);
	stream.checkAnomalies(25.0, 0.0, 60.0);

	SinglyList rawEvents;
	rawEvents.insert(75.0, 1, 3);
	rawEvents.insert(65.0, 2, 4);
	rawEvents.insert(55.0, 3, 6);
	cout << "Raw event stream:" << endl;
	rawEvents.display();

	bool noisy = rawEvents.removeNoise(30.0);
	if (noisy) {
		cout << "Noise removed from stream." << endl;
	}
	rawEvents.filterAnomalies(25.0, 20.0);

	DoublyList corrections;
	corrections.insertBack(75.0, 1, 3);
	corrections.insertBack(65.0, 2, 4);
	corrections.insertBack(55.0, 3, 6);
	corrections.correctForward(2, 60.0);
	corrections.displayForward();

	EmergencyQueue eq;
	eq.enqueue(1, 3, 75.0, 1);
	eq.enqueue(2, 4, 65.0, 2);
	eq.enqueue(3, 6, 55.0, 3);
	eq.display();
	eq.dequeue();

	AdjListGraph graph(10);
	graph.addEdge(3, 4, 2.0);
	graph.addEdge(3, 6, 3.0);
	graph.addEdge(4, 6, 1.5);
	graph.setFireLevel(3, 0.9);
	graph.setFireLevel(4, 0.6);
	graph.setFireLevel(6, 0.4);
	graph.bfsFireSpread(3);

	IncidentTree itree;
	itree.buildDefault();
	itree.computeFireLevel(0.4, 75.0, 0.3, 85.0);

	DecisionTree dtree;
	dtree.buildDefault();
	float score = dtree.computeScore(0.9, 0.85, 0.75);
	dtree.localDecision(score);
	dtree.regionalDecision(0.7);

	ResourceTree rtree;
	rtree.buildDefault();
	rtree.checkWaterAvailability(60.0, 100.0);
	rtree.computePriority(0.9, 0.8);

	PrimaryIndexTable table(20);
	table.insert(3, 75.0, 85.0, 15.0);
	table.insert(4, 65.0, 78.0, 18.0);
	table.insert(6, 55.0, 60.0, 25.0);
	table.show();

	cout << "Scenario 1 complete." << endl;
}

// Scenario 2: Zone 2 sensors fail, system reconstructs using past data
// Uses: arrays, doubly list, singly list, hash
void ScenarioRunner::runSensorFailure() {
	cout << "SCENARIO 2: Sensor Failure and System Reconstruction" << endl;

	SensorStream stream;
	stream.add(25.0, 5.0, 60.0);
	stream.add(-1.0, 5.0, 60.0);
	stream.add(26.0, 5.0, 60.0);
	stream.add(200.0, 5.0, 60.0);
	stream.add(27.0, 6.0, 58.0);
	cout << "Raw sensor stream before validation:" << endl;
	stream.show();
	stream.checkAnomalies(25.0, 0.0, 60.0);

	ForestGrid grid;
	grid.setCell(1, 1, 25.0, 5.0);
	grid.setCell(1, 3, 27.0, 5.0);
	grid.setCell(0, 2, 26.0, 5.0);
	grid.setCell(2, 2, 28.0, 5.0);
	cout << "Zone 2 sensor failed. Interpolating..." << endl;
	grid.interpolate(1, 2);
	grid.show();

	DoublyList history;
	history.insertBack(25.0, 1, 2);
	history.insertBack(26.0, 2, 2);
	history.insertBack(27.0, 3, 2);
	cout << "Last valid history (backward):" << endl;
	history.displayBackward();
	history.correctBackward(2, 25.5);
	history.synchronize(26.0);
	history.displayForward();

	CollisionTable colTable(10);
	colTable.insert(2, 26.0, 5.0, 59.0);
	colTable.insert(12, 26.5, 5.5, 58.0);
	colTable.show();

	cout << "Zone 2 restored. Resuming normal operation." << endl;
	cout << "Scenario 2 complete." << endl;
}

// Scenario 3: wildlife, fire, and human anomalies detected simultaneously
// Uses: singly list, decision queue, incident tree, decision tree, graph
void ScenarioRunner::runMultiFactorAnomaly() {
	cout << "SCENARIO 3: Multi-Factor Anomaly Escalation System" << endl;

	SinglyList wildlifeEvents;
	wildlifeEvents.insert(0.8, 1, 5);
	wildlifeEvents.insert(0.9, 2, 5);
	cout << "Wildlife events:" << endl;
	wildlifeEvents.display();

	SinglyList fireEvents;
	fireEvents.insert(70.0, 1, 7);
	fireEvents.insert(80.0, 2, 7);
	cout << "Fire events:" << endl;
	fireEvents.display();

	SinglyList humanEvents;
	humanEvents.insert(0.7, 1, 9);
	cout << "Human events:" << endl;
	humanEvents.display();

	DecisionQueue dq;
	dq.enqueue(1, 5, 0.85, 2);
	dq.enqueue(2, 7, 75.0, 1);
	dq.enqueue(3, 9, 0.7, 2);
	dq.display();

	IncidentTree itree;
	itree.buildDefault();
	itree.computeFireLevel(0.4, 80.0, 0.3, 75.0);
	itree.computeHumanRisk(0.7, 0.9);
	itree.classify(0.85, 0.6);

	DecisionTree dtree;
	dtree.buildDefault();
	float score = dtree.computeScore(0.85, 0.75, 0.8);
	dtree.localDecision(score);
	dtree.regionalDecision(0.6);
	dtree.globalDecision(score, 0.6);

	AdjMatrixGraph mg(10);
	mg.addEdge(5, 7, 2.0);
	mg.addEdge(7, 9, 1.5);
	mg.setFireLevel(7, 0.75);
	mg.dfsDeepAnalysis(5);

	cout << "Scenario 3 complete." << endl;
}

// Scenario 4: system gets overloaded, tasks paused, cache used, load balanced
// Uses: all queues, hash cache, monitor, doubly list
void ScenarioRunner::runSystemOverload() {
	cout << "SCENARIO 4: System Overload and Load Redistribution" << endl;

	RoutineQueue rq;
	SurveillanceQueue sq;
	EmergencyQueue eq;

	int i = 1;
	while (i <= 5) {
		rq.enqueue(i, i, 20.0 + i);
		sq.enqueue(i + 10, i, 30.0 + i);
		i = i + 1;
	}
	eq.enqueue(100, 3, 80.0, 1);
	eq.enqueue(101, 4, 75.0, 2);

	cout << "Routine queue:" << endl;
	rq.display();
	cout << "Surveillance queue:" << endl;
	sq.display();
	cout << "Emergency queue (processes first):" << endl;
	eq.display();
	eq.dequeue();
	eq.dequeue();

	DecisionQueue dq;
	dq.enqueue(200, 1, 25.0, 3);
	dq.enqueue(201, 2, 26.0, 3);
	dq.enqueue(202, 3, 80.0, 1);
	dq.pauseTask(200);
	dq.pauseTask(201);
	cout << "Decision queue with paused low-priority tasks:" << endl;
	dq.display();
	dq.processAll();

	FastCache cache(10);
	cache.store(3, 80.0, 75.0, 15.0);
	cache.store(4, 75.0, 70.0, 18.0);
	double t, s, h;
	cache.fetch(3, t, s, h);
	cache.fetch(3, t, s, h);
	cache.show();

	SystemMonitor mon;
	mon.setStartTime(0.0);
	mon.setFinishTime(12.0);
	mon.computeLatency();
	mon.setActiveTasks(90);
	mon.setCapacity(100);
	mon.computeLoad();
	mon.detectBottleneck("QueueLayer", 0.9);
	mon.optimizePerformance();

	dq.resumeTask(200);
	dq.resumeTask(201);
	dq.processAll();

	mon.setActiveTasks(40);
	mon.computeLoad();
	mon.viewSystemHealth();

	cout << "Scenario 4 complete." << endl;
}

// Scenario 5: multi-zone emergency, global sync, coordinated response
// Uses: all structures together
void ScenarioRunner::runGlobalEmergency() {
	cout << "SCENARIO 5: Global Multi-Zone Emergency Synchronization" << endl;

	ForestGrid grid;
	int r = 0;
	while (r < 5) {
		int c = 0;
		while (c < 5) {
			grid.setCell(r, c, 25.0 + r * 2.0, r * 5.0);
			c = c + 1;
		}
		r = r + 1;
	}
	grid.findBoundaries();

	DoublyList globalHistory;
	globalHistory.insertBack(25.0, 1, 1);
	globalHistory.insertBack(45.0, 2, 2);
	globalHistory.insertBack(70.0, 3, 3);
	globalHistory.insertBack(80.0, 4, 4);
	cout << "Global event history:" << endl;
	globalHistory.displayForward();
	globalHistory.correctBackward(2, 26.0);
	globalHistory.synchronize(30.0);
	globalHistory.displayForward();

	AdjListGraph graph(10);
	graph.addEdge(1, 2, 1.0);
	graph.addEdge(2, 3, 1.5);
	graph.addEdge(3, 4, 2.0);
	graph.addEdge(4, 5, 1.0);
	graph.setFireLevel(1, 0.3);
	graph.setFireLevel(2, 0.6);
	graph.setFireLevel(3, 0.9);
	graph.setFireLevel(4, 0.7);
	graph.setFireLevel(5, 0.5);
	graph.bfsFireSpread(3);
	graph.blockRoute(3, 4);
	graph.computeFireAwareCost(2, 3);

	DecisionTree dtree;
	dtree.buildDefault();
	float score = dtree.computeScore(0.9, 0.8, 0.85);
	dtree.globalDecision(score, 0.6);

	EmergencyQueue eq;
	eq.enqueue(1, 1, 0.3, 3);
	eq.enqueue(2, 2, 0.6, 2);
	eq.enqueue(3, 3, 0.9, 1);
	eq.enqueue(4, 4, 0.7, 1);
	eq.enqueue(5, 5, 0.5, 2);
	eq.display();

	ResourceTree rtree;
	rtree.buildDefault();
	rtree.checkWaterAvailability(50.0, 150.0);
	rtree.computePriority(0.9, 0.95);

	PrimaryIndexTable table(20);
	table.insert(1, 25.0, 10.0, 55.0);
	table.insert(2, 45.0, 40.0, 30.0);
	table.insert(3, 80.0, 90.0, 10.0);
	table.insert(4, 70.0, 75.0, 15.0);
	table.insert(5, 50.0, 55.0, 25.0);
	table.show();

	CircularList cloop;
	cloop.insert(0.3, 1, 1);
	cloop.insert(0.6, 2, 2);
	cloop.insert(0.9, 3, 3);
	cloop.insert(0.7, 4, 4);
	cloop.insert(0.5, 5, 5);
	cloop.monitorEmergency(0.5, 2);

	SystemMonitor mon;
	mon.setStartTime(0.0);
	mon.setFinishTime(8.0);
	mon.computeLatency();
	mon.setActiveTasks(95);
	mon.setCapacity(100);
	mon.computeLoad();
	mon.detectBottleneck("GraphLayer", 0.9);
	mon.viewSystemHealth();

	cout << "Scenario 5 complete." << endl;
}

// Runs all five scenarios in sequence
void ScenarioRunner::runFullSimulation() {
	cout << "RUNNING FULL SYSTEM SIMULATION" << endl;
	runCascadingFire();
	runSensorFailure();
	runMultiFactorAnomaly();
	runSystemOverload();
	runGlobalEmergency();
	cout << "Full simulation complete." << endl;
}