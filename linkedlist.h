#pragma once

#include <iostream>
using namespace std;

// Node class for singly and doubly linked lists
class Node {
public:
	float value;
	int time;
	int zone;
	Node* next;
	Node* prev;

	Node(float v, int t, int z);
};

// L1, L2, L3: Singly Linked List - raw, verified, anomaly event streams
class SinglyList {
private:
	Node* head;
	int size;

public:
	SinglyList();

	void insert(float value, int time, int zone);       // O(n) - add event at end
	void display();                                      // O(n) - print all events forward
	bool isEmpty();                                      // O(1) - check if list is empty
	Node* getHead();                                     // O(1) - return head pointer
	int getSize();                                       // O(1) - return current count
	void clear();                                        // O(n) - free all nodes
	float getLastValue();                                // O(n) - return last stored value
	bool removeNoise(float delta);                       // O(n) - filter noisy readings
	void filterAnomalies(float normal, float theta);     // O(n) - flag anomalous values
};

// L4, L5, L6: Doubly Linked List - correction and sync chains
class DoublyList {
private:
	Node* head;
	Node* tail;
	int size;

public:
	DoublyList();

	void insertFront(float value, int time, int zone);   // O(1) - add at front
	void insertBack(float value, int time, int zone);    // O(1) - add at back
	void displayForward();                               // O(n) - traverse head to tail
	void displayBackward();                              // O(n) - traverse tail to head
	void correctForward(int fromTime, float newVal);     // O(n) - fix events after a point
	void correctBackward(int fromTime, float newVal);    // O(n) - fix events before a point
	void synchronize(float globalVal);                   // O(n) - set all nodes to one value
	Node* getTail();                                     // O(1) - return tail pointer
	bool isEmpty();                                      // O(1) - check empty
	void clear();                                        // O(n) - remove all nodes
};

// L7-L10: Circular Linked List - monitoring loops
class CircularList {
private:
	Node* head;
	int size;

public:
	CircularList();

	void insert(float value, int time, int zone);        // O(n) - insert and keep loop
	void monitor(int rounds);                            // O(n*rounds) - scan in loop
	void monitorEmergency(float threshold, int rounds);  // O(n*rounds) - scan for danger
	void monitorStability(float epsilon, int rounds);    // O(n*rounds) - check stable change
	void display();                                      // O(n) - show all nodes once
	bool isEmpty();                                      // O(1) - check if empty
	void clear();                                        // O(n) - unlink all nodes
	int getSize();                                       // O(1) - return count
};