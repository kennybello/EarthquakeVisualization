#include <fstream>
#include <algorithm>

#include "EarthquakeDatabase.h"

using namespace std;

EarthquakeDatabase::EarthquakeDatabase(G3D::String filename) {
  ifstream in(filename.c_str());
  string line;
  while (getline(in, line)) {
    if (line.size() > 30) {
      earthquakes.push_back(Earthquake(line));
    }
  }
}

Earthquake EarthquakeDatabase::getByIndex(int index) {
  return earthquakes[index];
}

int EarthquakeDatabase::getMinIndex() {
  return 250;
}

int EarthquakeDatabase::getMaxIndex() {
  return earthquakes.size() - 1;
}

int EarthquakeDatabase::getIndexByDate(Date d) {
  double targetSeconds = d.asSeconds();
  int start = getMinIndex();
  int end = getMaxIndex();
  while (start < end-1) {
    int half = (start + end) / 2;
    if (earthquakes[half].getDate().asSeconds() > targetSeconds) {
      end = half - 1;
    } else {
      start = half;
    }
  }
  if (start == end)
    return start;
  else {
    if (earthquakes[end].getDate().asSeconds() > targetSeconds) {
      return start;
    } else {
      return end;
    }
  }
}