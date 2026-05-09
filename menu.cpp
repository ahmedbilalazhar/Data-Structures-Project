// menu.cpp
#include "menu.h"

// O(1) - reads a valid integer from cin, recovers from bad input
static int readInt(const char* prompt) {
	int val;
	while (true) {
		cout << prompt;
		cin >> val;
		if (cin.good()) {
			return val;
		}
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Error: Expected a number. Try again." << endl;
	}
}

// O(1) - reads a valid float from cin, recovers from bad input
static float readFloat(const char* prompt) {
	float val;
	while (true) {
		cout << prompt;
		cin >> val;
		if (cin.good()) {
			return val;
		}
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Error: Expected a decimal number. Try again." << endl;
	}
}

Menu::Menu() : listGraph(10), matGraph(10), h1(20), h2(10), h3(10) {
	taskCounter = 1;
	eventCounter = 1;
	ztree.buildDefault();
	ttree.buildDefault();
	rtree.buildDefault();
	itree.buildDefault();
	dtree.buildDefault();
	listGraph.addEdge(0, 1, 2.0f);
	listGraph.addEdge(1, 2, 3.0f);
	listGraph.addEdge(2, 3, 1.5f);
	listGraph.addEdge(3, 4, 2.5f);
	matGraph.addEdge(0, 1, 2.0f);
	matGraph.addEdge(1, 2, 3.0f);
	matGraph.addEdge(2, 3, 1.5f);
}

void Menu::menuEnvironmental() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "Environmental Data" << endl;
		cout << "  1. Add sensor reading" << endl;
		cout << "  2. Show dynamic array" << endl;
		cout << "  3. Compare with baseline" << endl;
		cout << "  4. Validate and filter noise" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Choice: ");
		if (choice == 1) {
			float t = readFloat("  Temp: ");
			float s = readFloat("  Smoke: ");
			float h = readFloat("  Humidity: ");
			stream.add(t, s, h);
		}
		else if (choice == 2) {
			stream.show();
		}
		else if (choice == 3) {
			stream.show();
			base.show();
		}
		else if (choice == 4) {
			stream.checkAnomalies(25.0f, 0.0f, 60.0f);
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "No such option." << endl;
		}
	}
}

void Menu::menuForestGrid() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "Forest Grid Status" << endl;
		cout << "  1. 1D time series" << endl;
		cout << "  2. 2D forest matrix" << endl;
		cout << "  3. Zone conditions" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Choice: ");
		if (choice == 1) {
			stream.show();
		}
		else if (choice == 2) {
			grid.show();
		}
		else if (choice == 3) {
			terrain.show();
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "No such option." << endl;
		}
	}
}

void Menu::menuEventMemory() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "Event Memory System" << endl;
		cout << "  1. Store event" << endl;
		cout << "  2. Traverse forward" << endl;
		cout << "  3. Traverse backward" << endl;
		cout << "  4. Circular monitoring" << endl;
		cout << "  5. Restore last stable state" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Choice: ");
		if (choice == 1) {
			float v = readFloat("  Value: ");
			int t = readInt("  Time: ");
			int z = readInt("  Zone: ");
			rawList.insert(v, t, z);
			corrList.insertBack(v, t, z);
			loopList.insert(v, t, z);
			eventCounter = eventCounter + 1;
			cout << "Event stored." << endl;
		}
		else if (choice == 2) {
			cout << "Forward:" << endl;
			rawList.display();
		}
		else if (choice == 3) {
			cout << "Backward:" << endl;
			corrList.displayBackward();
		}
		else if (choice == 4) {
			int rounds = readInt("  Rounds: ");
			loopList.monitor(rounds);
		}
		else if (choice == 5) {
			float v = readFloat("  Correction value: ");
			int t = readInt("  From-time: ");
			corrList.correctBackward(t, v);
			corrList.displayForward();
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "No such option." << endl;
		}
	}
}

void Menu::menuFireDetection() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "Fire Detection and Control" << endl;
		cout << "  1. Detect fire risk" << endl;
		cout << "  2. Trigger emergency alert" << endl;
		cout << "  3. Priority-based fire response" << endl;
		cout << "  4. Simulate fire spread (BFS)" << endl;
		cout << "  5. Allocate firefighting resources" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Choice: ");
		if (choice == 1) {
			float fire = readFloat("  Fire (0-1): ");
			float smoke = readFloat("  Smoke (0-1): ");
			float temp = readFloat("  Temp (0-1): ");
			float score = dtree.computeScore(fire, smoke, temp);
			dtree.localDecision(score);
		}
		else if (choice == 2) {
			int zone = readInt("  Zone: ");
			float val = readFloat("  Fire value: ");
			eq.enqueue(taskCounter, zone, val, 1);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 3) {
			eq.display();
			eq.dequeue();
		}
		else if (choice == 4) {
			int zone = readInt("  Start zone (0-9): ");
			listGraph.bfsFireSpread(zone);
		}
		else if (choice == 5) {
			float avail = readFloat("  Available water: ");
			float req = readFloat("  Required water: ");
			rtree.checkWaterAvailability(avail, req);
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "No such option." << endl;
		}
	}
}

void Menu::menuTaskScheduling() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "Task Scheduling System" << endl;
		cout << "  1. Add routine task" << endl;
		cout << "  2. Add surveillance task" << endl;
		cout << "  3. Add emergency task" << endl;
		cout << "  4. Process tasks" << endl;
		cout << "  5. Pause / resume task" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Choice: ");
		if (choice == 1) {
			int zone = readInt("  Zone: ");
			float val = readFloat("  Value: ");
			rq.enqueue(taskCounter, zone, val);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 2) {
			int zone = readInt("  Zone: ");
			float val = readFloat("  Value: ");
			sq.enqueue(taskCounter, zone, val);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 3) {
			int zone = readInt("  Zone: ");
			float val = readFloat("  Value: ");
			int pri = readInt("  Priority: ");
			eq.enqueue(taskCounter, zone, val, pri);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 4) {
			cout << "Processing emergency queue..." << endl;
			eq.dequeue();
			cout << "Processing routine queue..." << endl;
			rq.dequeue();
			cout << "Processing surveillance queue..." << endl;
			sq.dequeue();
		}
		else if (choice == 5) {
			int tid = readInt("  Task ID to pause (0 to resume): ");
			if (tid == 0) {
				int rid = readInt("  Task ID to resume: ");
				dq.resumeTask(rid);
			}
			else {
				dq.pauseTask(tid);
			}
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "No such option." << endl;
		}
	}
}

void Menu::menuDecisionSystem() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "Decision System" << endl;
		cout << "  1. Compute risk score" << endl;
		cout << "  2. Zone-level decision" << endl;
		cout << "  3. Regional decision" << endl;
		cout << "  4. Global emergency decision" << endl;
		cout << "  5. Execute final action" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Choice: ");
		if (choice == 1) {
			float f = readFloat("  Fire (0-1): ");
			float s = readFloat("  Smoke (0-1): ");
			float t = readFloat("  Temp (0-1): ");
			dtree.computeScore(f, s, t);
		}
		else if (choice == 2) {
			float risk = readFloat("  Risk score: ");
			dtree.localDecision(risk);
		}
		else if (choice == 3) {
			float rate = readFloat("  Fire spread rate: ");
			dtree.regionalDecision(rate);
		}
		else if (choice == 4) {
			float total = readFloat("  Total risk: ");
			float threshold = readFloat("  Threshold: ");
			dtree.globalDecision(total, threshold);
		}
		else if (choice == 5) {
			float f = readFloat("  Fire (0-1): ");
			float s = readFloat("  Smoke (0-1): ");
			float t = readFloat("  Temp (0-1): ");
			float score = dtree.computeScore(f, s, t);
			dtree.localDecision(score);
			dtree.regionalDecision(score - 0.1f);
			dtree.globalDecision(score, 0.6f);
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "No such option." << endl;
		}
	}
}

void Menu::menuSpatialRouting() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "Spatial Routing System" << endl;
		cout << "  1. Show adjacency list graph" << endl;
		cout << "  2. Show adjacency matrix graph" << endl;
		cout << "  3. BFS traversal" << endl;
		cout << "  4. DFS traversal" << endl;
		cout << "  5. Compute safe path" << endl;
		cout << "  6. Block a route" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Choice: ");
		if (choice == 1) {
			listGraph.show();
		}
		else if (choice == 2) {
			matGraph.show();
		}
		else if (choice == 3) {
			int zone = readInt("  Start zone: ");
			listGraph.bfsFireSpread(zone);
		}
		else if (choice == 4) {
			int zone = readInt("  Start zone: ");
			listGraph.dfsDeepAnalysis(zone);
		}
		else if (choice == 5) {
			int from = readInt("  From zone: ");
			int to = readInt("  To zone: ");
			float danger = readFloat("  Danger factor: ");
			listGraph.computePathCost(from, to, danger);
			listGraph.computeFireAwareCost(from, to);
		}
		else if (choice == 6) {
			int from = readInt("  From zone: ");
			int to = readInt("  To zone: ");
			listGraph.blockRoute(from, to);
			matGraph.blockRoute(from, to);
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "No such option." << endl;
		}
	}
}

void Menu::menuHashAccess() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "Hash-Based Fast Access" << endl;
		cout << "  1. Insert into hash table" << endl;
		cout << "  2. Retrieve by key" << endl;
		cout << "  3. Chain insert (collision table)" << endl;
		cout << "  4. Update cache" << endl;
		cout << "  5. View all tables" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Choice: ");
		if (choice == 1) {
			int key = readInt("  Key: ");
			float t = readFloat("  Temp: ");
			float s = readFloat("  Smoke: ");
			float h = readFloat("  Humidity: ");
			h1.insert(key, t, s, h);
		}
		else if (choice == 2) {
			int key = readInt("  Zone key: ");
			double t, s, h;
			bool found = h1.retrieve(key, t, s, h);
			if (found == false) {
				h2.retrieve(key, t, s, h);
			}
		}
		else if (choice == 3) {
			int key = readInt("  Key: ");
			float t = readFloat("  Temp: ");
			float s = readFloat("  Smoke: ");
			float h = readFloat("  Humidity: ");
			h2.insert(key, t, s, h);
			h2.show();
		}
		else if (choice == 4) {
			int key = readInt("  Key: ");
			float t = readFloat("  Temp: ");
			float s = readFloat("  Smoke: ");
			float h = readFloat("  Humidity: ");
			h3.store(key, t, s, h);
			double dt, ds, dh;
			h3.fetch(key, dt, ds, dh);
		}
		else if (choice == 5) {
			h1.show();
			h2.show();
			h3.show();
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "No such option." << endl;
		}
	}
}

void Menu::menuMonitoring() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "System Monitoring" << endl;
		cout << "  1. Monitor system load" << endl;
		cout << "  2. Track execution time" << endl;
		cout << "  3. Detect bottlenecks" << endl;
		cout << "  4. Optimize performance" << endl;
		cout << "  5. View system health" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Choice: ");
		if (choice == 1) {
			int tasks = readInt("  Active tasks: ");
			int cap = readInt("  Capacity: ");
			monitor.setActiveTasks(tasks);
			monitor.setCapacity(cap);
			monitor.computeLoad();
		}
		else if (choice == 2) {
			float start = readFloat("  Start time (ms): ");
			float finish = readFloat("  Finish time (ms): ");
			monitor.setStartTime(start);
			monitor.setFinishTime(finish);
			monitor.computeLatency();
		}
		else if (choice == 3) {
			char mod[50];
			cout << "  Module name: ";
			cin >> mod;
			float mload = readFloat("  Load (0-1): ");
			monitor.detectBottleneck(mod, mload);
		}
		else if (choice == 4) {
			monitor.optimizePerformance();
		}
		else if (choice == 5) {
			monitor.viewSystemHealth();
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "No such option." << endl;
		}
	}
}

void Menu::menuScenarios() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "Scenario Simulation" << endl;
		cout << "  1. Cascading fire" << endl;
		cout << "  2. Sensor failure" << endl;
		cout << "  3. Multi-factor anomaly" << endl;
		cout << "  4. System overload" << endl;
		cout << "  5. Global emergency" << endl;
		cout << "  6. Full simulation" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Choice: ");
		if (choice == 1) {
			runner.runCascadingFire();
		}
		else if (choice == 2) {
			runner.runSensorFailure();
		}
		else if (choice == 3) {
			runner.runMultiFactorAnomaly();
		}
		else if (choice == 4) {
			runner.runSystemOverload();
		}
		else if (choice == 5) {
			runner.runGlobalEmergency();
		}
		else if (choice == 6) {
			runner.runFullSimulation();
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "No such option." << endl;
		}
	}
}

void Menu::run() {
	int choice = -1;
	while (choice != 0) {
		cout << endl;
		cout << "IFAMDS - Forest Advisory System" << endl;
		cout << "  1.  Environmental data" << endl;
		cout << "  2.  Forest grid status" << endl;
		cout << "  3.  Event memory" << endl;
		cout << "  4.  Fire detection and control" << endl;
		cout << "  5.  Task scheduling" << endl;
		cout << "  6.  Decision system" << endl;
		cout << "  7.  Spatial routing" << endl;
		cout << "  8.  Hash-based fast access" << endl;
		cout << "  9.  System monitoring" << endl;
		cout << "  10. Scenario simulation" << endl;
		cout << "  0.  Exit" << endl;
		choice = readInt("Choice: ");

		switch (choice) {
		case 1:
			menuEnvironmental();
			break;
		case 2:
			menuForestGrid();
			break;
		case 3:
			menuEventMemory();
			break;
		case 4:
			menuFireDetection();
			break;
		case 5:
			menuTaskScheduling();
			break;
		case 6:
			menuDecisionSystem();
			break;
		case 7:
			menuSpatialRouting();
			break;
		case 8:
			menuHashAccess();
			break;
		case 9:
			menuMonitoring();
			break;
		case 10:
			menuScenarios();
			break;
		case 0:
			cout << "Shutting down IFAMDS." << endl;
			break;
		default:
			cout << "No such option." << endl;
			break;
		}
	}
}