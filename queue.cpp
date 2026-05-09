#include "queue.h"

// ============================
// TaskNode
// ============================

TaskNode::TaskNode(int id, int z, float v, int p) {
	taskId = id;
	zone = z;
	value = v;
	priority = p;
	paused = false;
	next = NULL;
}

// ============================
// RoutineQueue (Q1)
// ============================

RoutineQueue::RoutineQueue() {
	front = NULL;
	rear = NULL;
	size = 0;
}

// O(1) - add new routine sensor task at back of queue
void RoutineQueue::enqueue(int id, int zone, float value) {
	TaskNode* newNode = new TaskNode(id, zone, value, 3);
	if (rear == NULL) {
		front = newNode;
		rear = newNode;
	}
	else {
		rear->next = newNode;
		rear = newNode;
	}
	size = size + 1;
	cout << "  [Q1] Task " << id << " added -> Zone:" << zone << " Value:" << value << endl;
    cout << endl;
}

// O(1) - remove and process the oldest routine task
void RoutineQueue::dequeue() {
	if (front == NULL) {
		cout << "  [Q1] Queue is empty. Nothing to process." << endl;
		return;
	}
	cout << "  [Q1] Processing Task " << front->taskId << " -> Zone:" << front->zone << " Value:" << front->value << endl;
	TaskNode* temp = front;
	front = front->next;
	if (front == NULL) {
		rear = NULL;
	}
	delete temp;
	size = size - 1;
    cout << endl;
}

// O(n) - print all tasks currently in routine queue
void RoutineQueue::display() {
	if (front == NULL) {
		cout << "  [Q1] Routine queue is empty." << endl;
		return;
	}
	TaskNode* cur = front;
	cout << "  [Q1] Routine Queue:" << endl;
	while (cur != NULL) {
		cout << "    Task:" << cur->taskId << " Zone:" << cur->zone << " Value:" << cur->value << endl;
		cur = cur->next;
	}
    cout << endl;
}

// O(1) - return true if no tasks pending
bool RoutineQueue::isEmpty() {
	if (front == NULL) {
		return true;
	}
	return false;
}

// O(1) - return total number of pending tasks
int RoutineQueue::getSize() {
	return size;
}

// O(1) - look at front task without removing it
TaskNode* RoutineQueue::getFront() {
	return front;
}

// O(n) - delete every task in queue
void RoutineQueue::clear() {
	while (front != NULL) {
		TaskNode* temp = front;
		front = front->next;
		delete temp;
	}
	rear = NULL;
	size = 0;
}

// ============================
// SurveillanceQueue (Q2)
// ============================

SurveillanceQueue::SurveillanceQueue() {
	front = NULL;
	rear = NULL;
	size = 0;
}

// O(1) - add frequent sensor update from a sensitive zone
void SurveillanceQueue::enqueue(int id, int zone, float value) {
	TaskNode* newNode = new TaskNode(id, zone, value, 2);
	if (rear == NULL) {
		front = newNode;
		rear = newNode;
	}
	else {
		rear->next = newNode;
		rear = newNode;
	}
	size = size + 1;
	cout << "  [Q2] Surveillance Task " << id << " added -> Zone:" << zone << " Value:" << value << endl;
    cout << endl;
}

// O(1) - process and remove oldest surveillance update
void SurveillanceQueue::dequeue() {
	if (front == NULL) {
		cout << "  [Q2] Queue is empty. Nothing to process." << endl;
		return;
	}
	cout << "  [Q2] Processing Surveillance Task " << front->taskId << " -> Zone:" << front->zone << " Value:" << front->value << endl;
	TaskNode* temp = front;
	front = front->next;
	if (front == NULL) {
		rear = NULL;
	}
	delete temp;
	size = size - 1;
    cout << endl;
}

// O(n) - print all surveillance tasks in queue
void SurveillanceQueue::display() {
	if (front == NULL) {
		cout << "  [Q2] Surveillance queue is empty." << endl;
		return;
	}
	TaskNode* cur = front;
	cout << "  [Q2] Surveillance Queue:" << endl;
	while (cur != NULL) {
		cout << "    Task:" << cur->taskId << " Zone:" << cur->zone << " Value:" << cur->value << endl;
		cur = cur->next;
	}
    cout << endl;
}

// O(1) - check if surveillance queue is empty
bool SurveillanceQueue::isEmpty() {
	if (front == NULL) {
		return true;
	}
	return false;
}

// O(1) - get number of pending surveillance tasks
int SurveillanceQueue::getSize() {
	return size;
}

// O(n) - delete all surveillance tasks
void SurveillanceQueue::clear() {
	while (front != NULL) {
		TaskNode* temp = front;
		front = front->next;
		delete temp;
	}
	rear = NULL;
	size = 0;
}

// ============================
// EmergencyQueue (Q3) - Priority-Based
// ============================

EmergencyQueue::EmergencyQueue() {
	front = NULL;
	size = 0;
}

// O(n) - insert in sorted order so highest priority (lowest number) is at front
void EmergencyQueue::enqueue(int id, int zone, float value, int priority) {
	TaskNode* newNode = new TaskNode(id, zone, value, priority);
	if (front == NULL) {
		front = newNode;
	}
	else if (priority < front->priority) {
		newNode->next = front;
		front = newNode;
	}
	else {
		TaskNode* cur = front;
		while (cur->next != NULL) {
			if (priority < cur->next->priority) {
				break;
			}
			cur = cur->next;
		}
		newNode->next = cur->next;
		cur->next = newNode;
	}
	size = size + 1;
	cout << "  [Q3] Emergency Task " << id << " inserted -> Zone:" << zone << " Priority:" << priority << endl;
    cout << endl;
}

// O(1) - process highest priority emergency task first
void EmergencyQueue::dequeue() {
	if (front == NULL) {
		cout << "  [Q3] Emergency queue is empty." << endl;
		return;
	}
	cout << "  [Q3] Responding to Emergency Task " << front->taskId << " -> Zone:" << front->zone << " Priority:" << front->priority << " Value:" << front->value << endl;
	TaskNode* temp = front;
	front = front->next;
	delete temp;
	size = size - 1;
    cout << endl;
}

// O(n) - show all emergency tasks in priority order
void EmergencyQueue::display() {
	if (front == NULL) {
		cout << "  [Q3] Emergency queue is empty." << endl;
		return;
	}
	TaskNode* cur = front;
	cout << "  [Q3] Emergency Queue (by priority):" << endl;
	while (cur != NULL) {
		cout << "    Task:" << cur->taskId << " Zone:" << cur->zone << " Priority:" << cur->priority << " Value:" << cur->value << endl;
		cur = cur->next;
	}
    cout << endl;
}

// O(1) - check if no emergency tasks are pending
bool EmergencyQueue::isEmpty() {
	if (front == NULL) {
		return true;
	}
	return false;
}

// O(1) - return count of emergency tasks
int EmergencyQueue::getSize() {
	return size;
}

// O(1) - view top priority task without removing it
TaskNode* EmergencyQueue::getFront() {
	return front;
}

// O(n) - remove all emergency tasks
void EmergencyQueue::clear() {
	while (front != NULL) {
		TaskNode* temp = front;
		front = front->next;
		delete temp;
	}
	size = 0;
}

// ============================
// DecisionQueue (Q4)
// ============================

DecisionQueue::DecisionQueue() {
	front = NULL;
	rear = NULL;
	size = 0;
}

// O(1) - add a combined multi-factor task to decision queue
void DecisionQueue::enqueue(int id, int zone, float value, int priority) {
	TaskNode* newNode = new TaskNode(id, zone, value, priority);
	if (rear == NULL) {
		front = newNode;
		rear = newNode;
	}
	else {
		rear->next = newNode;
		rear = newNode;
	}
	size = size + 1;
	cout << "  [Q4] Decision Task " << id << " added -> Zone:" << zone << " Value:" << value << endl;
    cout << endl;
}

// O(1) - remove front decision task after processing
void DecisionQueue::dequeue() {
	if (front == NULL) {
		cout << "  [Q4] Decision queue is empty." << endl;
		return;
	}
	cout << "  [Q4] Executing Decision Task " << front->taskId << " -> Zone:" << front->zone << " Value:" << front->value << endl;
	TaskNode* temp = front;
	front = front->next;
	if (front == NULL) {
		rear = NULL;
	}
	delete temp;
	size = size - 1;
    cout << endl;
}

// O(n) - find task by id and mark it as paused
void DecisionQueue::pauseTask(int taskId) {
	TaskNode* cur = front;
	while (cur != NULL) {
		if (cur->taskId == taskId) {
			cur->paused = true;
			cout << "  [Q4] Task " << taskId << " paused." << endl;
            cout << endl;
			return;
		}
		cur = cur->next;
	}
	cout << "  [Q4] Task " << taskId << " not found." << endl;
    cout << endl;
}

// O(n) - find task by id and clear paused flag
void DecisionQueue::resumeTask(int taskId) {
	TaskNode* cur = front;
	while (cur != NULL) {
		if (cur->taskId == taskId) {
			cur->paused = false;
			cout << "  [Q4] Task " << taskId << " resumed." << endl;
            cout << endl;
			return;
		}
		cur = cur->next;
	}
	cout << "  [Q4] Task " << taskId << " not found." << endl;
    cout << endl;
}

// O(n) - execute all tasks that are not paused
void DecisionQueue::processAll() {
	if (front == NULL) {
		cout << "  [Q4] No tasks to process." << endl;
		return;
	}
	TaskNode* cur = front;
	while (cur != NULL) {
		if (cur->paused == false) {
			cout << "  [Q4] Processing Task " << cur->taskId << " -> Zone:" << cur->zone << " Value:" << cur->value << endl;
		}
		else {
			cout << "  [Q4] Task " << cur->taskId << " is paused, skipping." << endl;
		}
		cur = cur->next;
	}
    cout << endl;
}

// O(n) - show all tasks with their current paused status
void DecisionQueue::display() {
	if (front == NULL) {
		cout << "  [Q4] Decision queue is empty." << endl;
		return;
	}
	TaskNode* cur = front;
	cout << "  [Q4] Decision Queue:" << endl;
	while (cur != NULL) {
		cout << "    Task:" << cur->taskId << " Zone:" << cur->zone << " Value:" << cur->value;
		if (cur->paused == true) {
			cout << " [PAUSED]";
		}
		else {
			cout << " [ACTIVE]";
		}
		cout << endl;
		cur = cur->next;
	}
    cout << endl;
}

// O(1) - return true if no decision tasks pending
bool DecisionQueue::isEmpty() {
	if (front == NULL) {
		return true;
	}
	return false;
}

// O(n) - remove every task in decision queue
void DecisionQueue::clear() {
	while (front != NULL) {
		TaskNode* temp = front;
		front = front->next;
		delete temp;
	}
	rear = NULL;
	size = 0;
}