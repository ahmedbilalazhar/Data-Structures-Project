#include "graph.h"

// ============================================================
// AdjNode
// ============================================================

AdjNode::AdjNode(int z, double w) {
	zone = z;
	weight = w;
	next = NULL;
}

// ============================================================
// AdjListGraph — G1
// ============================================================

AdjListGraph::AdjListGraph(int maxZ) {
	maxZones = maxZ;
	zoneCount = 0;

	heads = new AdjNode * [maxZones];
	fireLevel = new double[maxZones];
	visited = new bool[maxZones];
	bfsQueue = new int[maxZones];
	dfsStack = new int[maxZones];

	for (int i = 0; i < maxZones; i++) {
		heads[i] = NULL;
		fireLevel[i] = 0.0;
		visited[i] = false;
	}

	bfsFront = 0;
	bfsBack = 0;
	dfsTop = -1;
}

AdjListGraph::~AdjListGraph() {
	for (int i = 0; i < maxZones; i++) {
		AdjNode* cur = heads[i];
		while (cur != NULL) {
			AdjNode* temp = cur;
			cur = cur->next;
			delete temp;
		}
	}
	delete[] heads;
	delete[] fireLevel;
	delete[] visited;
	delete[] bfsQueue;
	delete[] dfsStack;
}

// O(1) — adds a zone to the graph (just ensures index is valid)
void AdjListGraph::addZone(int zoneId) {
	if (zoneId < 0 || zoneId >= maxZones) {
		cout << "Zone ID " << zoneId << " is out of range." << endl;
		return;
	}
	zoneCount = zoneCount + 1;
	cout << "Zone " << zoneId << " registered in adjacency list graph." << endl;
   cout << endl;
}

// O(1) — adds undirected edge between two zones
void AdjListGraph::addEdge(int from, int to, double distance) {
	if (from < 0 || from >= maxZones || to < 0 || to >= maxZones) {
		cout << "Invalid zone IDs for edge." << endl;
		return;
	}

	AdjNode* nodeA = new AdjNode(to, distance);
	nodeA->next = heads[from];
	heads[from] = nodeA;

	AdjNode* nodeB = new AdjNode(from, distance);
	nodeB->next = heads[to];
	heads[to] = nodeB;

	cout << "Edge added between Zone " << from << " and Zone " << to
		<< " with distance " << distance << endl;
   cout << endl;
}

// O(1) — updates the fire level for a zone
void AdjListGraph::setFireLevel(int zone, double level) {
	if (zone < 0 || zone >= maxZones) {
		cout << "Invalid zone ID." << endl;
		return;
	}
	fireLevel[zone] = level;
	cout << "Fire level for Zone " << zone << " set to " << level << endl;
   cout << endl;
}

// BFS helper: enqueue
void AdjListGraph::bfsEnqueue(int z) {
	bfsQueue[bfsBack] = z;
	bfsBack = bfsBack + 1;
}

// BFS helper: dequeue
int AdjListGraph::bfsDequeue() {
	int val = bfsQueue[bfsFront];
	bfsFront = bfsFront + 1;
	return val;
}

// DFS helper: push
void AdjListGraph::dfsPush(int z) {
	dfsTop = dfsTop + 1;
	dfsStack[dfsTop] = z;
}

// DFS helper: pop
int AdjListGraph::dfsPop() {
	int val = dfsStack[dfsTop];
	dfsTop = dfsTop - 1;
	return val;
}

// O(V + E) — BFS from startZone, simulates fire spread level by level
void AdjListGraph::bfsFireSpread(int startZone) {
	if (startZone < 0 || startZone >= maxZones) {
		cout << "Invalid start zone." << endl;
		return;
	}

	for (int i = 0; i < maxZones; i++) {
		visited[i] = false;
	}

	bfsFront = 0;
	bfsBack = 0;

	cout << "BFS Fire Spread from Zone " << startZone << ":" << endl;

	visited[startZone] = true;
	bfsEnqueue(startZone);

	while (bfsFront < bfsBack) {
		int cur = bfsDequeue();
		cout << "  Visiting Zone " << cur
			<< " (fire level: " << fireLevel[cur] << ")" << endl;

		AdjNode* neighbor = heads[cur];
		while (neighbor != NULL) {
			int z = neighbor->zone;
			if (visited[z] == false) {
				visited[z] = true;
				bfsEnqueue(z);
				cout << "  Fire may spread from Zone " << cur
					<< " to Zone " << z
					<< " via path cost " << neighbor->weight << endl;
			}
			neighbor = neighbor->next;
		}
	}

	cout << "BFS traversal complete." << endl;
   cout << endl;
}

// O(V + E) — DFS from startZone, deep path analysis
void AdjListGraph::dfsDeepAnalysis(int startZone) {
	if (startZone < 0 || startZone >= maxZones) {
		cout << "Invalid start zone." << endl;
		return;
	}

	for (int i = 0; i < maxZones; i++) {
		visited[i] = false;
	}

	dfsTop = -1;

	cout << "DFS Deep Analysis from Zone " << startZone << ":" << endl;

	dfsPush(startZone);

	while (dfsTop >= 0) {
		int cur = dfsPop();

		if (visited[cur] == false) {
			visited[cur] = true;
			cout << "  Analysing Zone " << cur
				<< " (fire level: " << fireLevel[cur] << ")" << endl;

			AdjNode* neighbor = heads[cur];
			while (neighbor != NULL) {
				int z = neighbor->zone;
				if (visited[z] == false) {
					dfsPush(z);
				}
				neighbor = neighbor->next;
			}
		}
	}

	cout << "DFS traversal complete." << endl;
   cout << endl;
}

// O(E) — simple path cost: distance + danger value for a direct edge
double AdjListGraph::computePathCost(int from, int to, double dangerValue) {
	AdjNode* cur = heads[from];
	while (cur != NULL) {
		if (cur->zone == to) {
			double cost = cur->weight + dangerValue;
			cout << "Path cost from Zone " << from << " to Zone " << to
				<< ": distance=" << cur->weight
				<< " + danger=" << dangerValue
				<< " = " << cost << endl;
        cout << endl;
			return cost;
		}
		cur = cur->next;
	}
	cout << "No direct edge from Zone " << from << " to Zone " << to << endl;
    cout << endl;
	return -1.0;
}

// O(E) — fire-aware cost: distance * (1 + fireLevel of destination)
double AdjListGraph::computeFireAwareCost(int from, int to) {
	AdjNode* cur = heads[from];
	while (cur != NULL) {
		if (cur->zone == to) {
			double cost = cur->weight * (1.0 + fireLevel[to]);
			cout << "Fire-aware path cost from Zone " << from << " to Zone " << to
				<< ": " << cur->weight << " * (1 + " << fireLevel[to]
				<< ") = " << cost << endl;
        cout << endl;
			return cost;
		}
		cur = cur->next;
	}
	cout << "No direct edge from Zone " << from << " to Zone " << to << endl;
    cout << endl;
	return -1.0;
}

// O(E) — blocks a route by setting its weight to a very high value (9999)
void AdjListGraph::blockRoute(int from, int to) {
	AdjNode* cur = heads[from];
	while (cur != NULL) {
		if (cur->zone == to) {
			cur->weight = 9999.0;
		}
		cur = cur->next;
	}

	cur = heads[to];
	while (cur != NULL) {
		if (cur->zone == from) {
			cur->weight = 9999.0;
		}
		cur = cur->next;
	}

	cout << "Route between Zone " << from << " and Zone " << to
		<< " has been blocked." << endl;
   cout << endl;
}

// O(V + E) — prints all zones and their neighbor lists
void AdjListGraph::show() {
	cout << "Adjacency List Graph (G1):" << endl;
	for (int i = 0; i < maxZones; i++) {
		if (heads[i] != NULL) {
			cout << "  Zone " << i << " -> ";
			AdjNode* cur = heads[i];
			while (cur != NULL) {
				cout << "Zone " << cur->zone
					<< "(dist:" << cur->weight << ")";
				if (cur->next != NULL) {
					cout << " -> ";
				}
				cur = cur->next;
			}
			cout << endl;
		}
	}
   cout << endl;
}

// ============================================================
// AdjMatrixGraph — G2
// ============================================================

AdjMatrixGraph::AdjMatrixGraph(int maxZ) {
	maxZones = maxZ;
	zoneCount = 0;

	matrix = new double* [maxZones];
	fireLevel = new double[maxZones];
	visited = new bool[maxZones];
	bfsQueue = new int[maxZones];
	dfsStack = new int[maxZones];

	for (int i = 0; i < maxZones; i++) {
		matrix[i] = new double[maxZones];
		fireLevel[i] = 0.0;
		visited[i] = false;

		for (int j = 0; j < maxZones; j++) {
			matrix[i][j] = 0.0;
		}
	}

	bfsFront = 0;
	bfsBack = 0;
	dfsTop = -1;
}

AdjMatrixGraph::~AdjMatrixGraph() {
	for (int i = 0; i < maxZones; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
	delete[] fireLevel;
	delete[] visited;
	delete[] bfsQueue;
	delete[] dfsStack;
}

// O(1) — registers a zone (validates range)
void AdjMatrixGraph::addZone(int zoneId) {
	if (zoneId < 0 || zoneId >= maxZones) {
		cout << "Zone ID " << zoneId << " is out of range." << endl;
		return;
	}
	zoneCount = zoneCount + 1;
	cout << "Zone " << zoneId << " registered in adjacency matrix graph." << endl;
}

// O(1) — sets matrix[from][to] and matrix[to][from] to distance
void AdjMatrixGraph::addEdge(int from, int to, double distance) {
	if (from < 0 || from >= maxZones || to < 0 || to >= maxZones) {
		cout << "Invalid zone IDs for edge." << endl;
		return;
	}
	matrix[from][to] = distance;
	matrix[to][from] = distance;
	cout << "Matrix edge added between Zone " << from
		<< " and Zone " << to
		<< " with distance " << distance << endl;
}

// O(1) — sets fire level of a zone
void AdjMatrixGraph::setFireLevel(int zone, double level) {
	if (zone < 0 || zone >= maxZones) {
		cout << "Invalid zone ID." << endl;
		return;
	}
	fireLevel[zone] = level;
	cout << "Fire level for Zone " << zone << " set to " << level << endl;
}

void AdjMatrixGraph::bfsEnqueue(int z) {
	bfsQueue[bfsBack] = z;
	bfsBack = bfsBack + 1;
}

int AdjMatrixGraph::bfsDequeue() {
	int val = bfsQueue[bfsFront];
	bfsFront = bfsFront + 1;
	return val;
}

void AdjMatrixGraph::dfsPush(int z) {
	dfsTop = dfsTop + 1;
	dfsStack[dfsTop] = z;
}

int AdjMatrixGraph::dfsPop() {
	int val = dfsStack[dfsTop];
	dfsTop = dfsTop - 1;
	return val;
}

// O(V^2) — BFS on matrix, shows fire spread zone by zone
void AdjMatrixGraph::bfsFireSpread(int startZone) {
	if (startZone < 0 || startZone >= maxZones) {
		cout << "Invalid start zone." << endl;
		return;
	}

	for (int i = 0; i < maxZones; i++) {
		visited[i] = false;
	}

	bfsFront = 0;
	bfsBack = 0;

	cout << "BFS Fire Spread from Zone " << startZone << " (matrix):" << endl;

	visited[startZone] = true;
	bfsEnqueue(startZone);

	while (bfsFront < bfsBack) {
		int cur = bfsDequeue();
		cout << "  Visiting Zone " << cur
			<< " (fire level: " << fireLevel[cur] << ")" << endl;

		for (int j = 0; j < maxZones; j++) {
			if (matrix[cur][j] > 0.0 && visited[j] == false) {
				visited[j] = true;
				bfsEnqueue(j);
				cout << "  Fire may spread to Zone " << j
					<< " via distance " << matrix[cur][j] << endl;
			}
		}
	}

	cout << "BFS traversal complete." << endl;
}

// O(V^2) — DFS on matrix, deep single-path analysis
void AdjMatrixGraph::dfsDeepAnalysis(int startZone) {
	if (startZone < 0 || startZone >= maxZones) {
		cout << "Invalid start zone." << endl;
		return;
	}

	for (int i = 0; i < maxZones; i++) {
		visited[i] = false;
	}

	dfsTop = -1;

	cout << "DFS Deep Analysis from Zone " << startZone << " (matrix):" << endl;
	cout << endl;

	dfsPush(startZone);

	while (dfsTop >= 0) {
		int cur = dfsPop();

		if (visited[cur] == false) {
			visited[cur] = true;
			cout << "  Analysing Zone " << cur
				<< " (fire level: " << fireLevel[cur] << ")" << endl;

			for (int j = 0; j < maxZones; j++) {
				if (matrix[cur][j] > 0.0 && visited[j] == false) {
					dfsPush(j);
				}
			}
		}
	}

	cout << "DFS traversal complete." << endl;
   cout << endl;
}

// O(V) — simple path cost: matrix[from][to] + dangerValue
double AdjMatrixGraph::computePathCost(int from, int to, double dangerValue) {
	if (from < 0 || from >= maxZones || to < 0 || to >= maxZones) {
		cout << "Invalid zone IDs." << endl;
		return -1.0;
	}
	if (matrix[from][to] == 0.0) {
		cout << "No direct connection from Zone " << from << " to Zone " << to << endl;
		return -1.0;
	}
	double cost = matrix[from][to] + dangerValue;
	cout << "Path cost Zone " << from << " to Zone " << to
		<< ": distance=" << matrix[from][to]
		<< " + danger=" << dangerValue
		<< " = " << cost << endl;
	return cost;
}

// O(V) — fire-aware cost using matrix edge weight
double AdjMatrixGraph::computeFireAwareCost(int from, int to) {
	if (from < 0 || from >= maxZones || to < 0 || to >= maxZones) {
		cout << "Invalid zone IDs." << endl;
		return -1.0;
	}
	if (matrix[from][to] == 0.0) {
		cout << "No direct connection from Zone " << from << " to Zone " << to << endl;
		return -1.0;
	}
	double cost = matrix[from][to] * (1.0 + fireLevel[to]);
	cout << "Fire-aware cost Zone " << from << " to Zone " << to
		<< ": " << matrix[from][to]
		<< " * (1 + " << fireLevel[to] << ") = " << cost << endl;
	return cost;
}

// O(1) — blocks edge by zeroing matrix cell
void AdjMatrixGraph::blockRoute(int from, int to) {
	if (from < 0 || from >= maxZones || to < 0 || to >= maxZones) {
		cout << "Invalid zone IDs." << endl;
		return;
	}
	matrix[from][to] = 0.0;
	matrix[to][from] = 0.0;
	cout << "Route between Zone " << from << " and Zone " << to
		<< " has been blocked in matrix." << endl;
}

// O(V^2) — displays the entire adjacency matrix
void AdjMatrixGraph::show() {
	cout << "Adjacency Matrix Graph (G2):" << endl;
	cout << "     ";
	for (int i = 0; i < maxZones; i++) {
		cout << "Z" << i << "   ";
	}
	cout << endl;

	for (int i = 0; i < maxZones; i++) {
		cout << "Z" << i << "  ";
		for (int j = 0; j < maxZones; j++) {
			cout << " " << matrix[i][j] << "  ";
		}
		cout << endl;
	}
}