#pragma once
#include <iostream>
using namespace std;

// H1: Primary Index Table — direct key-value store for zone sensor data
// H2: Collision Handling Table — chained collision resolution
// H3: Fast Retrieval Cache — stores frequently accessed zone data

// Entry used in H1 and H2 chaining
class HashEntry {
public:
	int key;
	double temperature;
	double smoke;
	double humidity;
	bool occupied;
	HashEntry* next;   // for chaining in H2

	HashEntry();
};

// CacheEntry used in H3
class CacheEntry {
public:
	int key;
	double temperature;
	double smoke;
	double humidity;
	bool valid;
	int accessCount;

	CacheEntry();
};

// H1: Primary Index Table — stores zone data at index = key mod tableSize
class PrimaryIndexTable {
private:
	int tableSize;
	HashEntry* table;

	// O(1) — computes hash index from key
	int hashFunction(int key);

public:
	PrimaryIndexTable(int size);
	~PrimaryIndexTable();

	// O(1) average — inserts zone sensor data using hash key
	void insert(int key, double temp, double smk, double humid);

	// O(1) average — retrieves sensor data for a zone key
	bool retrieve(int key, double& temp, double& smk, double& humid);

	// O(1) average — updates existing entry data
	void update(int key, double temp, double smk, double humid);

	// O(n) — displays all occupied slots in the table
	void show();
};

// H2: Collision Handling Table — uses separate chaining at each slot
class CollisionTable {
private:
	int tableSize;
	HashEntry** buckets;   // array of linked lists

	// O(1) — computes hash index
	int hashFunction(int key);

public:
	CollisionTable(int size);
	~CollisionTable();

	// O(1) average, O(n) worst — inserts via chaining on collision
	void insert(int key, double temp, double smk, double humid);

	// O(n) worst — retrieves by traversing chain at index
	bool retrieve(int key, double& temp, double& smk, double& humid);

	// O(n) worst — updates entry in chain
	void update(int key, double temp, double smk, double humid);

	// O(n) — displays all buckets and their chains
	void show();
};

// H3: Fast Retrieval Cache — fixed-size direct cache for quick access
class FastCache {
private:
	int cacheSize;
	CacheEntry* cache;

	// O(1) — maps key to cache slot
	int cacheIndex(int key);

public:
	FastCache(int size);
	~FastCache();

	// O(1) — stores data in cache slot
	void store(int key, double temp, double smk, double humid);

	// O(1) — retrieves data from cache, increments access count
	bool fetch(int key, double& temp, double& smk, double& humid);

	// O(1) — updates cache entry if it exists
	void update(int key, double temp, double smk, double humid);

	// O(1) — invalidates a cache slot
	void evict(int key);

	// O(n) — displays all valid cache entries with access counts
	void show();
};