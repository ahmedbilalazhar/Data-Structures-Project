// monitor.h
#pragma once
#include <iostream>
using namespace std;

// Tracks execution time, load, and bottlenecks across system modules
class SystemMonitor {
private:
	float startTime;
	float finishTime;
	int activeTasks;
	int capacity;
	float latency;
	float load;
	bool bottleneckDetected;
	char bottleneckModule[50];

public:
	SystemMonitor();

	void setStartTime(float t);              // O(1) - record operation start
	void setFinishTime(float t);             // O(1) - record operation end
	float computeLatency();                  // O(1) - latency = finish - start
	void setActiveTasks(int tasks);          // O(1) - update active task count
	void setCapacity(int cap);              // O(1) - set processing capacity
	float computeLoad();                    // O(1) - load = activeTasks / capacity
	void detectBottleneck(const char* mod, float moduleLoad);  // O(1) - flag overloaded module
	void optimizePerformance();             // O(1) - suggest load adjustment
	void viewSystemHealth();                // O(1) - print all metrics
	void reset();                           // O(1) - reset all counters
};