#pragma once

#include <iostream>
using namespace std;

// Task node used in all queues
class TaskNode {
public:
	int taskId;
	int zone;
	float value;
	int priority;       // lower number = higher priority
	bool paused;
	TaskNode* next;

	TaskNode(int id, int z, float v, int p);
};

// Q1: Routine Monitoring Queue - normal FIFO sensor updates
class RoutineQueue {
private:
	TaskNode* front;
	TaskNode* rear;
	int size;

public:
	RoutineQueue();

	void enqueue(int id, int zone, float value);     // O(1) - add task at rear
	void dequeue();                                   // O(1) - remove task from front
	void display();                                   // O(n) - print all tasks in order
	bool isEmpty();                                   // O(1) - check if queue empty
	int getSize();                                    // O(1) - return task count
	TaskNode* getFront();                             // O(1) - peek at front task
	void clear();                                     // O(n) - delete all tasks
};

// Q2: Surveillance Queue - frequent updates from sensitive zones
class SurveillanceQueue {
private:
	TaskNode* front;
	TaskNode* rear;
	int size;

public:
	SurveillanceQueue();

	void enqueue(int id, int zone, float value);     // O(1) - add surveillance task
	void dequeue();                                   // O(1) - process and remove front
	void display();                                   // O(n) - show all pending tasks
	bool isEmpty();                                   // O(1) - check empty state
	int getSize();                                    // O(1) - return pending count
	void clear();                                     // O(n) - flush all tasks
};

// Q3: Emergency Response Queue - urgent fire/smoke tasks, priority-based
class EmergencyQueue {
private:
	TaskNode* front;
	int size;

public:
	EmergencyQueue();

	void enqueue(int id, int zone, float value, int priority);  // O(n) - insert by priority
	void dequeue();                                              // O(1) - remove top priority
	void display();                                              // O(n) - show sorted queue
	bool isEmpty();                                              // O(1) - check empty
	int getSize();                                               // O(1) - count of tasks
	TaskNode* getFront();                                        // O(1) - peek at top task
	void clear();                                                // O(n) - remove all
};

// Q4: Multi-Factor Decision Queue - combined sensor tasks before final decision
class DecisionQueue {
private:
	TaskNode* front;
	TaskNode* rear;
	int size;

public:
	DecisionQueue();

	void enqueue(int id, int zone, float value, int priority);   // O(1) - add combined task
	void dequeue();                                               // O(1) - process and remove
	void pauseTask(int taskId);                                   // O(n) - mark task as paused
	void resumeTask(int taskId);                                  // O(n) - unmark paused task
	void processAll();                                            // O(n) - run all non-paused
	void display();                                               // O(n) - print queue state
	bool isEmpty();                                               // O(1) - check empty
	void clear();                                                 // O(n) - delete all tasks
};