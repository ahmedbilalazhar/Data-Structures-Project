#pragma once

#include <iostream>
using namespace std;

// Tree node used by all decision trees
class TreeNode {
public:
	int id;
	char label[50];
	float riskScore;
	TreeNode* left;
	TreeNode* right;

	TreeNode(int i, const char* lbl, float score);
};

// T1/T2: Zone Hierarchy and Sub-Zone Decomposition Tree
class ZoneTree {
private:
	TreeNode* root;
	int size;

	void insertHelper(TreeNode* node, int id, const char* label, float score);
	void displayHelper(TreeNode* node, int level);
	void clearHelper(TreeNode* node);

public:
	ZoneTree();

	void insert(int id, const char* label, float score);   // O(log n) avg - add zone node
	void display();                                         // O(n) - print tree with depth
	void buildDefault();                                    // O(1) - build preset zone layout
	float getRisk(int id);                                  // O(n) - find risk score by id
	void clear();                                           // O(n) - delete all tree nodes
};

// T3: Terrain Classification Tree
class TerrainTree {
private:
	TreeNode* root;

	void insertHelper(TreeNode* node, int id, const char* label, float score);
	void displayHelper(TreeNode* node, int level);
	void clearHelper(TreeNode* node);

public:
	TerrainTree();

	void insert(int id, const char* label, float score);   // O(log n) avg - add terrain node
	float computeTerrainRisk(float slope, float dry, float density);  // O(1) - formula based
	void display();                                         // O(n) - print terrain tree
	void buildDefault();                                    // O(1) - build default terrain
	void clear();                                           // O(n) - free all nodes
};

// T4/T5/T6: Resource Trees (water, fire control, equipment)
class ResourceTree {
private:
	TreeNode* root;

	void insertHelper(TreeNode* node, int id, const char* label, float score);
	void displayHelper(TreeNode* node, int level);
	void clearHelper(TreeNode* node);

public:
	ResourceTree();

	void insert(int id, const char* label, float score);   // O(log n) avg - add resource node
	float checkWaterAvailability(float available, float required);    // O(1) - ratio formula
	float computePriority(float risk, float impact);                  // O(1) - priority = risk * impact
	void display();                                         // O(n) - show resource tree
	void buildDefault();                                    // O(1) - build preset resources
	void clear();                                           // O(n) - delete all nodes
};

// T7/T8/T9: Incident Trees (fire, wildlife, human)
class IncidentTree {
private:
	TreeNode* root;

	void insertHelper(TreeNode* node, int id, const char* label, float score);
	void displayHelper(TreeNode* node, int level);
	void clearHelper(TreeNode* node);

public:
	IncidentTree();

	void insert(int id, const char* label, float score);   // O(log n) avg - add incident node
	float computeFireLevel(float alpha, float temp, float beta, float smoke);   // O(1) - fire formula
	float computeHumanRisk(float movement, float restrictedFactor);             // O(1) - human risk
	void display();                                         // O(n) - print incident tree
	void buildDefault();                                    // O(1) - build default incidents
	void classify(float value, float threshold);            // O(1) - classify as fire or not
	void clear();                                           // O(n) - remove all nodes
};

// T10/T11/T12: Decision Trees (local, regional, global)
class DecisionTree {
private:
	TreeNode* root;

	void insertHelper(TreeNode* node, int id, const char* label, float score);
	void displayHelper(TreeNode* node, int level);
	void clearHelper(TreeNode* node);

public:
	DecisionTree();

	void insert(int id, const char* label, float score);           // O(log n) avg - add node
	float computeScore(float fire, float smoke, float temp);       // O(1) - weighted score
	void localDecision(float risk);                                // O(1) - zone-level response
	void regionalDecision(float spreadRate);                       // O(1) - escalate check
	void globalDecision(float totalRisk, float threshold);         // O(1) - full system check
	void display();                                                // O(n) - print decision tree
	void buildDefault();                                           // O(1) - build preset tree
	void clear();                                                  // O(n) - free all memory
};