// arrays.h
// Array-Based Environmental Processing Layer
// Covers A1 (baseline), A2 (sensor stream), A3 (forest grid), A4 (terrain matrix)

#pragma once

using namespace std;

class Baseline {
private:
	float temp[10];
	float humidity[10];
	float smoke[10];
	int   count;

public:
	Baseline();
	void  set(int zone, float t, float h, float s);   // set baseline values for one zone
	float getTemp(int zone);                           // get normal temperature for zone
	float getHumidity(int zone);                       // get normal humidity for zone
	float getSmoke(int zone);                          // get normal smoke for zone
	int   getCount();                                  // return total number of zones
	void  show();                                      // print all baseline values
};

class SensorStream {
private:
	float temp[50];
	float smoke[50];
	float humid[50];
	int   total;

public:
	SensorStream();
	void  add(float t, float s, float h);   // O(1) - add a new sensor reading
	float getTemp(int i);                   // O(1) - get temperature at index i
	float getSmoke(int i);                  // O(1) - get smoke at index i
	float getHumid(int i);                  // O(1) - get humidity at index i
	int   getTotal();                       // return count of stored readings
	void  show();                           // print all stored readings
};

class ForestGrid {
private:
	float temp[5][5];
	float smoke[5][5];
	int   rows;
	int   cols;

public:
	ForestGrid();
	void  setCell(int r, int c, float t, float s);   // O(1) - set temperature and smoke at cell
	float getCellTemp(int r, int c);                  // O(1) - get temperature at cell
	float getCellSmoke(int r, int c);                 // O(1) - get smoke at cell
	void  interpolate(int r, int c);                  // O(1) - estimate missing cell value from neighbors
	void  findBoundaries();                           // O(n*m) - detect zones with sharp temperature change
	void  show();                                     // print temperature and smoke grids
};

class Terrain {
private:
	float risk[5][5];
	float humidity[5][5];
	float slope[5][5];
	int   rows;
	int   cols;

public:
	Terrain();
	void  update(int r, int c, float rk, float h, float sl);   // O(1) - update terrain cell at runtime
	float getRisk(int r, int c);                                // O(1) - get fire risk at cell
	void  filterAnomalies();                                    // O(n*m) - flag cells beyond safe limits
	void  show();                                               // print risk and humidity grids
};