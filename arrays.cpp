// arrays.cpp
// Implementation of Array-Based Environmental Processing Layer

#include <iostream>
#include "arrays.h"

using namespace std;

// Baseline - A1: Static Environmental Baseline Array

// Initialize all zones to safe default values
// Time Complexity: O(n)
Baseline::Baseline() {
	count = 10;
	for (int i = 0; i < 10; i++) {
		temp[i] = 25.0;
		humidity[i] = 60.0;
		smoke[i] = 0.0;
	}
}

// Set baseline values for a specific zone
// Time Complexity: O(1)
void Baseline::set(int zone, float t, float h, float s) {
	if (zone >= 0 && zone < 10) {
		temp[zone] = t;
		humidity[zone] = h;
		smoke[zone] = s;
	}
	else {
		cout << "Invalid zone." << endl;
	}
}

// Return normal temperature for a zone
// Time Complexity: O(1)
float Baseline::getTemp(int zone) {
	if (zone >= 0 && zone < 10) {
		return temp[zone];
	}
	return -1;
}

// Return normal humidity for a zone
// Time Complexity: O(1)
float Baseline::getHumidity(int zone) {
	if (zone >= 0 && zone < 10) {
		return humidity[zone];
	}
	return -1;
}

// Return normal smoke level for a zone
// Time Complexity: O(1)
float Baseline::getSmoke(int zone) {
	if (zone >= 0 && zone < 10) {
		return smoke[zone];
	}
	return -1;
}

// Return total zone count
// Time Complexity: O(1)
int Baseline::getCount() {
	return count;
}

// Print all baseline values for all zones
// Time Complexity: O(n)
void Baseline::show() {
	cout << "Zone  Temp(C)  Humidity(%)  Smoke" << endl;
	for (int i = 0; i < 10; i++) {
		cout << i << "     " << temp[i] << "    " << humidity[i] << "        " << smoke[i] << endl;
	}
	cout << endl;
}

// SensorStream - A2: Dynamic Sensor Stream Array (1D)

// Initialize empty stream
// Time Complexity: O(n)
SensorStream::SensorStream() {
	total = 0;
	for (int i = 0; i < 50; i++) {
		temp[i] = 0;
		smoke[i] = 0;
		humid[i] = 0;
	}
}

// Add a new sensor reading to the stream
// Time Complexity: O(1)
void SensorStream::add(float t, float s, float h) {
	if (total < 50) {
		temp[total] = t;
		smoke[total] = s;
		humid[total] = h;
		total = total + 1;
		cout << "Reading added. Total: " << total << endl;
		cout << endl;
	}
	else {
		cout << "Stream full. Cannot add more readings." << endl;
	}
}

// Get temperature at index i
// Time Complexity: O(1)
float SensorStream::getTemp(int i) {
	if (i >= 0 && i < total) {
		return temp[i];
	}
	return -1;
}

// Get smoke at index i
// Time Complexity: O(1)
float SensorStream::getSmoke(int i) {
	if (i >= 0 && i < total) {
		return smoke[i];
	}
	return -1;
}

// Get humidity at index i
// Time Complexity: O(1)
float SensorStream::getHumid(int i) {
	if (i >= 0 && i < total) {
		return humid[i];
	}
	return -1;
}

// Return total number of readings stored
// Time Complexity: O(1)
int SensorStream::getTotal() {
	return total;
}

// Print all stored readings in order
// Time Complexity: O(n)
void SensorStream::show() {
	if (total == 0) {
		cout << "No sensor readings stored." << endl;
		return;
	}
	cout << "Index  Temp(C)  Smoke  Humidity(%)" << endl;
	for (int i = 0; i < total; i++) {
		cout << i << "      " << temp[i] << "     " << smoke[i] << "    " << humid[i] << endl;
	}
	cout << endl;
}

// ForestGrid - A3: Static Forest Grid Matrix (2D)

// Initialize entire grid to zero
// Time Complexity: O(n*m)
ForestGrid::ForestGrid() {
	rows = 5;
	cols = 5;
	for (int r = 0; r < 5; r++) {
		for (int c = 0; c < 5; c++) {
			temp[r][c] = 0;
			smoke[r][c] = 0;
		}
	}
}

// Set temperature and smoke at a grid cell
// Time Complexity: O(1)
void ForestGrid::setCell(int r, int c, float t, float s) {
	if (r >= 0 && r < rows && c >= 0 && c < cols) {
		temp[r][c] = t;
		smoke[r][c] = s;
	}
	else {
		cout << "Invalid cell position." << endl;
	}
}

// Get temperature at a grid cell
// Time Complexity: O(1)
float ForestGrid::getCellTemp(int r, int c) {
	if (r >= 0 && r < rows && c >= 0 && c < cols) {
		return temp[r][c];
	}
	return -1;
}

// Get smoke at a grid cell
// Time Complexity: O(1)
float ForestGrid::getCellSmoke(int r, int c) {
	if (r >= 0 && r < rows && c >= 0 && c < cols) {
		return smoke[r][c];
	}
	return -1;
}

// Estimate missing temperature at (r,c) using average of valid neighbors
// Time Complexity: O(1)
void ForestGrid::interpolate(int r, int c) {
	if (r < 0 || r >= rows || c < 0 || c >= cols) {
		cout << "Invalid cell for interpolation." << endl;
		return;
	}

	float sum = 0;
	int   count = 0;

	if (r - 1 >= 0) {
		sum = sum + temp[r - 1][c];
		count = count + 1;
	}
	if (r + 1 < rows) {
		sum = sum + temp[r + 1][c];
		count = count + 1;
	}
	if (c - 1 >= 0) {
		sum = sum + temp[r][c - 1];
		count = count + 1;
	}
	if (c + 1 < cols) {
		sum = sum + temp[r][c + 1];
		count = count + 1;
	}

	if (count > 0) {
		temp[r][c] = sum / count;
		cout << "Interpolated value at (" << r << "," << c << "): " << temp[r][c] << " C" << endl;
		cout << endl;
	}
	else {
		cout << "No valid neighbors for interpolation." << endl;
		cout << endl;
	}
}

// Detect cells where temperature changes sharply with a neighbor
// Time Complexity: O(n*m)
void ForestGrid::findBoundaries() {
	float limit = 15.0;
	bool  found = false;

	cout << "Boundary Detection Results:" << endl;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			float curr = temp[r][c];

			if (c + 1 < cols) {
				float diff = curr - temp[r][c + 1];
				if (diff < 0) {
					diff = diff * -1;
				}
				if (diff > limit) {
					cout << "Boundary at (" << r << "," << c << ") and (" << r << "," << c + 1 << ") diff: " << diff << " C" << endl;
					found = true;
				}
			}

			if (r + 1 < rows) {
				float diff = curr - temp[r + 1][c];
				if (diff < 0) {
					diff = diff * -1;
				}
				if (diff > limit) {
					cout << "Boundary at (" << r << "," << c << ") and (" << r + 1 << "," << c << ") diff: " << diff << " C" << endl;
					found = true;
				}
			}
		}
	}

	if (found == false) {
		cout << "No sharp boundaries found." << endl;
		cout << endl;
	}
}

// Print temperature and smoke grids
// Time Complexity: O(n*m)
void ForestGrid::show() {
	cout << "Temperature Grid (C):" << endl;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			cout << temp[r][c] << "\t";
		}
		cout << endl;
	}

	cout << "Smoke Grid:" << endl;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			cout << smoke[r][c] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

// Terrain - A4: Dynamic Terrain Expansion Matrix (2D)

// Initialize all terrain values to zero
// Time Complexity: O(n*m)
Terrain::Terrain() {
	rows = 5;
	cols = 5;
	for (int r = 0; r < 5; r++) {
		for (int c = 0; c < 5; c++) {
			risk[r][c] = 0;
			humidity[r][c] = 0;
			slope[r][c] = 0;
		}
	}
}

// Update terrain cell with new values during runtime
// Time Complexity: O(1)
void Terrain::update(int r, int c, float rk, float h, float sl) {
	if (r >= 0 && r < rows && c >= 0 && c < cols) {
		risk[r][c] = rk;
		humidity[r][c] = h;
		slope[r][c] = sl;
		cout << "Terrain cell (" << r << "," << c << ") updated." << endl;
		cout << endl;
	}
	else {
		cout << "Invalid terrain cell." << endl;
	}
}

// Return fire risk at a specific cell
// Time Complexity: O(1)
float Terrain::getRisk(int r, int c) {
	if (r >= 0 && r < rows && c >= 0 && c < cols) {
		return risk[r][c];
	}
	return -1;
}

// Scan terrain and flag cells that exceed safe thresholds
// Fire risk > 0.7, humidity < 20, slope > 0.8
// Time Complexity: O(n*m)
void Terrain::filterAnomalies() {
	bool found = false;
	cout << "Terrain Anomaly Scan:" << endl;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			bool bad = false;

			if (risk[r][c] > 0.7) {
				cout << "Cell (" << r << "," << c << ") HIGH FIRE RISK: " << risk[r][c] << endl;
				bad = true;
			}
			if (humidity[r][c] < 20.0) {
				cout << "Cell (" << r << "," << c << ") DRY CONDITION: " << humidity[r][c] << "%" << endl;
				bad = true;
			}
			if (slope[r][c] > 0.8) {
				cout << "Cell (" << r << "," << c << ") STEEP SLOPE: " << slope[r][c] << endl;
				bad = true;
			}

			if (bad == true) {
				found = true;
			}
		}
	}

	if (found == false) {
		cout << "No anomalies detected in terrain." << endl;
		cout << endl;
	}
}

// Print risk and humidity grids
// Time Complexity: O(n*m)
void Terrain::show() {
	cout << "Fire Risk Grid:" << endl;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			cout << risk[r][c] << "\t";
		}
		cout << endl;
	}

	cout << "Humidity Grid (%):" << endl;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			cout << humidity[r][c] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

// O(n) - check each reading against caller-supplied thresholds
// FIX: previously used hard-coded 45.0, 70.0, 20.0 — parameters were
//      received but never used. Now uses baseTemp, baseSmoke, baseHumid.
void SensorStream::checkAnomalies(float baseTemp, float baseSmoke, float baseHumid) {
	if (total == 0) {
		cout << "No sensor readings to check." << endl;
		return;
	}
	bool found = false;
	cout << "Anomaly Check Results:" << endl;
	for (int i = 0; i < total; i++) {
		if (temp[i] > baseTemp) {
			cout << "Index " << i << " FIRE RISK - Temp: " << temp[i] << " C" << endl;
			found = true;
		}
		if (smoke[i] > baseSmoke) {
			cout << "Index " << i << " SMOKE ALERT - Smoke: " << smoke[i] << endl;
			found = true;
		}
		if (humid[i] < baseHumid) {
			cout << "Index " << i << " DRY CONDITION - Humidity: " << humid[i] << "%" << endl;
			found = true;
		}
	}
	if (found == false) {
		cout << "All readings are within safe limits." << endl;
		cout << endl;
	}
}