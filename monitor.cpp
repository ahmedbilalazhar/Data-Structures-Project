// monitor.cpp
#include "monitor.h"

SystemMonitor::SystemMonitor() {
	startTime = 0.0;
	finishTime = 0.0;
	activeTasks = 0;
	capacity = 100;
	latency = 0.0;
	load = 0.0;
	bottleneckDetected = false;
	int i = 0;
	while (i < 50) {
		bottleneckModule[i] = '\0';
		i = i + 1;
	}
}

// O(1) - record operation start time
void SystemMonitor::setStartTime(float t) {
	startTime = t;
	cout << "Monitor: Start time set to " << startTime << " ms" << endl;
}

// O(1) - record operation finish time
void SystemMonitor::setFinishTime(float t) {
	finishTime = t;
	cout << "Monitor: Finish time set to " << finishTime << " ms" << endl;
}

// O(1) - compute latency = finishTime - startTime
float SystemMonitor::computeLatency() {
	latency = finishTime - startTime;
	cout << "Monitor: Latency = " << finishTime << " - " << startTime << " = " << latency << " ms" << endl;
	if (latency > 5.0) {
		cout << "Monitor: WARNING - High latency detected." << endl;
	}
	else {
		cout << "Monitor: Latency is within normal range." << endl;
	}
	return latency;
}

// O(1) - set number of currently active tasks
void SystemMonitor::setActiveTasks(int tasks) {
	activeTasks = tasks;
	cout << "Monitor: Active tasks set to " << activeTasks << endl;
}

// O(1) - set maximum processing capacity
void SystemMonitor::setCapacity(int cap) {
	if (cap > 0) {
		capacity = cap;
		cout << "Monitor: Capacity set to " << capacity << endl;
	}
	else {
		cout << "Monitor: Invalid capacity value." << endl;
	}
}

// O(1) - compute load = activeTasks / capacity
float SystemMonitor::computeLoad() {
	if (capacity == 0) {
		cout << "Monitor: Capacity is zero, cannot compute load." << endl;
		return 0.0;
	}
	load = (float)activeTasks / (float)capacity;
	cout << "Monitor: Load = " << activeTasks << " / " << capacity << " = " << load << endl;
	if (load > 0.8) {
		cout << "Monitor: OVERLOAD detected." << endl;
	}
	else if (load > 0.5) {
		cout << "Monitor: Load is moderate." << endl;
	}
	else {
		cout << "Monitor: Load is normal." << endl;
	}
	return load;
}

// O(1) - check if a module's load exceeds 0.8 threshold
void SystemMonitor::detectBottleneck(const char* mod, float moduleLoad) {
	cout << "Monitor: Checking module [" << mod << "] load = " << moduleLoad << endl;
	if (moduleLoad > 0.8) {
		bottleneckDetected = true;
		int i = 0;
		while (mod[i] != '\0' && i < 49) {
			bottleneckModule[i] = mod[i];
			i = i + 1;
		}
		bottleneckModule[i] = '\0';
		cout << "Monitor: BOTTLENECK in [" << bottleneckModule << "]" << endl;
	}
	else {
		cout << "Monitor: Module [" << mod << "] is running normally." << endl;
	}
}

// O(1) - print performance suggestion based on current load
void SystemMonitor::optimizePerformance() {
	cout << "Monitor: Running performance optimization..." << endl;
	if (load > 0.8) {
		cout << "Monitor: Redistributing workload to reduce overload." << endl;
		cout << "Monitor: Pausing low-priority tasks." << endl;
		cout << "Monitor: Increasing cache usage for frequent reads." << endl;
	}
	else if (load > 0.5) {
		cout << "Monitor: Moderate load. Adjusting execution priorities." << endl;
	}
	else {
		cout << "Monitor: System is stable. No optimization needed." << endl;
	}
}

// O(1) - display full system health summary
void SystemMonitor::viewSystemHealth() {
	cout << "System Health Report:" << endl;
	cout << "  Start Time   : " << startTime << " ms" << endl;
	cout << "  Finish Time  : " << finishTime << " ms" << endl;
	cout << "  Latency      : " << latency << " ms" << endl;
	cout << "  Active Tasks : " << activeTasks << endl;
	cout << "  Capacity     : " << capacity << endl;
	cout << "  Load         : " << load << endl;
	if (bottleneckDetected) {
		cout << "  Bottleneck   : " << bottleneckModule << endl;
	}
	else {
		cout << "  Bottleneck   : None detected" << endl;
	}
	if (latency <= 5.0 && load <= 0.8) {
		cout << "  Status       : NORMAL" << endl;
	}
	else if (load > 0.8) {
		cout << "  Status       : OVERLOADED" << endl;
	}
	else {
		cout << "  Status       : WARNING" << endl;
	}
}

// O(1) - reset all monitoring counters
void SystemMonitor::reset() {
	startTime = 0.0;
	finishTime = 0.0;
	activeTasks = 0;
	latency = 0.0;
	load = 0.0;
	bottleneckDetected = false;
	int i = 0;
	while (i < 50) {
		bottleneckModule[i] = '\0';
		i = i + 1;
	}
	cout << "Monitor: All metrics reset." << endl;
}