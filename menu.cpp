// menu.cpp
#include "menu.h"

// O(1) - reads a valid int within [lo, hi] from cin, retries on bad input or out-of-range
static int readInt(const char* prompt, int lo, int hi) {
	int val;
	while (true) {
		cout << prompt;
		cin >> val;
		if (cin.good() && val >= lo && val <= hi) {
			return val;
		}
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Error: Enter a number between " << lo << " and " << hi << "." << endl;
	}
}

// O(1) - reads a valid float from cin, retries on bad input
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
		cout << "Expected a decimal number. Try again." << endl;
	}
}

// O(1) - sets up graph edges and builds all default trees
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
	listGraph.addEdge(4, 5, 2.0);
	listGraph.addEdge(5, 6, 1.0);
	matGraph.addEdge(0, 1, 2.0);
	matGraph.addEdge(1, 2, 3.0);
	matGraph.addEdge(2, 3, 1.5);
	matGraph.addEdge(3, 4, 2.5);
	matGraph.addEdge(4, 5, 2.0);
}

// [Test] Environmental - adds readings, checks baseline, filters noise, scans terrain
void Menu::testEnvironmental() {
	cout << endl;
	cout << "[Test] Environmental Data" << endl;
	cout << endl;

	cout << endl;
	cout << "-> Setting baseline for zones 0, 1, 2..." << endl;
	base.set(0, 25.0f, 60.0f, 0.0f);
	base.set(1, 26.0f, 58.0f, 0.0f);
	base.set(2, 24.0f, 62.0f, 0.0f);

	cout << endl;
	cout << "-> Adding 3 sensor readings (normal, critical, normal)..." << endl;
	stream.add(27.0f, 10.0f, 55.0f);
	stream.add(48.0f, 80.0f, 15.0f);
	stream.add(22.0f, 5.0f, 65.0f);

	cout << endl;
	cout << "-> Sensor stream contents:" << endl;
	stream.show();

	cout << endl;
	cout << "-> Baseline table:" << endl;
	base.show();

	cout << endl;
	cout << "-> Anomaly check (temp>45, smoke>70, humid<20):" << endl;
	stream.checkAnomalies(45.0f, 70.0f, 20.0f);

	cout << endl;
	cout << "-> Inserting raw events for noise filter test..." << endl;
	rawList.insert(25.0f, 1, 0);
	rawList.insert(26.0f, 2, 0);
	rawList.insert(90.0f, 3, 0);
	rawList.insert(27.0f, 4, 0);

	cout << endl;
	cout << "-> Noise filter with delta=30.0:" << endl;
	bool removed = rawList.removeNoise(30.0f);
	if (removed == false) {
		cout << "  No noise detected." << endl;
	}

	cout << endl;
	cout << "-> Setting grid cell (2,2) to temp=50, smoke=85..." << endl;
	grid.setCell(2, 2, 50.0f, 85.0f);
	grid.setCell(0, 0, 25.0f, 5.0f);
	grid.setCell(4, 4, 30.0f, 10.0f);

	cout << endl;
	cout << "-> Updating terrain cell (1,1) with high risk values..." << endl;
	terrain.update(1, 1, 0.8f, 15.0f, 0.9f);

	cout << endl;
	cout << "Test done. Press Enter to return." << endl;
	cin.ignore(1000, '\n');
	cin.get();
}

// Menu 1 - Environmental Data Acquisition and Structuring
void Menu::menuEnvironmental() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "[ 1 ] Environmental Data" << endl;
		cout << "  1.  Add sensor reading" << endl;
		cout << "  2.  Show sensor stream" << endl;
		cout << "  3.  Set baseline for a zone" << endl;
		cout << "  4.  Show baseline table" << endl;
		cout << "  5.  Anomaly check vs thresholds" << endl;
		cout << "  6.  Filter noise from event stream" << endl;
		cout << "  7.  Update forest grid cell" << endl;
		cout << "  8.  Show forest grid" << endl;
		cout << "  9.  Update terrain cell" << endl;
		cout << "  10. Show terrain grid" << endl;
		cout << "  11. Run test sequence" << endl;
		cout << "  0.  Back" << endl;
		choice = readInt("Select (0-11): ", 0, 11);

		if (choice == 1) {
			float t = readFloat("  Temperature (C): ");
			float s = readFloat("  Smoke level (0-100): ");
			float h = readFloat("  Humidity (%): ");
			stream.add(t, s, h);
			// FIX (Bug 1): rawList was never populated from Menu 1, making
			// Option 6 (noise filter) always run on an empty list.
			// Now every sensor reading added here is also inserted into rawList.
			rawList.insert(t, eventCounter, 0);
			eventCounter = eventCounter + 1;
		}
		else if (choice == 2) {
			stream.show();
		}
		else if (choice == 3) {
			int zone = readInt("  Zone (0-9): ", 0, 9);
			float t = readFloat("  Normal temperature (C): ");
			float h = readFloat("  Normal humidity (%): ");
			float s = readFloat("  Normal smoke level: ");
			base.set(zone, t, h, s);
			cout << "  Baseline updated for zone " << zone << "." << endl;
		}
		else if (choice == 4) {
			base.show();
		}
		else if (choice == 5) {
			cout << "  Rules: temp > 45 C, smoke > 70, humidity < 20%" << endl;
			stream.checkAnomalies(45.0f, 70.0f, 20.0f);
		}
		else if (choice == 6) {
			float delta = readFloat("  Noise delta (e.g. 30.0): ");
			bool removed = rawList.removeNoise(delta);
			if (removed == false) {
				cout << "  No noisy readings found with delta " << delta << "." << endl;
			}
		}
		else if (choice == 7) {
			int r = readInt("  Row (0-4): ", 0, 4);
			int c = readInt("  Col (0-4): ", 0, 4);
			float t = readFloat("  Temperature (C): ");
			float s = readFloat("  Smoke level: ");
			grid.setCell(r, c, t, s);
		}
		else if (choice == 8) {
			grid.show();
		}
		else if (choice == 9) {
			int r = readInt("  Row (0-4): ", 0, 4);
			int c = readInt("  Col (0-4): ", 0, 4);
			float rk = readFloat("  Fire risk (0.0-1.0): ");
			float h = readFloat("  Humidity (%): ");
			float sl = readFloat("  Slope (0.0-1.0): ");
			terrain.update(r, c, rk, h, sl);
		}
		else if (choice == 10) {
			terrain.show();
		}
		else if (choice == 11) {
			testEnvironmental();
		}
		else if (choice == 0) {
			again = 0;
		}
	}
}

// [Test] Forest Grid - fills grid, interpolates missing cell, detects boundaries
void Menu::testForestGrid() {
	cout << endl;
	cout << "[Test] Forest Grid Status" << endl;
	cout << endl;

	cout << endl;
	cout << "-> Populating grid with temperature and smoke values..." << endl;
	grid.setCell(0, 0, 25.0f, 5.0f);
	grid.setCell(0, 1, 27.0f, 6.0f);
	grid.setCell(0, 3, 60.0f, 70.0f);
	grid.setCell(0, 4, 28.0f, 8.0f);
	grid.setCell(1, 0, 26.0f, 5.0f);
	grid.setCell(1, 2, 26.0f, 5.0f);
	grid.setCell(2, 2, 27.0f, 6.0f);

	cout << endl;
	cout << "-> Cell (0,2) is missing. Interpolating from neighbors..." << endl;
	grid.interpolate(0, 2);

	cout << endl;
	cout << "-> Grid after interpolation:" << endl;
	grid.show();

	cout << endl;
	cout << "-> Running boundary detection..." << endl;
	grid.findBoundaries();

	cout << endl;
	cout << "-> Terrain: setting one high-risk cell and one safe cell..." << endl;
	terrain.update(0, 0, 0.9f, 12.0f, 0.85f);
	terrain.update(2, 2, 0.3f, 55.0f, 0.2f);

	cout << endl;
	cout << "-> Terrain anomaly scan:" << endl;
	terrain.filterAnomalies();

	cout << endl;
	cout << "Test done. Press Enter to return." << endl;
	cin.ignore(1000, '\n');
	cin.get();
}

// Menu 2 - Forest Grid Status and Spatial Analysis
void Menu::menuForestGrid() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "[ 2 ] Forest Grid Status" << endl;
		cout << "  1. Show 1D time series (sensor stream)" << endl;
		cout << "  2. Show 2D forest zone matrix" << endl;
		cout << "  3. Interpolate a missing cell value" << endl;
		cout << "  4. Detect sharp temperature boundaries" << endl;
		cout << "  5. Show terrain conditions" << endl;
		cout << "  6. Scan terrain for anomalies" << endl;
		cout << "  7. Run test sequence" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Select (0-7): ", 0, 7);

		if (choice == 1) {
			stream.show();
		}
		else if (choice == 2) {
			grid.show();
		}
		else if (choice == 3) {
			int r = readInt("  Row (0-4): ", 0, 4);
			int c = readInt("  Col (0-4): ", 0, 4);
			grid.interpolate(r, c);
		}
		else if (choice == 4) {
			grid.findBoundaries();
		}
		else if (choice == 5) {
			terrain.show();
		}
		else if (choice == 6) {
			terrain.filterAnomalies();
		}
		else if (choice == 7) {
			testForestGrid();
		}
		else if (choice == 0) {
			again = 0;
		}
	}
}

// [Test] Event Memory - stores events, traverses both directions, corrects, monitors loop
void Menu::testEventMemory() {
	cout << endl;
	cout << "[Test] Event Memory System" << endl;
	cout << endl;

	cout << endl;
	cout << "-> Inserting 4 events into singly, doubly, and circular lists..." << endl;
	rawList.insert(25.0f, 10, 1);
	rawList.insert(30.0f, 20, 2);
	rawList.insert(55.0f, 30, 3);
	rawList.insert(28.0f, 40, 4);

	corrList.insertBack(25.0f, 10, 1);
	corrList.insertBack(30.0f, 20, 2);
	corrList.insertBack(55.0f, 30, 3);
	corrList.insertBack(28.0f, 40, 4);

	loopList.insert(25.0f, 10, 1);
	loopList.insert(30.0f, 20, 2);
	loopList.insert(55.0f, 30, 3);

	cout << endl;
	cout << "-> Forward traversal (singly list):" << endl;
	rawList.display();

	cout << endl;
	cout << "-> Backward traversal (doubly list):" << endl;
	corrList.displayBackward();

	cout << endl;
	cout << "-> Anomaly filter on singly list (normal=27, theta=10):" << endl;
	rawList.filterAnomalies(27.0f, 10.0f);

	cout << endl;
	cout << "-> Forward correction from time=20, new value=29.0..." << endl;
	corrList.correctForward(20, 29.0f);

	cout << endl;
	cout << "-> Doubly list after correction (forward view):" << endl;
	corrList.displayForward();

	cout << endl;
	cout << "-> Circular emergency scan (threshold=28.0, 2 rounds):" << endl;
	loopList.monitorEmergency(28.0f, 2);

	cout << endl;
	cout << "-> Stability scan (epsilon=6.0, 1 round):" << endl;
	loopList.monitorStability(6.0f, 1);

	cout << endl;
	cout << "Test done. Press Enter to return." << endl;
	cin.ignore(1000, '\n');
	cin.get();
}

// Menu 3 - Event Memory and Temporal Reconstruction
void Menu::menuEventMemory() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "[ 3 ] Event Memory System" << endl;
		cout << "  1.  Store event (singly + doubly + circular)" << endl;
		cout << "  2.  Traverse forward (singly list)" << endl;
		cout << "  3.  Traverse backward (doubly list)" << endl;
		cout << "  4.  Circular monitoring (basic scan)" << endl;
		cout << "  5.  Emergency circular monitoring" << endl;
		cout << "  6.  Stability circular monitoring" << endl;
		cout << "  7.  Filter anomalies from singly list" << endl;
		cout << "  8.  Forward correction (doubly list)" << endl;
		cout << "  9.  Backward correction / restore state" << endl;
		cout << "  10. Synchronize all doubly list events" << endl;
		cout << "  11. Run test sequence" << endl;
		cout << "  0.  Back" << endl;
		choice = readInt("Select (0-11): ", 0, 11);

		if (choice == 1) {
			float v = readFloat("  Value: ");
			int t = readInt("  Time stamp: ", 0, 2147483647);
			int z = readInt("  Zone (0-9): ", 0, 9);
			rawList.insert(v, t, z);
			corrList.insertBack(v, t, z);
			loopList.insert(v, t, z);
			eventCounter = eventCounter + 1;
			cout << "  Event stored in singly, doubly, and circular lists." << endl;
		}
		else if (choice == 2) {
			cout << "  Singly list (forward):" << endl;
			rawList.display();
		}
		else if (choice == 3) {
			cout << "  Doubly list (backward):" << endl;
			corrList.displayBackward();
		}
		else if (choice == 4) {
			int rounds = readInt("  Rounds to scan (1-5): ", 1, 5);
			loopList.monitor(rounds);
		}
		else if (choice == 5) {
			float thresh = readFloat("  Emergency alert threshold: ");
			int rounds = readInt("  Rounds (1-5): ", 1, 5);
			loopList.monitorEmergency(thresh, rounds);
		}
		else if (choice == 6) {
			float eps = readFloat("  Stability epsilon (e.g. 2.0): ");
			int rounds = readInt("  Rounds (1-5): ", 1, 5);
			loopList.monitorStability(eps, rounds);
		}
		else if (choice == 7) {
			float normal = readFloat("  Expected normal value: ");
			float theta = readFloat("  Deviation threshold (theta): ");
			rawList.filterAnomalies(normal, theta);
		}
		else if (choice == 8) {
			int fromTime = readInt("  Apply correction from time (inclusive): ", 0, 2147483647);
			float newVal = readFloat("  Replacement value: ");
			corrList.correctForward(fromTime, newVal);
			corrList.displayForward();
		}
		else if (choice == 9) {
			int fromTime = readInt("  Restore back to time: ", 0, 2147483647);
			float newVal = readFloat("  Correction value: ");
			corrList.correctBackward(fromTime, newVal);
			corrList.displayForward();
		}
		else if (choice == 10) {
			float gval = readFloat("  Global sync value: ");
			corrList.synchronize(gval);
			corrList.displayForward();
		}
		else if (choice == 11) {
			testEventMemory();
		}
		else if (choice == 0) {
			again = 0;
		}
	}
}

// [Test] Fire Detection - score, alert queue enqueue/dequeue, BFS spread, resource check
void Menu::testFireDetection() {
	cout << endl;
	cout << "[Test] Fire Detection and Control" << endl;
	cout << endl;

	cout << endl;
	cout << "-> Computing weighted risk score (fire=0.9, smoke=0.8, temp=0.75)..." << endl;
	float score = dtree.computeScore(0.9f, 0.8f, 0.75f);

	cout << endl;
	cout << "-> Local zone decision on computed score..." << endl;
	dtree.localDecision(score);

	cout << endl;
	cout << "-> Enqueueing 3 emergency alerts (zones 3, 4, 6 by priority)..." << endl;
	eq.enqueue(taskCounter, 3, 75.0f, 1);
	taskCounter = taskCounter + 1;
	eq.enqueue(taskCounter, 4, 60.0f, 2);
	taskCounter = taskCounter + 1;
	eq.enqueue(taskCounter, 6, 45.0f, 3);
	taskCounter = taskCounter + 1;

	cout << endl;
	cout << "-> Emergency queue state:" << endl;
	eq.display();

	cout << endl;
	cout << "-> Dequeuing highest priority task..." << endl;
	eq.dequeue();

	cout << endl;
	cout << "-> Setting fire levels and running BFS from Zone 3..." << endl;
	listGraph.setFireLevel(3, 0.9);
	listGraph.setFireLevel(4, 0.6);
	listGraph.bfsFireSpread(3);

	cout << endl;
	cout << "-> Water availability check (60L available, 100L required)..." << endl;
	rtree.checkWaterAvailability(60.0f, 100.0f);

	cout << endl;
	cout << "-> Equipment priority score (risk=0.9, impact=0.8)..." << endl;
	rtree.computePriority(0.9f, 0.8f);

	cout << endl;
	cout << "-> Fire level formula (alpha=0.4, temp=75, beta=0.3, smoke=85)..." << endl;
	itree.computeFireLevel(0.4f, 75.0f, 0.3f, 85.0f);

	cout << endl;
	cout << "Test done. Press Enter to return." << endl;
	cin.ignore(1000, '\n');
	cin.get();
}

// Menu 4 - Fire Detection and Control
void Menu::menuFireDetection() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "[ 4 ] Fire Detection and Control" << endl;
		cout << "  1. Detect fire risk (weighted score)" << endl;
		cout << "  2. Trigger emergency alert (enqueue)" << endl;
		cout << "  3. Priority-based fire response (dequeue)" << endl;
		cout << "  4. Simulate fire spread via BFS" << endl;
		cout << "  5. Allocate firefighting resources" << endl;
		cout << "  6. Set zone fire level in graph" << endl;
		cout << "  7. Compute incident fire level (formula)" << endl;
		cout << "  8. Run test sequence" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Select (0-8): ", 0, 8);

		if (choice == 1) {
			cout << "  Score = 0.4*fire + 0.3*smoke + 0.3*temp" << endl;
			float fire = readFloat("  Fire intensity (0.0-1.0): ");
			float smoke = readFloat("  Smoke factor (0.0-1.0): ");
			float temp = readFloat("  Temperature factor (0.0-1.0): ");
			float score = dtree.computeScore(fire, smoke, temp);
			dtree.localDecision(score);
		}
		else if (choice == 2) {
			int zone = readInt("  Zone (0-9): ", 0, 9);
			float val = readFloat("  Fire value: ");
			int pri = readInt("  Priority (1=highest, 5=lowest): ", 1, 5);
			eq.enqueue(taskCounter, zone, val, pri);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 3) {
			cout << "  Current emergency queue:" << endl;
			eq.display();
			eq.dequeue();
		}
		else if (choice == 4) {
			int zone = readInt("  Start zone (0-9): ", 0, 9);
			listGraph.bfsFireSpread(zone);
		}
		else if (choice == 5) {
			float avail = readFloat("  Available water (L): ");
			float req = readFloat("  Required water (L): ");
			rtree.checkWaterAvailability(avail, req);
			float risk = readFloat("  Zone risk (0.0-1.0): ");
			float impact = readFloat("  Impact factor (0.0-1.0): ");
			rtree.computePriority(risk, impact);
		}
		else if (choice == 6) {
			int zone = readInt("  Zone (0-9): ", 0, 9);
			float level = readFloat("  Fire level (0.0-1.0): ");
			listGraph.setFireLevel(zone, level);
			matGraph.setFireLevel(zone, level);
		}
		else if (choice == 7) {
			cout << "  Formula: fire_level = alpha*temp + beta*smoke" << endl;
			float alpha = readFloat("  Alpha weight (e.g. 0.4): ");
			float temp = readFloat("  Temperature (C): ");
			float beta = readFloat("  Beta weight (e.g. 0.3): ");
			float smoke = readFloat("  Smoke level: ");
			itree.computeFireLevel(alpha, temp, beta, smoke);
		}
		else if (choice == 8) {
			testFireDetection();
		}
		else if (choice == 0) {
			again = 0;
		}
	}
}

// [Test] Task Scheduling - fills all four queues, pauses tasks, processes, resumes
void Menu::testTaskScheduling() {
	cout << endl;
	cout << "[Test] Task Scheduling System" << endl;
	cout << endl;

	cout << endl;
	cout << "-> Adding 3 routine tasks to Q1..." << endl;
	rq.enqueue(taskCounter, 1, 22.0f);
	taskCounter = taskCounter + 1;
	rq.enqueue(taskCounter, 2, 24.0f);
	taskCounter = taskCounter + 1;
	rq.enqueue(taskCounter, 3, 23.0f);
	taskCounter = taskCounter + 1;

	cout << endl;
	cout << "-> Adding 2 surveillance tasks to Q2..." << endl;
	sq.enqueue(taskCounter, 4, 35.0f);
	taskCounter = taskCounter + 1;
	sq.enqueue(taskCounter, 5, 38.0f);
	taskCounter = taskCounter + 1;

	cout << endl;
	cout << "-> Adding 3 emergency tasks to Q3 (priority-sorted)..." << endl;
	eq.enqueue(taskCounter, 6, 80.0f, 1);
	taskCounter = taskCounter + 1;
	eq.enqueue(taskCounter, 7, 60.0f, 3);
	taskCounter = taskCounter + 1;
	eq.enqueue(taskCounter, 8, 70.0f, 2);
	taskCounter = taskCounter + 1;

	cout << endl;
	cout << "-> Adding 3 decision tasks to Q4..." << endl;
	int pauseId = taskCounter;
	dq.enqueue(taskCounter, 1, 25.0f, 3);
	taskCounter = taskCounter + 1;
	dq.enqueue(taskCounter, 2, 80.0f, 1);
	taskCounter = taskCounter + 1;
	dq.enqueue(taskCounter, 3, 26.0f, 3);
	taskCounter = taskCounter + 1;

	cout << endl;
	cout << "-> All queues before processing:" << endl;
	rq.display();
	sq.display();
	eq.display();
	dq.display();

	cout << endl;
	cout << "-> Processing one task from each queue..." << endl;
	eq.dequeue();
	rq.dequeue();
	sq.dequeue();
	dq.dequeue();

	cout << endl;
	cout << "-> Pausing first decision task (id=" << pauseId << ")..." << endl;
	dq.pauseTask(pauseId);

	cout << endl;
	cout << "-> Running all non-paused decision tasks..." << endl;
	dq.processAll();

	cout << endl;
	cout << "-> Resuming paused task and running again..." << endl;
	dq.resumeTask(pauseId);
	dq.processAll();

	cout << endl;
	cout << "Test done. Press Enter to return." << endl;
	cin.ignore(1000, '\n');
	cin.get();
}

// Menu 5 - Task Scheduling and Priority Allocation
void Menu::menuTaskScheduling() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "[ 5 ] Task Scheduling System" << endl;
		cout << "  1. Add routine task (Q1)" << endl;
		cout << "  2. Add surveillance task (Q2)" << endl;
		cout << "  3. Add emergency task (Q3, priority-sorted)" << endl;
		cout << "  4. Add decision task (Q4)" << endl;
		cout << "  5. Process one task from each queue" << endl;
		cout << "  6. Run all non-paused decision tasks (Q4)" << endl;
		cout << "  7. Pause a decision task" << endl;
		cout << "  8. Resume a decision task" << endl;
		cout << "  9. View all queues" << endl;
		cout << "  10. Run test sequence" << endl;
		cout << "  0.  Back" << endl;
		choice = readInt("Select (0-10): ", 0, 10);

		if (choice == 1) {
			int zone = readInt("  Zone (0-9): ", 0, 9);
			float val = readFloat("  Sensor value: ");
			rq.enqueue(taskCounter, zone, val);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 2) {
			int zone = readInt("  Zone (0-9): ", 0, 9);
			float val = readFloat("  Sensor value: ");
			sq.enqueue(taskCounter, zone, val);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 3) {
			int zone = readInt("  Zone (0-9): ", 0, 9);
			float val = readFloat("  Sensor value: ");
			int pri = readInt("  Priority (1=highest, 5=lowest): ", 1, 5);
			eq.enqueue(taskCounter, zone, val, pri);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 4) {
			int zone = readInt("  Zone (0-9): ", 0, 9);
			float val = readFloat("  Combined sensor value: ");
			int pri = readInt("  Priority (1-5): ", 1, 5);
			dq.enqueue(taskCounter, zone, val, pri);
			taskCounter = taskCounter + 1;
		}
		else if (choice == 5) {
			cout << "  Emergency queue (processed first):" << endl;
			eq.dequeue();
			cout << "  Routine queue:" << endl;
			rq.dequeue();
			cout << "  Surveillance queue:" << endl;
			sq.dequeue();
			cout << "  Decision queue:" << endl;
			dq.dequeue();
		}
		else if (choice == 6) {
			dq.processAll();
		}
		else if (choice == 7) {
			int tid = readInt("  Task ID to pause: ", 1, 2147483647);
			dq.pauseTask(tid);
		}
		else if (choice == 8) {
			int tid = readInt("  Task ID to resume: ", 1, 2147483647);
			dq.resumeTask(tid);
		}
		else if (choice == 9) {
			rq.display();
			sq.display();
			eq.display();
			dq.display();
		}
		else if (choice == 10) {
			testTaskScheduling();
		}
		else if (choice == 0) {
			again = 0;
		}
	}
}

// [Test] Decision System - score, local/regional/global decisions, terrain, human risk
void Menu::testDecisionSystem() {
	cout << endl;
	cout << "[Test] Decision System" << endl;
	cout << endl;

	cout << endl;
	cout << "-> Computing risk score (fire=0.85, smoke=0.75, temp=0.8)..." << endl;
	float score = dtree.computeScore(0.85f, 0.75f, 0.8f);

	cout << endl;
	cout << "-> Local decision on score..." << endl;
	dtree.localDecision(score);

	cout << endl;
	cout << "-> Regional decision (spread rate=0.65)..." << endl;
	dtree.regionalDecision(0.65f);

	cout << endl;
	cout << "-> Global decision (total risk=0.82, threshold=0.6)..." << endl;
	dtree.globalDecision(0.82f, 0.6f);

	cout << endl;
	cout << "-> Full decision chain (fire=0.7, smoke=0.6, temp=0.5)..." << endl;
	float chain = dtree.computeScore(0.7f, 0.6f, 0.5f);
	dtree.localDecision(chain);
	float spread = chain - 0.1f;
	if (spread < 0.0f) {
		spread = 0.0f;
	}
	dtree.regionalDecision(spread);
	dtree.globalDecision(chain, 0.6f);

	cout << endl;
	cout << "-> Zone hierarchy tree:" << endl;
	ztree.display();

	cout << endl;
	cout << "-> Terrain risk (slope=0.7, dryness=0.8, density=0.6)..." << endl;
	ttree.computeTerrainRisk(0.7f, 0.8f, 0.6f);

	cout << endl;
	cout << "-> Incident classification (value=0.75, threshold=0.6)..." << endl;
	itree.classify(0.75f, 0.6f);

	cout << endl;
	cout << "-> Human intrusion risk (movement=0.8, restricted=0.9)..." << endl;
	itree.computeHumanRisk(0.8f, 0.9f);

	cout << endl;
	cout << "Test done. Press Enter to return." << endl;
	cin.ignore(1000, '\n');
	cin.get();
}

// Menu 6 - Hierarchical Decision Intelligence
void Menu::menuDecisionSystem() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "[ 6 ] Decision System" << endl;
		cout << "  1.  Compute risk score (weighted formula)" << endl;
		cout << "  2.  Zone-level decision" << endl;
		cout << "  3.  Regional escalation decision" << endl;
		cout << "  4.  Global emergency decision" << endl;
		cout << "  5.  Execute full decision chain" << endl;
		cout << "  6.  Show zone hierarchy tree" << endl;
		cout << "  7.  Compute terrain risk" << endl;
		cout << "  8.  Classify incident by threshold" << endl;
		cout << "  9.  Compute human intrusion risk" << endl;
		cout << "  10. Show all trees" << endl;
		cout << "  11. Run test sequence" << endl;
		cout << "  0.  Back" << endl;
		choice = readInt("Select (0-11): ", 0, 11);

		if (choice == 1) {
			cout << "  Score = 0.4*fire + 0.3*smoke + 0.3*temp" << endl;
			float f = readFloat("  Fire (0.0-1.0): ");
			float s = readFloat("  Smoke (0.0-1.0): ");
			float t = readFloat("  Temp (0.0-1.0): ");
			dtree.computeScore(f, s, t);
		}
		else if (choice == 2) {
			float risk = readFloat("  Risk score (0.0-1.0, threshold=0.6): ");
			dtree.localDecision(risk);
		}
		else if (choice == 3) {
			float rate = readFloat("  Fire spread rate (0.0-1.0, threshold=0.5): ");
			dtree.regionalDecision(rate);
		}
		else if (choice == 4) {
			float total = readFloat("  Total risk score: ");
			float thresh = readFloat("  Emergency threshold (e.g. 0.6): ");
			dtree.globalDecision(total, thresh);
		}
		else if (choice == 5) {
			cout << "  Running full decision chain..." << endl;
			float f = readFloat("  Fire (0.0-1.0): ");
			float s = readFloat("  Smoke (0.0-1.0): ");
			float t = readFloat("  Temp (0.0-1.0): ");
			float score = dtree.computeScore(f, s, t);
			dtree.localDecision(score);
			float spread = score - 0.1f;
			if (spread < 0.0f) {
				spread = 0.0f;
			}
			dtree.regionalDecision(spread);
			dtree.globalDecision(score, 0.6f);
		}
		else if (choice == 6) {
			ztree.display();
		}
		else if (choice == 7) {
			cout << "  Formula: terrain_risk = (slope + dryness + density) / 3" << endl;
			float slope = readFloat("  Slope (0.0-1.0): ");
			float dry = readFloat("  Dryness (0.0-1.0): ");
			float density = readFloat("  Density (0.0-1.0): ");
			ttree.computeTerrainRisk(slope, dry, density);
		}
		else if (choice == 8) {
			float val = readFloat("  Incident value: ");
			float thresh = readFloat("  Classification threshold: ");
			itree.classify(val, thresh);
		}
		else if (choice == 9) {
			cout << "  Formula: human_risk = movement * restricted_factor" << endl;
			float movement = readFloat("  Movement factor (0.0-1.0): ");
			float factor = readFloat("  Restricted area factor (0.0-1.0): ");
			itree.computeHumanRisk(movement, factor);
		}
		else if (choice == 10) {
			ztree.display();
			ttree.display();
			rtree.display();
			itree.display();
			dtree.display();
		}
		else if (choice == 11) {
			testDecisionSystem();
		}
		else if (choice == 0) {
			again = 0;
		}
	}
}

// [Test] Spatial Routing - adds edges, runs BFS and DFS on both graphs, blocks a route
void Menu::testSpatialRouting() {
	cout << endl;
	cout << "[Test] Spatial Routing System" << endl;
	cout << endl;

	cout << endl;
	cout << "-> Adding extra edge between zone 6 and zone 7..." << endl;
	listGraph.addEdge(6, 7, 1.8);
	matGraph.addEdge(6, 7, 1.8);

	cout << endl;
	cout << "-> Setting fire levels on zones 2, 3, 4..." << endl;
	listGraph.setFireLevel(2, 0.5);
	listGraph.setFireLevel(3, 0.9);
	listGraph.setFireLevel(4, 0.6);
	matGraph.setFireLevel(2, 0.5);
	matGraph.setFireLevel(3, 0.9);
	matGraph.setFireLevel(4, 0.6);

	cout << endl;
	cout << "-> BFS fire spread from Zone 2 (adjacency list)..." << endl;
	listGraph.bfsFireSpread(2);

	cout << endl;
	cout << "-> DFS deep analysis from Zone 2 (adjacency list)..." << endl;
	listGraph.dfsDeepAnalysis(2);

	cout << endl;
	cout << "-> BFS fire spread from Zone 2 (adjacency matrix)..." << endl;
	matGraph.bfsFireSpread(2);

	cout << endl;
	cout << "-> DFS deep analysis from Zone 2 (adjacency matrix)..." << endl;
	matGraph.dfsDeepAnalysis(2);

	cout << endl;
	cout << "-> Simple path cost Zone 2 -> Zone 3 (danger=0.5)..." << endl;
	listGraph.computePathCost(2, 3, 0.5);

	cout << endl;
	cout << "-> Fire-aware path cost Zone 2 -> Zone 3..." << endl;
	listGraph.computeFireAwareCost(2, 3);
	matGraph.computeFireAwareCost(2, 3);

	cout << endl;
	cout << "-> Blocking route between Zone 3 and Zone 4..." << endl;
	listGraph.blockRoute(3, 4);
	matGraph.blockRoute(3, 4);

	cout << endl;
	cout << "-> Adjacency list graph after block:" << endl;
	listGraph.show();

	cout << endl;
	cout << "Test done. Press Enter to return." << endl;
	cin.ignore(1000, '\n');
	cin.get();
}

// Menu 7 - Spatial Connectivity and Graph-Based Routing
void Menu::menuSpatialRouting() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "[ 7 ] Spatial Routing System" << endl;
		cout << "  1.  Show adjacency list graph (G1)" << endl;
		cout << "  2.  Show adjacency matrix graph (G2)" << endl;
		cout << "  3.  Add edge to both graphs" << endl;
		cout << "  4.  BFS fire spread - adj list (G1)" << endl;
		cout << "  5.  DFS deep analysis - adj list (G1)" << endl;
		cout << "  6.  BFS fire spread - matrix (G2)" << endl;
		cout << "  7.  DFS deep analysis - matrix (G2)" << endl;
		cout << "  8.  Compute simple path cost" << endl;
		cout << "  9.  Compute fire-aware path cost" << endl;
		cout << "  10. Block a route" << endl;
		cout << "  11. Set zone fire level" << endl;
		cout << "  12. Run test sequence" << endl;
		cout << "  0.  Back" << endl;
		choice = readInt("Select (0-12): ", 0, 12);

		if (choice == 1) {
			listGraph.show();
		}
		else if (choice == 2) {
			matGraph.show();
		}
		else if (choice == 3) {
			int from = readInt("  From zone (0-9): ", 0, 9);
			int to = readInt("  To zone (0-9): ", 0, 9);
			float dist = readFloat("  Distance/weight: ");
			listGraph.addEdge(from, to, dist);
			matGraph.addEdge(from, to, dist);
		}
		else if (choice == 4) {
			int zone = readInt("  Start zone (0-9): ", 0, 9);
			listGraph.bfsFireSpread(zone);
		}
		else if (choice == 5) {
			int zone = readInt("  Start zone (0-9): ", 0, 9);
			listGraph.dfsDeepAnalysis(zone);
		}
		else if (choice == 6) {
			int zone = readInt("  Start zone (0-9): ", 0, 9);
			matGraph.bfsFireSpread(zone);
		}
		else if (choice == 7) {
			int zone = readInt("  Start zone (0-9): ", 0, 9);
			matGraph.dfsDeepAnalysis(zone);
		}
		else if (choice == 8) {
			cout << "  Formula: cost = distance + danger" << endl;
			int from = readInt("  From zone (0-9): ", 0, 9);
			int to = readInt("  To zone (0-9): ", 0, 9);
			float danger = readFloat("  Danger value: ");
			listGraph.computePathCost(from, to, danger);
		}
		else if (choice == 9) {
			cout << "  Formula: cost = distance * (1 + fire_level)" << endl;
			int from = readInt("  From zone (0-9): ", 0, 9);
			int to = readInt("  To zone (0-9): ", 0, 9);
			listGraph.computeFireAwareCost(from, to);
			matGraph.computeFireAwareCost(from, to);
		}
		else if (choice == 10) {
			int from = readInt("  From zone (0-9): ", 0, 9);
			int to = readInt("  To zone (0-9): ", 0, 9);
			listGraph.blockRoute(from, to);
			matGraph.blockRoute(from, to);
		}
		else if (choice == 11) {
			int zone = readInt("  Zone (0-9): ", 0, 9);
			float level = readFloat("  Fire level (0.0-1.0): ");
			listGraph.setFireLevel(zone, level);
			matGraph.setFireLevel(zone, level);
		}
		else if (choice == 12) {
			testSpatialRouting();
		}
		else if (choice == 0) {
			again = 0;
		}
	}
}

// [Test] Hash Access - inserts into H1 and H2, triggers collision, caches, evicts
void Menu::testHashAccess() {
	cout << endl;
	cout << "[Test] Hash-Based Fast Access" << endl;
	cout << endl;

	cout << endl;
	cout << "-> Inserting zones 3, 4, 6 into primary table (H1)..." << endl;
	h1.insert(3, 75.0, 85.0, 15.0);
	h1.insert(4, 65.0, 78.0, 18.0);
	h1.insert(6, 55.0, 60.0, 25.0);

	cout << endl;
	cout << "-> Retrieving zone 4 from H1..." << endl;
	double t, s, h;
	bool found = h1.retrieve(4, t, s, h);
	if (found == false) {
		cout << "  Not found in H1." << endl;
	}

	cout << endl;
	cout << "-> Updating zone 3 in H1 (new temp=80)..." << endl;
	h1.update(3, 80.0, 85.0, 15.0);

	cout << endl;
	cout << "-> Inserting zones 3 and 13 into collision table (H2)..." << endl;
	cout << "   (Keys 3 and 13 hash to same bucket in a size-10 table)" << endl;
	h2.insert(3, 75.0, 85.0, 15.0);
	h2.insert(13, 50.0, 40.0, 30.0);

	cout << endl;
	cout << "-> Retrieving zone 13 from H2..." << endl;
	h2.retrieve(13, t, s, h);

	cout << endl;
	cout << "-> Storing zones 3 and 4 in fast cache (H3)..." << endl;
	h3.store(3, 80.0, 85.0, 15.0);
	h3.store(4, 65.0, 78.0, 18.0);

	cout << endl;
	cout << "-> Fetching zone 3 twice (access count should increment)..." << endl;
	h3.fetch(3, t, s, h);
	h3.fetch(3, t, s, h);

	cout << endl;
	cout << "-> Trying to fetch zone 9 (cache miss)..." << endl;
	h3.fetch(9, t, s, h);

	cout << endl;
	cout << "-> Evicting zone 4 from cache..." << endl;
	h3.evict(4);

	cout << endl;
	cout << "-> Full table view:" << endl;
	h1.show();
	h2.show();
	h3.show();

	cout << endl;
	cout << "Test done. Press Enter to return." << endl;
	cin.ignore(1000, '\n');
	cin.get();
}

// Menu 8 - Hash-Based Indexing and Fast Retrieval
void Menu::menuHashAccess() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "[ 8 ] Hash-Based Fast Access" << endl;
		cout << "  1. Insert into primary table (H1)" << endl;
		cout << "  2. Retrieve by key (H1, fallback H2)" << endl;
		cout << "  3. Insert into collision table (H2)" << endl;
		cout << "  4. Store in cache (H3)" << endl;
		cout << "  5. Fetch from cache (H3)" << endl;
		cout << "  6. Update entry in H1" << endl;
		cout << "  7. Evict entry from cache (H3)" << endl;
		cout << "  8. View all hash tables" << endl;
		cout << "  9. Run test sequence" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Select (0-9): ", 0, 9);

		if (choice == 1) {
			int key = readInt("  Zone key: ", 0, 2147483647);
			float t = readFloat("  Temperature: ");
			float s = readFloat("  Smoke: ");
			float h = readFloat("  Humidity: ");
			h1.insert(key, t, s, h);
		}
		else if (choice == 2) {
			int key = readInt("  Zone key: ", 0, 2147483647);
			double t, s, h;
			bool found = h1.retrieve(key, t, s, h);
			if (found == false) {
				cout << "  Not found in H1. Checking H2..." << endl;
				h2.retrieve(key, t, s, h);
			}
		}
		else if (choice == 3) {
			int key = readInt("  Zone key: ", 0, 2147483647);
			float t = readFloat("  Temperature: ");
			float s = readFloat("  Smoke: ");
			float h = readFloat("  Humidity: ");
			h2.insert(key, t, s, h);
		}
		else if (choice == 4) {
			int key = readInt("  Zone key: ", 0, 2147483647);
			float t = readFloat("  Temperature: ");
			float s = readFloat("  Smoke: ");
			float h = readFloat("  Humidity: ");
			h3.store(key, t, s, h);
		}
		else if (choice == 5) {
			int key = readInt("  Zone key: ", 0, 2147483647);
			double t, s, h;
			h3.fetch(key, t, s, h);
		}
		else if (choice == 6) {
			int key = readInt("  Zone key: ", 0, 2147483647);
			float t = readFloat("  New temperature: ");
			float s = readFloat("  New smoke: ");
			float h = readFloat("  New humidity: ");
			h1.update(key, t, s, h);
		}
		else if (choice == 7) {
			int key = readInt("  Zone key to evict: ", 0, 2147483647);
			h3.evict(key);
		}
		else if (choice == 8) {
			h1.show();
			h2.show();
			h3.show();
		}
		else if (choice == 9) {
			testHashAccess();
		}
		else if (choice == 0) {
			again = 0;
		}
	}
}

// [Test] System Monitoring - sets load, latency, detects bottleneck, optimizes, resets
void Menu::testMonitoring() {
	cout << endl;
	cout << "[Test] System Monitoring" << endl;
	cout << endl;

	cout << endl;
	cout << "-> Setting active tasks=85, capacity=100 and computing load..." << endl;
	monitor.setActiveTasks(85);
	monitor.setCapacity(100);
	monitor.computeLoad();

	cout << endl;
	cout << "-> Recording execution time (start=0ms, finish=12ms)..." << endl;
	monitor.setStartTime(0.0f);
	monitor.setFinishTime(12.0f);
	monitor.computeLatency();

	cout << endl;
	cout << "-> Checking module QueueLayer for bottleneck (load=0.92)..." << endl;
	monitor.detectBottleneck("QueueLayer", 0.92f);

	cout << endl;
	cout << "-> Running performance optimization..." << endl;
	monitor.optimizePerformance();

	cout << endl;
	cout << "-> Full system health report:" << endl;
	monitor.viewSystemHealth();

	cout << endl;
	cout << "-> Reducing load to 30 tasks and re-checking..." << endl;
	monitor.setActiveTasks(30);
	monitor.computeLoad();

	cout << endl;
	cout << "-> Resetting all metrics..." << endl;
	monitor.reset();

	cout << endl;
	cout << "-> Health report after reset:" << endl;
	monitor.viewSystemHealth();

	cout << endl;
	cout << "Test done. Press Enter to return." << endl;
	cin.ignore(1000, '\n');
	cin.get();
}

// Menu 9 - System Monitoring and Adaptive Optimization
void Menu::menuMonitoring() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "[ 9 ] System Monitoring" << endl;
		cout << "  1. Set active tasks and compute load" << endl;
		cout << "  2. Track execution time and latency" << endl;
		cout << "  3. Detect bottleneck in a module" << endl;
		cout << "  4. Optimize performance" << endl;
		cout << "  5. View system health report" << endl;
		cout << "  6. Reset all monitoring metrics" << endl;
		cout << "  7. Run test sequence" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Select (0-7): ", 0, 7);

		if (choice == 1) {
			int tasks = readInt("  Active tasks: ", 0, 2147483647);
			int cap = readInt("  Processing capacity: ", 1, 2147483647);
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
			float mload = readFloat("  Module load (0.0-1.0): ");
			monitor.detectBottleneck(mod, mload);
		}
		else if (choice == 4) {
			monitor.optimizePerformance();
		}
		else if (choice == 5) {
			monitor.viewSystemHealth();
		}
		else if (choice == 6) {
			monitor.reset();
		}
		else if (choice == 7) {
			testMonitoring();
		}
		else if (choice == 0) {
			again = 0;
		}
	}
}

// Menu 10 - Scenario-Based System Evaluation
void Menu::menuScenarios() {
	int choice;
	int again = 1;
	while (again == 1) {
		cout << endl;
		cout << "[ 10 ] Scenario Simulation" << endl;
		cout << "  1. Cascading fire (Zone 3 -> 4, 6)" << endl;
		cout << "  2. Sensor failure and reconstruction" << endl;
		cout << "  3. Multi-factor anomaly escalation" << endl;
		cout << "  4. System overload and recovery" << endl;
		cout << "  5. Global multi-zone emergency" << endl;
		cout << "  6. Run full simulation (all 5 scenarios)" << endl;
		cout << "  0. Back" << endl;
		choice = readInt("Select (0-6): ", 0, 6);

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
	}
}

// O(1) per iteration - top-level menu loop, switch used only here
void Menu::run() {
	int choice = -1;
	while (choice != 0) {
		cout << endl;
		cout << "IFAMDS  Forest Advisory System" << endl;
		cout << "  1.  Environmental data" << endl;
		cout << "  2.  Forest grid status" << endl;
		cout << "  3.  Event memory system" << endl;
		cout << "  4.  Fire detection and control" << endl;
		cout << "  5.  Task scheduling system" << endl;
		cout << "  6.  Decision system" << endl;
		cout << "  7.  Spatial routing system" << endl;
		cout << "  8.  Hash-based fast access" << endl;
		cout << "  9.  System monitoring" << endl;
		cout << "  10. Scenario simulation" << endl;
		cout << "  0.  Exit" << endl;
		choice = readInt("Select (0-10): ", 0, 10);

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
			cout << "Shutting down." << endl;
			break;
		default:
			cout << "No such option." << endl;
			break;
		}
	}
}