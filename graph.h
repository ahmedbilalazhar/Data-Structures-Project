#pragma once
#include <iostream>
using namespace std;

// G1: Adjacency List Graph — stores connections as linked neighbor lists
// G2: Adjacency Matrix Graph — stores connections as a 2D boolean grid

// Node in the adjacency list's neighbor chain
class AdjNode {
public:
	int zone;
	double weight;
	AdjNode* next;

	AdjNode(int z, double w);
};

// G1: Adjacency List Graph — efficient for sparse zone connections
class AdjListGraph {
private:
	int maxZones;
	int zoneCount;
	AdjNode** heads;    // array of linked lists, one per zone
	double* fireLevel;  // fire level per zone, used for path cost updates

	// BFS queue (manual array-based circular queue, no STL)
	int* bfsQueue;
	int bfsFront;
	int bfsBack;

	// DFS stack (manual array-based stack, no STL)
	int* dfsStack;
	int dfsTop;

	bool* visited;

	void bfsEnqueue(int z);      // helper: enqueue for BFS
	int  bfsDequeue();           // helper: dequeue for BFS
	void dfsPush(int z);         // helper: push for DFS
	int  dfsPop();               // helper: pop for DFS

public:
	AdjListGraph(int maxZ);
	~AdjListGraph();

	// O(1) — adds a zone node to the graph
	void addZone(int zoneId);

	// O(1) — connects two zones with a given distance
	void addEdge(int from, int to, double distance);

	// O(1) — updates fire level for a zone, affects path cost
	void setFireLevel(int zone, double level);

	// O(V + E) — BFS traversal starting from a source zone, shows fire spread
	void bfsFireSpread(int startZone);

	// O(V + E) — DFS traversal starting from a source zone, deep path analysis
	void dfsDeepAnalysis(int startZone);

	// O(E) — computes simple path cost: distance + danger value
	double computePathCost(int from, int to, double dangerValue);

	// O(E) — fire-aware path cost: distance * (1 + fireLevel)
	double computeFireAwareCost(int from, int to);

	// O(E) — marks an edge as blocked by setting its weight to a very high value
	void blockRoute(int from, int to);

	// O(V + E) — displays the full adjacency list
	void show();
};

// G2: Adjacency Matrix Graph — efficient for dense zone grids
class AdjMatrixGraph {
private:
	int maxZones;
	int zoneCount;
	double** matrix;    // 2D matrix: matrix[i][j] = edge weight, 0 means no edge
	double* fireLevel;  // fire level per zone

	int* bfsQueue;
	int bfsFront;
	int bfsBack;

	int* dfsStack;
	int dfsTop;

	bool* visited;

	void bfsEnqueue(int z);
	int  bfsDequeue();
	void dfsPush(int z);
	int  dfsPop();

public:
	AdjMatrixGraph(int maxZ);
	~AdjMatrixGraph();

	// O(1) — registers a zone index
	void addZone(int zoneId);

	// O(1) — sets matrix[from][to] and matrix[to][from] with given distance
	void addEdge(int from, int to, double distance);

	// O(1) — updates fire level for a zone
	void setFireLevel(int zone, double level);

	// O(V^2) — BFS on matrix, shows fire spread level by level
	void bfsFireSpread(int startZone);

	// O(V^2) — DFS on matrix, follows one deep path before backtracking
	void dfsDeepAnalysis(int startZone);

	// O(V) — computes path cost between two directly connected zones
	double computePathCost(int from, int to, double dangerValue);

	// O(V) — fire-aware cost for a direct edge
	double computeFireAwareCost(int from, int to);

	// O(1) — blocks an edge by setting matrix cell to 0
	void blockRoute(int from, int to);

	// O(V^2) — displays the full adjacency matrix
	void show();
};