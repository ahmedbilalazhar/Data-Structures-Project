// menu.h
#pragma once
#include "arrays.h"
#include "linkedlist.h"
#include "queue.h"
#include "tree.h"
#include "graph.h"
#include "hash.h"
#include "monitor.h"
#include "scenarios.h"

class Menu {
private:
	Baseline base;
	SensorStream stream;
	ForestGrid grid;
	Terrain terrain;

	SinglyList rawList;
	DoublyList corrList;
	CircularList loopList;

	RoutineQueue rq;
	SurveillanceQueue sq;
	EmergencyQueue eq;
	DecisionQueue dq;

	ZoneTree ztree;
	TerrainTree ttree;
	ResourceTree rtree;
	IncidentTree itree;
	DecisionTree dtree;

	AdjListGraph listGraph;
	AdjMatrixGraph matGraph;

	PrimaryIndexTable h1;
	CollisionTable h2;
	FastCache h3;

	SystemMonitor monitor;
	ScenarioRunner runner;

	int taskCounter;
	int eventCounter;

	void menuEnvironmental();
	void menuForestGrid();
	void menuEventMemory();
	void menuFireDetection();
	void menuTaskScheduling();
	void menuDecisionSystem();
	void menuSpatialRouting();
	void menuHashAccess();
	void menuMonitoring();
	void menuScenarios();

	void testEnvironmental();
	void testForestGrid();
	void testEventMemory();
	void testFireDetection();
	void testTaskScheduling();
	void testDecisionSystem();
	void testSpatialRouting();
	void testHashAccess();
	void testMonitoring();

public:
	Menu();
	void run();
};