#include "tree.h"
#include <cstring>

// ============================
// TreeNode
// ============================

TreeNode::TreeNode(int i, const char* lbl, float score) {
	id = i;
	int j = 0;
	while (lbl[j] != '\0' && j < 49) {
		label[j] = lbl[j];
		j = j + 1;
	}
	label[j] = '\0';
	riskScore = score;
	left = NULL;
	right = NULL;
}

// ============================
// ZoneTree (T1, T2)
// ============================

ZoneTree::ZoneTree() {
	root = NULL;
	size = 0;
}

// O(log n) avg - insert based on id comparison
void ZoneTree::insertHelper(TreeNode* node, int id, const char* label, float score) {
	if (id < node->id) {
		if (node->left == NULL) {
			node->left = new TreeNode(id, label, score);
		}
		else {
			insertHelper(node->left, id, label, score);
		}
	}
	else {
		if (node->right == NULL) {
			node->right = new TreeNode(id, label, score);
		}
		else {
			insertHelper(node->right, id, label, score);
		}
	}
}

// O(log n) avg - add zone into BST by id
void ZoneTree::insert(int id, const char* label, float score) {
	if (root == NULL) {
		root = new TreeNode(id, label, score);
	}
	else {
		insertHelper(root, id, label, score);
	}
	size = size + 1;
}

// O(n) - print tree showing depth using dashes
void ZoneTree::displayHelper(TreeNode* node, int level) {
	if (node == NULL) {
		return;
	}
	displayHelper(node->right, level + 1);
	int i = 0;
	while (i < level) {
		cout << "   ";
		i = i + 1;
	}
	cout << "[" << node->id << " | " << node->label << " | Risk:" << node->riskScore << "]" << endl;
	displayHelper(node->left, level + 1);
}

void ZoneTree::display() {
	if (root == NULL) {
		cout << "  Zone tree is empty." << endl;
		return;
	}
	cout << "  Zone Hierarchy Tree:" << endl;
	displayHelper(root, 0);
}

// O(1) - insert preset forest zone layout
void ZoneTree::buildDefault() {
	insert(5, "Forest-Root", 0.0);
	insert(3, "Zone-A", 0.3);
	insert(7, "Zone-B", 0.4);
	insert(1, "Zone-A1", 0.2);
	insert(4, "Zone-A2", 0.5);
	insert(6, "Zone-B1", 0.3);
	insert(9, "Zone-B2", 0.6);
	cout << "  Default zone hierarchy built." << endl;
}

// O(n) - search tree by id and return risk score
float ZoneTree::getRisk(int id) {
	TreeNode* cur = root;
	while (cur != NULL) {
		if (cur->id == id) {
			return cur->riskScore;
		}
		else if (id < cur->id) {
			cur = cur->left;
		}
		else {
			cur = cur->right;
		}
	}
	return -1;
}

// O(n) - recursive delete of all nodes
void ZoneTree::clearHelper(TreeNode* node) {
	if (node == NULL) {
		return;
	}
	clearHelper(node->left);
	clearHelper(node->right);
	delete node;
}

void ZoneTree::clear() {
	clearHelper(root);
	root = NULL;
	size = 0;
}

// ============================
// TerrainTree (T3)
// ============================

TerrainTree::TerrainTree() {
	root = NULL;
}

void TerrainTree::insertHelper(TreeNode* node, int id, const char* label, float score) {
	if (id < node->id) {
		if (node->left == NULL) {
			node->left = new TreeNode(id, label, score);
		}
		else {
			insertHelper(node->left, id, label, score);
		}
	}
	else {
		if (node->right == NULL) {
			node->right = new TreeNode(id, label, score);
		}
		else {
			insertHelper(node->right, id, label, score);
		}
	}
}

// O(log n) avg - insert terrain classification node
void TerrainTree::insert(int id, const char* label, float score) {
	if (root == NULL) {
		root = new TreeNode(id, label, score);
	}
	else {
		insertHelper(root, id, label, score);
	}
}

// O(1) - compute terrain risk using slope, dryness, density average
float TerrainTree::computeTerrainRisk(float slope, float dry, float density) {
	float risk = (slope + dry + density) / 3.0;
	cout << "  Terrain Risk = (" << slope << " + " << dry << " + " << density << ") / 3 = " << risk << endl;
	if (risk > 0.7) {
		cout << "  HIGH terrain risk detected." << endl;
	}
	else if (risk > 0.4) {
		cout << "  MODERATE terrain risk." << endl;
	}
	else {
		cout << "  LOW terrain risk." << endl;
	}
	return risk;
}

void TerrainTree::displayHelper(TreeNode* node, int level) {
	if (node == NULL) {
		return;
	}
	displayHelper(node->right, level + 1);
	int i = 0;
	while (i < level) {
		cout << "   ";
		i = i + 1;
	}
	cout << "[" << node->id << " | " << node->label << " | Score:" << node->riskScore << "]" << endl;
	displayHelper(node->left, level + 1);
}

// O(n) - print terrain classification tree
void TerrainTree::display() {
	if (root == NULL) {
		cout << "  Terrain tree is empty." << endl;
		return;
	}
	cout << "  Terrain Classification Tree:" << endl;
	displayHelper(root, 0);
}

// O(1) - build a basic terrain classification tree
void TerrainTree::buildDefault() {
	insert(5, "Terrain-Root", 0.5);
	insert(2, "Low-Risk-Plains", 0.2);
	insert(8, "High-Risk-Hills", 0.8);
	insert(1, "Safe-Valley", 0.1);
	insert(3, "Dry-Brush", 0.6);
	insert(7, "Rocky-Slope", 0.7);
	insert(9, "Dense-Forest", 0.9);
	cout << "  Default terrain tree built." << endl;
}

void TerrainTree::clearHelper(TreeNode* node) {
	if (node == NULL) {
		return;
	}
	clearHelper(node->left);
	clearHelper(node->right);
	delete node;
}

// O(n) - remove all terrain nodes
void TerrainTree::clear() {
	clearHelper(root);
	root = NULL;
}

// ============================
// ResourceTree (T4, T5, T6)
// ============================

ResourceTree::ResourceTree() {
	root = NULL;
}

void ResourceTree::insertHelper(TreeNode* node, int id, const char* label, float score) {
	if (id < node->id) {
		if (node->left == NULL) {
			node->left = new TreeNode(id, label, score);
		}
		else {
			insertHelper(node->left, id, label, score);
		}
	}
	else {
		if (node->right == NULL) {
			node->right = new TreeNode(id, label, score);
		}
		else {
			insertHelper(node->right, id, label, score);
		}
	}
}

// O(log n) avg - add resource allocation node to tree
void ResourceTree::insert(int id, const char* label, float score) {
	if (root == NULL) {
		root = new TreeNode(id, label, score);
	}
	else {
		insertHelper(root, id, label, score);
	}
}

// O(1) - water availability ratio: available / required
float ResourceTree::checkWaterAvailability(float available, float required) {
	if (required == 0) {
		cout << "  Required water is zero, no calculation needed." << endl;
		return 1.0;
	}
	float ratio = available / required;
	cout << "  Water Availability = " << available << " / " << required << " = " << ratio << endl;
	if (ratio >= 1.0) {
		cout << "  Water supply is sufficient." << endl;
	}
	else if (ratio >= 0.5) {
		cout << "  Water supply is limited." << endl;
	}
	else {
		cout << "  Water supply is critically low." << endl;
	}
	return ratio;
}

// O(1) - equipment priority = risk * impact
float ResourceTree::computePriority(float risk, float impact) {
	float priority = risk * impact;
	cout << "  Equipment Priority = " << risk << " x " << impact << " = " << priority << endl;
	return priority;
}

void ResourceTree::displayHelper(TreeNode* node, int level) {
	if (node == NULL) {
		return;
	}
	displayHelper(node->right, level + 1);
	int i = 0;
	while (i < level) {
		cout << "   ";
		i = i + 1;
	}
	cout << "[" << node->id << " | " << node->label << " | Score:" << node->riskScore << "]" << endl;
	displayHelper(node->left, level + 1);
}

// O(n) - display resource tree contents
void ResourceTree::display() {
	if (root == NULL) {
		cout << "  Resource tree is empty." << endl;
		return;
	}
	cout << "  Resource Allocation Tree:" << endl;
	displayHelper(root, 0);
}

// O(1) - build preset water, fire, equipment nodes
void ResourceTree::buildDefault() {
	insert(5, "Resource-Root", 0.5);
	insert(3, "Water-Tank-A", 0.8);
	insert(7, "Fire-Truck-B", 0.9);
	insert(1, "Pump-Station-1", 0.6);
	insert(4, "Hose-Unit-2", 0.7);
	insert(6, "Helicopter-1", 0.95);
	insert(9, "Ground-Team-A", 0.5);
	cout << "  Default resource tree built." << endl;
}

void ResourceTree::clearHelper(TreeNode* node) {
	if (node == NULL) {
		return;
	}
	clearHelper(node->left);
	clearHelper(node->right);
	delete node;
}

// O(n) - free all resource tree nodes
void ResourceTree::clear() {
	clearHelper(root);
	root = NULL;
}

// ============================
// IncidentTree (T7, T8, T9)
// ============================

IncidentTree::IncidentTree() {
	root = NULL;
}

void IncidentTree::insertHelper(TreeNode* node, int id, const char* label, float score) {
	if (id < node->id) {
		if (node->left == NULL) {
			node->left = new TreeNode(id, label, score);
		}
		else {
			insertHelper(node->left, id, label, score);
		}
	}
	else {
		if (node->right == NULL) {
			node->right = new TreeNode(id, label, score);
		}
		else {
			insertHelper(node->right, id, label, score);
		}
	}
}

// O(log n) avg - insert incident event into tree
void IncidentTree::insert(int id, const char* label, float score) {
	if (root == NULL) {
		root = new TreeNode(id, label, score);
	}
	else {
		insertHelper(root, id, label, score);
	}
}

// O(1) - fire level = alpha*temp + beta*smoke (T7)
float IncidentTree::computeFireLevel(float alpha, float temp, float beta, float smoke) {
	float level = (alpha * temp) + (beta * smoke);
	cout << "  Fire Level = (" << alpha << " x " << temp << ") + (" << beta << " x " << smoke << ") = " << level << endl;
	if (level > 0.7) {
		cout << "  MAJOR fire alert triggered." << endl;
	}
	else if (level > 0.4) {
		cout << "  MODERATE fire risk detected." << endl;
	}
	else {
		cout << "  LOW fire level, monitoring continues." << endl;
	}
	return level;
}

// O(1) - human risk = movement * restricted area factor (T9)
float IncidentTree::computeHumanRisk(float movement, float restrictedFactor) {
	float risk = movement * restrictedFactor;
	cout << "  Human Risk = " << movement << " x " << restrictedFactor << " = " << risk << endl;
	if (risk > 0.6) {
		cout << "  Human intrusion risk is HIGH." << endl;
	}
	else {
		cout << "  Human risk is within acceptable limits." << endl;
	}
	return risk;
}

void IncidentTree::displayHelper(TreeNode* node, int level) {
	if (node == NULL) {
		return;
	}
	displayHelper(node->right, level + 1);
	int i = 0;
	while (i < level) {
		cout << "   ";
		i = i + 1;
	}
	cout << "[" << node->id << " | " << node->label << " | Score:" << node->riskScore << "]" << endl;
	displayHelper(node->left, level + 1);
}

// O(n) - display the incident classification tree
void IncidentTree::display() {
	if (root == NULL) {
		cout << "  Incident tree is empty." << endl;
		return;
	}
	cout << "  Incident Classification Tree:" << endl;
	displayHelper(root, 0);
}

// O(1) - build preset fire, wildlife, human incident nodes
void IncidentTree::buildDefault() {
	insert(5, "Incident-Root", 0.5);
	insert(3, "Fire-Zone-3", 0.8);
	insert(7, "Wildlife-Zone-7", 0.4);
	insert(1, "Human-Activity-1", 0.6);
	insert(4, "Smoke-Alert-4", 0.7);
	insert(6, "Animal-Movement-6", 0.3);
	insert(9, "Intruder-Zone-9", 0.9);
	cout << "  Default incident tree built." << endl;
}

// O(1) - check if value crosses threshold and classify
void IncidentTree::classify(float value, float threshold) {
	if (value > threshold) {
		cout << "  Value " << value << " EXCEEDS threshold " << threshold << " -> Incident Confirmed." << endl;
	}
	else {
		cout << "  Value " << value << " is below threshold " << threshold << " -> No incident." << endl;
	}
}

void IncidentTree::clearHelper(TreeNode* node) {
	if (node == NULL) {
		return;
	}
	clearHelper(node->left);
	clearHelper(node->right);
	delete node;
}

// O(n) - delete all incident nodes
void IncidentTree::clear() {
	clearHelper(root);
	root = NULL;
}

// ============================
// DecisionTree (T10, T11, T12)
// ============================

DecisionTree::DecisionTree() {
	root = NULL;
}

void DecisionTree::insertHelper(TreeNode* node, int id, const char* label, float score) {
	if (id < node->id) {
		if (node->left == NULL) {
			node->left = new TreeNode(id, label, score);
		}
		else {
			insertHelper(node->left, id, label, score);
		}
	}
	else {
		if (node->right == NULL) {
			node->right = new TreeNode(id, label, score);
		}
		else {
			insertHelper(node->right, id, label, score);
		}
	}
}

// O(log n) avg - add decision node to tree
void DecisionTree::insert(int id, const char* label, float score) {
	if (root == NULL) {
		root = new TreeNode(id, label, score);
	}
	else {
		insertHelper(root, id, label, score);
	}
}

// O(1) - decision score = w1*fire + w2*smoke + w3*temp with fixed weights
float DecisionTree::computeScore(float fire, float smoke, float temp) {
	float w1 = 0.4;
	float w2 = 0.3;
	float w3 = 0.3;
	float score = (w1 * fire) + (w2 * smoke) + (w3 * temp);
	cout << "  Decision Score = 0.4(" << fire << ") + 0.3(" << smoke << ") + 0.3(" << temp << ") = " << score << endl;
	return score;
}

// O(1) - local zone decision: risk > 0.6 triggers local response (T10)
void DecisionTree::localDecision(float risk) {
	cout << "  Local Decision -> Risk Score: " << risk << endl;
	if (risk > 0.6) {
		cout << "  ACTION: Activate local zone response immediately." << endl;
	}
	else {
		cout << "  STATUS: Zone is under normal monitoring." << endl;
	}
}

// O(1) - regional decision: spread rate > 0.5 escalates alert (T11)
void DecisionTree::regionalDecision(float spreadRate) {
	cout << "  Regional Decision -> Fire Spread Rate: " << spreadRate << endl;
	if (spreadRate > 0.5) {
		cout << "  ACTION: Escalate alert to neighboring zones." << endl;
	}
	else {
		cout << "  STATUS: Spread rate is controlled. No escalation needed." << endl;
	}
}

// O(1) - global decision: totalRisk vs threshold triggers full system emergency (T12)
void DecisionTree::globalDecision(float totalRisk, float threshold) {
	cout << "  Global Decision -> Total Risk: " << totalRisk << " | Threshold: " << threshold << endl;
	if (totalRisk > threshold) {
		cout << "  ACTION: GLOBAL EMERGENCY ACTIVATED. Full system response initiated." << endl;
	}
	else {
		cout << "  STATUS: Global risk is within acceptable limits." << endl;
	}
}

void DecisionTree::displayHelper(TreeNode* node, int level) {
	if (node == NULL) {
		return;
	}
	displayHelper(node->right, level + 1);
	int i = 0;
	while (i < level) {
		cout << "   ";
		i = i + 1;
	}
	cout << "[" << node->id << " | " << node->label << " | Score:" << node->riskScore << "]" << endl;
	displayHelper(node->left, level + 1);
}

// O(n) - print decision tree structure
void DecisionTree::display() {
	if (root == NULL) {
		cout << "  Decision tree is empty." << endl;
		return;
	}
	cout << "  Decision Intelligence Tree:" << endl;
	displayHelper(root, 0);
}

// O(1) - build preset local, regional, global decision nodes
void DecisionTree::buildDefault() {
	insert(6, "Decision-Root", 0.5);
	insert(3, "Local-Zone-3", 0.7);
	insert(9, "Regional-Zone-9", 0.6);
	insert(1, "Zone-A-Local", 0.4);
	insert(5, "Zone-B-Regional", 0.5);
	insert(7, "Global-Alert", 0.9);
	insert(11, "Emergency-Response", 0.95);
	cout << "  Default decision tree built." << endl;
}

void DecisionTree::clearHelper(TreeNode* node) {
	if (node == NULL) {
		return;
	}
	clearHelper(node->left);
	clearHelper(node->right);
	delete node;
}

// O(n) - free all decision tree nodes from memory
void DecisionTree::clear() {
	clearHelper(root);
	root = NULL;
}