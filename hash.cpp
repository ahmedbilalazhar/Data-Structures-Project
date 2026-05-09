#include "hash.h"

// ============================================================
// HashEntry
// ============================================================

HashEntry::HashEntry() {
	key = -1;
	temperature = 0.0;
	smoke = 0.0;
	humidity = 0.0;
	occupied = false;
	next = NULL;
}

// ============================================================
// CacheEntry
// ============================================================

CacheEntry::CacheEntry() {
	key = -1;
	temperature = 0.0;
	smoke = 0.0;
	humidity = 0.0;
	valid = false;
	accessCount = 0;
}

// ============================================================
// PrimaryIndexTable — H1
// ============================================================

PrimaryIndexTable::PrimaryIndexTable(int size) {
	tableSize = size;
	table = new HashEntry[tableSize];
}

PrimaryIndexTable::~PrimaryIndexTable() {
	delete[] table;
}

// O(1) — index = key mod tableSize
int PrimaryIndexTable::hashFunction(int key) {
	int index = key % tableSize;
	if (index < 0) {
		index = index + tableSize;
	}
	return index;
}

// O(1) average — inserts using open addressing (linear probe on collision)
void PrimaryIndexTable::insert(int key, double temp, double smk, double humid) {
	int index = hashFunction(key);
	int original = index;
	int count = 0;

	while (table[index].occupied == true && table[index].key != key) {
		index = (index + 1) % tableSize;
		count = count + 1;
		if (count == tableSize) {
			cout << "Primary table is full. Cannot insert key " << key << endl;
			return;
		}
	}

	table[index].key = key;
	table[index].temperature = temp;
	table[index].smoke = smk;
	table[index].humidity = humid;
	table[index].occupied = true;

	cout << "H1 Insert: Zone " << key
		<< " stored at index " << index
		<< " (temp=" << temp << ", smoke=" << smk << ", humid=" << humid << ")" << endl;

	if (index != original) {
		cout << "  Note: linear probe used, original index was " << original << endl;
	}
}

// O(1) average — retrieves using linear probe
bool PrimaryIndexTable::retrieve(int key, double& temp, double& smk, double& humid) {
	int index = hashFunction(key);
	int count = 0;

	while (table[index].occupied == true) {
		if (table[index].key == key) {
			temp = table[index].temperature;
			smk = table[index].smoke;
			humid = table[index].humidity;
			cout << "H1 Retrieve: Zone " << key
				<< " found at index " << index
				<< " (temp=" << temp << ", smoke=" << smk << ", humid=" << humid << ")" << endl;
			return true;
		}
		index = (index + 1) % tableSize;
		count = count + 1;
		if (count == tableSize) {
			break;
		}
	}

	cout << "H1 Retrieve: Zone " << key << " not found." << endl;
	return false;
}

// O(1) average — updates entry using linear probe search
void PrimaryIndexTable::update(int key, double temp, double smk, double humid) {
	int index = hashFunction(key);
	int count = 0;

	while (table[index].occupied == true) {
		if (table[index].key == key) {
			table[index].temperature = temp;
			table[index].smoke = smk;
			table[index].humidity = humid;
			cout << "H1 Update: Zone " << key
				<< " at index " << index << " updated." << endl;
			return;
		}
		index = (index + 1) % tableSize;
		count = count + 1;
		if (count == tableSize) {
			break;
		}
	}

	cout << "H1 Update: Zone " << key << " not found. Use insert instead." << endl;
}

// O(n) — prints all occupied slots
void PrimaryIndexTable::show() {
	cout << "Primary Index Table (H1):" << endl;
	cout << "Index  | Key  | Temp   | Smoke  | Humid  | Status" << endl;
	for (int i = 0; i < tableSize; i++) {
		if (table[i].occupied == true) {
			cout << "  " << i
				<< "    | " << table[i].key
				<< "   | " << table[i].temperature
				<< "  | " << table[i].smoke
				<< "  | " << table[i].humidity
				<< "  | occupied" << endl;
		}
		else {
			cout << "  " << i
				<< "    |  --  |  --    |  --    |  --    | empty" << endl;
		}
	}
}

// ============================================================
// CollisionTable — H2
// ============================================================

CollisionTable::CollisionTable(int size) {
	tableSize = size;
	buckets = new HashEntry * [tableSize];
	for (int i = 0; i < tableSize; i++) {
		buckets[i] = NULL;
	}
}

CollisionTable::~CollisionTable() {
	for (int i = 0; i < tableSize; i++) {
		HashEntry* cur = buckets[i];
		while (cur != NULL) {
			HashEntry* temp = cur;
			cur = cur->next;
			delete temp;
		}
	}
	delete[] buckets;
}

// O(1) — index = key mod tableSize
int CollisionTable::hashFunction(int key) {
	int index = key % tableSize;
	if (index < 0) {
		index = index + tableSize;
	}
	return index;
}

// O(1) average, O(n) worst — inserts at front of chain at computed index
void CollisionTable::insert(int key, double temp, double smk, double humid) {
	int index = hashFunction(key);

	HashEntry* cur = buckets[index];
	while (cur != NULL) {
		if (cur->key == key) {
			cur->temperature = temp;
			cur->smoke = smk;
			cur->humidity = humid;
			cout << "H2 Insert: Zone " << key
				<< " updated in chain at bucket " << index << endl;
			return;
		}
		cur = cur->next;
	}

	HashEntry* entry = new HashEntry();
	entry->key = key;
	entry->temperature = temp;
	entry->smoke = smk;
	entry->humidity = humid;
	entry->occupied = true;
	entry->next = buckets[index];
	buckets[index] = entry;

	cout << "H2 Insert: Zone " << key
		<< " stored at bucket " << index;
	if (entry->next != NULL) {
		cout << " (collision handled via chaining)";
	}
	cout << endl;
}

// O(n) worst — traverses chain at index to find key
bool CollisionTable::retrieve(int key, double& temp, double& smk, double& humid) {
	int index = hashFunction(key);
	HashEntry* cur = buckets[index];

	while (cur != NULL) {
		if (cur->key == key) {
			temp = cur->temperature;
			smk = cur->smoke;
			humid = cur->humidity;
			cout << "H2 Retrieve: Zone " << key
				<< " found at bucket " << index
				<< " (temp=" << temp << ", smoke=" << smk << ", humid=" << humid << ")" << endl;
			return true;
		}
		cur = cur->next;
	}

	cout << "H2 Retrieve: Zone " << key << " not found in bucket " << index << endl;
	return false;
}

// O(n) worst — finds and updates entry in chain
void CollisionTable::update(int key, double temp, double smk, double humid) {
	int index = hashFunction(key);
	HashEntry* cur = buckets[index];

	while (cur != NULL) {
		if (cur->key == key) {
			cur->temperature = temp;
			cur->smoke = smk;
			cur->humidity = humid;
			cout << "H2 Update: Zone " << key
				<< " updated at bucket " << index << endl;
			return;
		}
		cur = cur->next;
	}

	cout << "H2 Update: Zone " << key << " not found." << endl;
}

// O(n) — prints all buckets and their chained entries
void CollisionTable::show() {
	cout << "Collision Handling Table (H2) — Separate Chaining:" << endl;
	for (int i = 0; i < tableSize; i++) {
		cout << "  Bucket " << i << ": ";
		if (buckets[i] == NULL) {
			cout << "[empty]" << endl;
		}
		else {
			HashEntry* cur = buckets[i];
			while (cur != NULL) {
				cout << "[Zone " << cur->key
					<< " | T=" << cur->temperature
					<< " S=" << cur->smoke
					<< " H=" << cur->humidity << "]";
				if (cur->next != NULL) {
					cout << " -> ";
				}
				cur = cur->next;
			}
			cout << endl;
		}
	}
}

// ============================================================
// FastCache — H3
// ============================================================

FastCache::FastCache(int size) {
	cacheSize = size;
	cache = new CacheEntry[cacheSize];
}

FastCache::~FastCache() {
	delete[] cache;
}

// O(1) — slot = key mod cacheSize
int FastCache::cacheIndex(int key) {
	int index = key % cacheSize;
	if (index < 0) {
		index = index + cacheSize;
	}
	return index;
}

// O(1) — stores zone data in cache slot (overwrites if slot taken)
void FastCache::store(int key, double temp, double smk, double humid) {
	int index = cacheIndex(key);

	if (cache[index].valid == true && cache[index].key != key) {
		cout << "H3 Cache: evicting Zone " << cache[index].key
			<< " to make room for Zone " << key
			<< " at slot " << index << endl;
	}

	cache[index].key = key;
	cache[index].temperature = temp;
	cache[index].smoke = smk;
	cache[index].humidity = humid;
	cache[index].valid = true;
	cache[index].accessCount = 0;

	cout << "H3 Cache Store: Zone " << key
		<< " cached at slot " << index
		<< " (temp=" << temp << ", smoke=" << smk << ", humid=" << humid << ")" << endl;
}

// O(1) — fetches data from cache, increments access count
bool FastCache::fetch(int key, double& temp, double& smk, double& humid) {
	int index = cacheIndex(key);

	if (cache[index].valid == true && cache[index].key == key) {
		temp = cache[index].temperature;
		smk = cache[index].smoke;
		humid = cache[index].humidity;
		cache[index].accessCount = cache[index].accessCount + 1;

		cout << "H3 Cache Hit: Zone " << key
			<< " retrieved from slot " << index
			<< " (access count: " << cache[index].accessCount << ")" << endl;
		return true;
	}

	cout << "H3 Cache Miss: Zone " << key << " not in cache." << endl;
	return false;
}

// O(1) — updates cache slot if key matches
void FastCache::update(int key, double temp, double smk, double humid) {
	int index = cacheIndex(key);

	if (cache[index].valid == true && cache[index].key == key) {
		cache[index].temperature = temp;
		cache[index].smoke = smk;
		cache[index].humidity = humid;
		cout << "H3 Cache Update: Zone " << key
			<< " updated at slot " << index << endl;
	}
	else {
		cout << "H3 Cache Update: Zone " << key
			<< " not in cache. Use store first." << endl;
	}
}

// O(1) — marks a cache slot as invalid
void FastCache::evict(int key) {
	int index = cacheIndex(key);

	if (cache[index].valid == true && cache[index].key == key) {
		cache[index].valid = false;
		cout << "H3 Cache Evict: Zone " << key
			<< " removed from slot " << index << endl;
	}
	else {
		cout << "H3 Cache Evict: Zone " << key << " not in cache." << endl;
	}
}

// O(n) — prints all valid cache entries
void FastCache::show() {
	cout << "Fast Retrieval Cache (H3):" << endl;
	cout << "Slot | Key  | Temp   | Smoke  | Humid  | Accesses" << endl;
	for (int i = 0; i < cacheSize; i++) {
		if (cache[i].valid == true) {
			cout << "  " << i
				<< "  | " << cache[i].key
				<< "   | " << cache[i].temperature
				<< "  | " << cache[i].smoke
				<< "  | " << cache[i].humidity
				<< "  | " << cache[i].accessCount << endl;
		}
		else {
			cout << "  " << i
				<< "  |  --  |  --    |  --    |  --    |  --" << endl;
		}
	}
}