#include "linkedlist.h"

// ============================
// Node
// ============================

Node::Node(float v, int t, int z) {
	value = v;
	time = t;
	zone = z;
	next = NULL;
	prev = NULL;
}

// ============================
// SinglyList
// ============================

SinglyList::SinglyList() {
	head = NULL;
	size = 0;
}

// O(n) - traverse to end and link new node
void SinglyList::insert(float value, int time, int zone) {
	Node* newNode = new Node(value, time, zone);
	if (head == NULL) {
		head = newNode;
	}
	else {
		Node* cur = head;
		while (cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = newNode;
	}
	size = size + 1;
}

// O(n) - print all events in order
void SinglyList::display() {
	if (head == NULL) {
		cout << "  List is empty." << endl;
		return;
	}
	Node* cur = head;
	while (cur != NULL) {
		cout << "  [Time:" << cur->time << " Zone:" << cur->zone << " Value:" << cur->value << "]" << endl;
		cur = cur->next;
	}
    cout << endl;
}

// O(1) - check if list has no nodes
bool SinglyList::isEmpty() {
	if (head == NULL) {
		return true;
	}
	return false;
}

// O(1) - return head for external access
Node* SinglyList::getHead() {
	return head;
}

// O(1) - return current element count
int SinglyList::getSize() {
	return size;
}

// O(n) - delete all nodes and reset head
void SinglyList::clear() {
	Node* cur = head;
	while (cur != NULL) {
		Node* temp = cur;
		cur = cur->next;
		delete temp;
	}
	head = NULL;
	size = 0;
}

// O(n) - get last inserted value for comparison
float SinglyList::getLastValue() {
	if (head == NULL) {
		return -1;
	}
	Node* cur = head;
	while (cur->next != NULL) {
		cur = cur->next;
	}
	return cur->value;
}

// O(n) - remove node if change from previous exceeds delta (noise detection)
bool SinglyList::removeNoise(float delta) {
	if (head == NULL) {
		return false;
	}
	bool removed = false;
	Node* prev = NULL;
	Node* cur = head;
	float prevVal = cur->value;
	cur = cur->next;
	prev = head;

	while (cur != NULL) {
		float diff = cur->value - prevVal;
		if (diff < 0) {
			diff = diff * -1;
		}
		if (diff >= delta) {
			cout << "  Noise removed: Time=" << cur->time << " Zone=" << cur->zone << " Value=" << cur->value << endl;
			Node* temp = cur;
			prev->next = cur->next;
			cur = cur->next;
			delete temp;
			size = size - 1;
			removed = true;
		}
		else {
			prevVal = cur->value;
			prev = cur;
			cur = cur->next;
		}
	}
    cout << endl;
	return removed;
}

// O(n) - mark and print nodes that differ too much from normal value (anomaly stream)
void SinglyList::filterAnomalies(float normal, float theta) {
	if (head == NULL) {
		cout << "  No events to scan." << endl;
		return;
	}
	Node* cur = head;
	int count = 0;
	while (cur != NULL) {
		float diff = cur->value - normal;
		if (diff < 0) {
			diff = diff * -1;
		}
		if (diff > theta) {
			cout << "  ANOMALY -> Time:" << cur->time << " Zone:" << cur->zone << " Value:" << cur->value << endl;
			count = count + 1;
		}
		cur = cur->next;
	}
	if (count == 0) {
		cout << "  No anomalies found." << endl;
	}
    cout << endl;
}

// ============================
// DoublyList
// ============================

DoublyList::DoublyList() {
	head = NULL;
	tail = NULL;
	size = 0;
}

// O(1) - insert new event at front of chain
void DoublyList::insertFront(float value, int time, int zone) {
	Node* newNode = new Node(value, time, zone);
	if (head == NULL) {
		head = newNode;
		tail = newNode;
	}
	else {
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
	}
	size = size + 1;
}

// O(1) - insert new event at back of chain
void DoublyList::insertBack(float value, int time, int zone) {
	Node* newNode = new Node(value, time, zone);
	if (tail == NULL) {
		head = newNode;
		tail = newNode;
	}
	else {
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
	}
	size = size + 1;
}

// O(n) - show events from head to tail (forward history)
void DoublyList::displayForward() {
	if (head == NULL) {
		cout << "  List is empty." << endl;
		return;
	}
	Node* cur = head;
	while (cur != NULL) {
		cout << "  [Time:" << cur->time << " Zone:" << cur->zone << " Value:" << cur->value << "] -> ";
		cur = cur->next;
	}
	cout << "NULL" << endl;
    cout << endl;
}

// O(n) - show events from tail to head (backward correction review)
void DoublyList::displayBackward() {
	if (tail == NULL) {
		cout << "  List is empty." << endl;
		return;
	}
	Node* cur = tail;
	while (cur != NULL) {
		cout << "  [Time:" << cur->time << " Zone:" << cur->zone << " Value:" << cur->value << "] -> ";
		cur = cur->prev;
	}
	cout << "NULL" << endl;
    cout << endl;
}

// O(n) - update all events after given time to new value (forward correction chain L4)
void DoublyList::correctForward(int fromTime, float newVal) {
	Node* cur = head;
	int corrected = 0;
	while (cur != NULL) {
		if (cur->time >= fromTime) {
			cur->value = newVal;
			corrected = corrected + 1;
		}
		cur = cur->next;
	}
	cout << "  Forward correction applied to " << corrected << " events." << endl;
    cout << endl;
}

// O(n) - update all events before given time to new value (backward correction chain L5)
void DoublyList::correctBackward(int fromTime, float newVal) {
	Node* cur = tail;
	int corrected = 0;
	while (cur != NULL) {
		if (cur->time <= fromTime) {
			cur->value = newVal;
			corrected = corrected + 1;
		}
		cur = cur->prev;
	}
	cout << "  Backward correction applied to " << corrected << " events." << endl;
    cout << endl;
}

// O(n) - set all node values to global value for consistency (sync chain L6)
void DoublyList::synchronize(float globalVal) {
	Node* cur = head;
	while (cur != NULL) {
		cur->value = globalVal;
		cur = cur->next;
	}
	cout << "  All nodes synchronized to value: " << globalVal << endl;
    cout << endl;
}

// O(1) - return tail pointer for external access
Node* DoublyList::getTail() {
	return tail;
}

// O(1) - check if doubly list is empty
bool DoublyList::isEmpty() {
	if (head == NULL) {
		return true;
	}
	return false;
}

// O(n) - remove all nodes and reset pointers
void DoublyList::clear() {
	Node* cur = head;
	while (cur != NULL) {
		Node* temp = cur;
		cur = cur->next;
		delete temp;
	}
	head = NULL;
	tail = NULL;
	size = 0;
}

// ============================
// CircularList
// ============================

CircularList::CircularList() {
	head = NULL;
	size = 0;
}

// O(n) - insert at end and point last node back to head (circular)
void CircularList::insert(float value, int time, int zone) {
	Node* newNode = new Node(value, time, zone);
	if (head == NULL) {
		head = newNode;
		newNode->next = head;
	}
	else {
		Node* cur = head;
		while (cur->next != head) {
			cur = cur->next;
		}
		cur->next = newNode;
		newNode->next = head;
	}
	size = size + 1;
}

// O(n * rounds) - loop through all nodes for given number of rounds (L7, L8)
void CircularList::monitor(int rounds) {
	if (head == NULL) {
		cout << "  No nodes to monitor." << endl;
		return;
	}
	Node* cur = head;
	int count = 0;
	int total = rounds * size;
	while (count < total) {
		cout << "  Monitor -> Zone:" << cur->zone << " Value:" << cur->value << " Time:" << cur->time << endl;
		cur = cur->next;
		count = count + 1;
	}
}

// O(n * rounds) - loop and alert if value exceeds threshold (L9 emergency loop)
void CircularList::monitorEmergency(float threshold, int rounds) {
	if (head == NULL) {
		cout << "  No nodes to monitor." << endl;
		return;
	}
	Node* cur = head;
	int count = 0;
	int total = rounds * size;
	while (count < total) {
		if (cur->value > threshold) {
			cout << "  EMERGENCY -> Zone:" << cur->zone << " Value:" << cur->value << " exceeds threshold " << threshold << endl;
		}
		else {
			cout << "  OK -> Zone:" << cur->zone << " Value:" << cur->value << endl;
		}
		cur = cur->next;
		count = count + 1;
	}
}

// O(n * rounds) - loop and check if change between rounds is small (L10 stability loop)
void CircularList::monitorStability(float epsilon, int rounds) {
	if (head == NULL) {
		cout << "  No nodes to monitor." << endl;
		return;
	}
	Node* cur = head;
	float prevVal = cur->value;
	int count = 0;
	int total = rounds * size;
	while (count < total) {
		float diff = cur->value - prevVal;
		if (diff < 0) {
			diff = diff * -1;
		}
		if (diff < epsilon) {
			cout << "  STABLE -> Zone:" << cur->zone << " Value:" << cur->value << endl;
		}
		else {
			cout << "  UNSTABLE -> Zone:" << cur->zone << " Change:" << diff << endl;
		}
		prevVal = cur->value;
		cur = cur->next;
		count = count + 1;
	}
}

// O(n) - print each node once and stop at head (breaks the loop for display)
void CircularList::display() {
	if (head == NULL) {
		cout << "  Circular list is empty." << endl;
		return;
	}
	Node* cur = head;
	int count = 0;
	while (count < size) {
		cout << "  [Time:" << cur->time << " Zone:" << cur->zone << " Value:" << cur->value << "]" << endl;
		cur = cur->next;
		count = count + 1;
	}
}

// O(1) - check if list has no nodes
bool CircularList::isEmpty() {
	if (head == NULL) {
		return true;
	}
	return false;
}

// O(n) - break circular link and delete all nodes
void CircularList::clear() {
	if (head == NULL) {
		return;
	}
	Node* cur = head->next;
	while (cur != head) {
		Node* temp = cur;
		cur = cur->next;
		delete temp;
	}
	delete head;
	head = NULL;
	size = 0;
}

// O(1) - return node count
int CircularList::getSize() {
	return size;
}