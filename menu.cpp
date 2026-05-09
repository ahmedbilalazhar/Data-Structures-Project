// menu.cpp
#include "menu.h"

Menu::Menu() : listGraph(10), matGraph(10), h1(20), h2(10), h3(10) {
	taskCounter = 1;
	eventCounter = 1;
	ztree.buildDefault();
	ttree.buildDefault();
	rtree.buildDefault();
	itree.buildDefault();
	dtree.buildDefault();
	listGraph.addEdge(0, 1, 2.0);
	listGraph.addEdge(1, 2, 3.0);
	listGraph.addEdge(2, 3, 1.5);
	listGraph.addEdge(3, 4, 2.5);
	matGraph.addEdge(0, 1, 2.0);
	matGraph.addEdge(1, 2, 3.0);
	matGraph.addEdge(2, 3, 1.5);
}

void Menu::menuEnvironmental() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "1. Environmental Data" << endl;
		cout << "  1.1 Add Sensor Reading" << endl;
		cout << "  1.2 Store in Dynamic Array" << endl;
		cout << "  1.3 Compare with Baseline" << endl;
		cout << "  1.4 Validate and Filter Noise" << endl;
		cout << "  0. Back" << endl;
		cout << "Choice: ";
		cin >> choice;
		if (choice == 1) {
			float t, s, h;
			cout << "Enter temp smoke humidity: ";
			cin >> t >> s >> h;
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
			stream.checkAnomalies(25.0, 0.0, 60.0);
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "Invalid choice." << endl;
		}
	}
}

void Menu::menuForestGrid() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "2. Forest Grid Status" << endl;
		cout << "  2.1 Display 1D Time Series" << endl;
		cout << "  2.2 Display 2D Forest Matrix" << endl;
		cout << "  2.3 View Zone Conditions" << endl;
		cout << "  0. Back" << endl;
		cout << "Choice: ";
		cin >> choice;
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
			cout << "Invalid choice." << endl;
		}
	}
}

void Menu::menuEventMemory() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "3. Event Memory System" << endl;
		cout << "  3.1 Store Event" << endl;
		cout << "  3.2 Traverse Forward" << endl;
		cout << "  3.3 Traverse Backward" << endl;
		cout << "  3.4 Circular Monitoring" << endl;
		cout << "  3.5 Restore Last Stable State" << endl;
		cout << "  0. Back" << endl;
		cout << "Choice: ";
		cin >> choice;
		if (choice == 1) {
			float v;
			int t, z;
			cout << "Enter value time zone: ";
			cin >> v >> t >> z;
			rawList.insert(v, t, z);
			corrList.insertBack(v, t, z);
			loopList.insert(v, t, z);
			eventCounter = eventCounter + 1;
			cout << "Event stored in all lists." << endl;
		}
		else if (choice == 2) {
			cout << "Forward traversal:" << endl;
			rawList.display();
		}
		else if (choice == 3) {
			cout << "Backward traversal:" << endl;
			corrList.displayBackward();
		}
		else if (choice == 4) {
			int rounds;
			cout << "Enter rounds: ";
			cin >> rounds;
			loopList.monitor(rounds);
		}
		else if (choice == 5) {
			float v;
			int t;
			cout << "Enter correction value and from-time: ";
			cin >> v >> t;
			corrList.correctBackward(t, v);
			corrList.displayForward();
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "Invalid choice." << endl;
		}
	}
}

void Menu::menuFireDetection() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "4. Fire Detection and Control" << endl;
		cout << "  4.1 Detect Fire Risk" << endl;
		cout << "  4.2 Trigger Emergency Alert" << endl;
		cout << "  4.3 Priority-Based Fire Response" << endl;
		cout << "  4.4 Simulate Fire Spread (BFS)" << endl;
		cout << "  4.5 Allocate Firefighting Resources" << endl;
		cout << "  0. Back" << endl;
		cout << "Choice: ";
		cin >> choice;
		if (choice == 1) {
			float fire, smoke, temp;
			cout << "Enter fire smoke temp (0-1 scale): ";
			cin >> fire >> smoke >> temp;
			float score = dtree.computeScore(fire, smoke, temp);
			dtree.localDecision(score);
		}
		else if (choice == 2) {
			int zone;
			float val;
			cout << "Enter zone and fire value: ";
			cin >> zone >> val;
			eq.enqueue(taskCounter, zone, val, 1);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 3) {
			eq.display();
			eq.dequeue();
		}
		else if (choice == 4) {
			int zone;
			cout << "Enter start zone for BFS (0-9): ";
			cin >> zone;
			listGraph.bfsFireSpread(zone);
		}
		else if (choice == 5) {
			float avail, req;
			cout << "Enter available and required water: ";
			cin >> avail >> req;
			rtree.checkWaterAvailability(avail, req);
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "Invalid choice." << endl;
		}
	}
}

void Menu::menuTaskScheduling() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "5. Task Scheduling System" << endl;
		cout << "  5.1 Add Routine Task" << endl;
		cout << "  5.2 Add Surveillance Task" << endl;
		cout << "  5.3 Add Emergency Task" << endl;
		cout << "  5.4 Process Tasks" << endl;
		cout << "  5.5 Pause and Resume Tasks" << endl;
		cout << "  0. Back" << endl;
		cout << "Choice: ";
		cin >> choice;
		if (choice == 1) {
			int zone;
			float val;
			cout << "Enter zone and value: ";
			cin >> zone >> val;
			rq.enqueue(taskCounter, zone, val);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 2) {
			int zone;
			float val;
			cout << "Enter zone and value: ";
			cin >> zone >> val;
			sq.enqueue(taskCounter, zone, val);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 3) {
			int zone, pri;
			float val;
			cout << "Enter zone value priority: ";
			cin >> zone >> val >> pri;
			eq.enqueue(taskCounter, zone, val, pri);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 4) {
			cout << "Processing emergency queue first..." << endl;
			eq.dequeue();
			cout << "Processing routine queue..." << endl;
			rq.dequeue();
			cout << "Processing surveillance queue..." << endl;
			sq.dequeue();
		}
		else if (choice == 5) {
			int tid;
			cout << "Enter task ID to pause (0 to resume instead): ";
			cin >> tid;
			if (tid == 0) {
				cout << "Enter task ID to resume: ";
				cin >> tid;
				dq.resumeTask(tid);
			}
			else {
				dq.pauseTask(tid);
			}
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "Invalid choice." << endl;
		}
	}
}

void Menu::menuDecisionSystem() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "6. Decision System" << endl;
		cout << "  6.1 Compute Risk Score" << endl;
		cout << "  6.2 Zone-Level Decision Tree" << endl;
		cout << "  6.3 Regional Decision Processing" << endl;
		cout << "  6.4 Global Emergency Decision" << endl;
		cout << "  6.5 Execute Final Action" << endl;
		cout << "  0. Back" << endl;
		cout << "Choice: ";
		cin >> choice;
		if (choice == 1) {
			float f, s, t;
			cout << "Enter fire smoke temp (0-1): ";
			cin >> f >> s >> t;
			dtree.computeScore(f, s, t);
		}
		else if (choice == 2) {
			float risk;
			cout << "Enter risk score: ";
			cin >> risk;
			dtree.localDecision(risk);
		}
		else if (choice == 3) {
			float rate;
			cout << "Enter fire spread rate: ";
			cin >> rate;
			dtree.regionalDecision(rate);
		}
		else if (choice == 4) {
			float total, threshold;
			cout << "Enter total risk and threshold: ";
			cin >> total >> threshold;
			dtree.globalDecision(total, threshold);
		}
		else if (choice == 5) {
			float f, s, t;
			cout << "Enter fire smoke temp for final action: ";
			cin >> f >> s >> t;
			float score = dtree.computeScore(f, s, t);
			dtree.localDecision(score);
			dtree.regionalDecision(score - 0.1);
			dtree.globalDecision(score, 0.6);
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "Invalid choice." << endl;
		}
	}
}

void Menu::menuSpatialRouting() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "7. Spatial Routing System" << endl;
		cout << "  7.1 Show Adjacency List Graph" << endl;
		cout << "  7.2 Show Adjacency Matrix Graph" << endl;
		cout << "  7.3 BFS Traversal" << endl;
		cout << "  7.4 DFS Traversal" << endl;
		cout << "  7.5 Compute Safe Path" << endl;
		cout << "  7.6 Update Blocked Routes" << endl;
		cout << "  0. Back" << endl;
		cout << "Choice: ";
		cin >> choice;
		if (choice == 1) {
			listGraph.show();
		}
		else if (choice == 2) {
			matGraph.show();
		}
		else if (choice == 3) {
			int zone;
			cout << "Enter start zone: ";
			cin >> zone;
			listGraph.bfsFireSpread(zone);
		}
		else if (choice == 4) {
			int zone;
			cout << "Enter start zone: ";
			cin >> zone;
			listGraph.dfsDeepAnalysis(zone);
		}
		else if (choice == 5) {
			int from, to;
			float danger;
			cout << "Enter from to danger: ";
			cin >> from >> to >> danger;
			listGraph.computePathCost(from, to, danger);
			listGraph.computeFireAwareCost(from, to);
		}
		else if (choice == 6) {
			int from, to;
			cout << "Enter from and to zone to block: ";
			cin >> from >> to;
			listGraph.blockRoute(from, to);
			matGraph.blockRoute(from, to);
		}
		else if (choice == 0) {
			again = 0;
		}
		else {
			cout << "Invalid choice." << endl;
		}
	}
}

void Menu::menuHashAccess() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "8. Hash-Based Fast Access" << endl;
		cout << "  8.1 Insert Data (Hash Table)" << endl;
		cout << "  8.2 Retrieve Data" << endl;
		cout << "  8.3 Handle Collisions (H2 chain insert)" << endl;
		cout << "  8.4 Update Cache" << endl;
		cout << "  8.5 View Index Table" << endl;
		cout << "  0. Back" << endl;
		cout << "Choice: ";
		cin >> choice;
		if (choice == 1) {
			int key;
			float t, s, h;
			cout << "Enter key temp smoke humidity: ";
			cin >> key >> t >> s >> h;
			h1.insert(key, t, s, h);
		}
		else if (choice == 2) {
			int key;
			cout << "Enter zone key: ";
			cin >> key;
			double t, s, h;
			bool found = h1.retrieve(key, t, s, h);
			if (found == false) {
				h2.retrieve(key, t, s, h);
			}
		}
		else if (choice == 3) {
			int key;
			float t, s, h;
			cout << "Enter key temp smoke humidity for chain table: ";
			cin >> key >> t >> s >> h;
			h2.insert(key, t, s, h);
			h2.show();
		}
		else if (choice == 4) {
			int key;
			float t, s, h;
			cout << "Enter key temp smoke humidity to cache: ";
			cin >> key >> t >> s >> h;
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
			cout << "Invalid choice." << endl;
		}
	}
}

void Menu::menuMonitoring() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "9. System Monitoring" << endl;
		cout << "  9.1 Monitor System Load" << endl;
		cout << "  9.2 Track Execution Time" << endl;
		cout << "  9.3 Detect Bottlenecks" << endl;
		cout << "  9.4 Optimize Performance" << endl;
		cout << "  9.5 View System Health" << endl;
		cout << "  0. Back" << endl;
		cout << "Choice: ";
		cin >> choice;
		if (choice == 1) {
			int tasks, cap;
			cout << "Enter active tasks and capacity: ";
			cin >> tasks >> cap;
			monitor.setActiveTasks(tasks);
			monitor.setCapacity(cap);
			monitor.computeLoad();
		}
		else if (choice == 2) {
			float start, finish;
			cout << "Enter start and finish time (ms): ";
			cin >> start >> finish;
			monitor.setStartTime(start);
			monitor.setFinishTime(finish);
			monitor.computeLatency();
		}
		else if (choice == 3) {
			char mod[50];
			float mload;
			cout << "Enter module name and its load (0-1): ";
			cin >> mod >> mload;
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
			cout << "Invalid choice." << endl;
		}
	}
}

void Menu::menuScenarios() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "10. Scenario Simulation" << endl;
		cout << "  10.1 Cascading Fire Scenario" << endl;
		cout << "  10.2 Sensor Failure Scenario" << endl;
		cout << "  10.3 Multi-Factor Anomaly Scenario" << endl;
		cout << "  10.4 System Overload Scenario" << endl;
		cout << "  10.5 Global Emergency Scenario" << endl;
		cout << "  10.6 Run Full Simulation" << endl;
		cout << "  0. Back" << endl;
		cout << "Choice: ";
		cin >> choice;
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
			cout << "Invalid choice." << endl;
		}
	}
}

void Menu::run() {
	int choice = -1;
	while (choice != 0) {
		cout << endl;
		cout << "IFAMDS - Intelligent Forest Advisory System" << endl;
		cout << "1.  Environmental Data" << endl;
		cout << "2.  Forest Grid Status" << endl;
		cout << "3.  Event Memory System" << endl;
		cout << "4.  Fire Detection and Control" << endl;
		cout << "5.  Task Scheduling System" << endl;
		cout << "6.  Decision System" << endl;
		cout << "7.  Spatial Routing System" << endl;
		cout << "8.  Hash-Based Fast Access" << endl;
		cout << "9.  System Monitoring" << endl;
		cout << "10. Scenario Simulation" << endl;
		cout << "0.  Exit System" << endl;
		cout << "Choice: ";
		cin >> choice;

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
			cout << "Exiting IFAMDS. Goodbye." << endl;
			break;
		default:
			cout << "Invalid choice. Try again." << endl;
			break;
		}
	}
}