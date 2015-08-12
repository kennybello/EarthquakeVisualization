#include <sstream>
#include "Earthquake.h"

using namespace std;

Earthquake::Earthquake() {
  
}

Earthquake::Earthquake(std::string s) {
  line = s;
}

Date Earthquake::getDate() {
  int year = parseInt(line.substr(12,4));
  int month = parseInt(line.substr(17,2));
  int day = parseInt(line.substr(20,2));
  int hour = parseInt(line.substr(24,2));
  int minute = parseInt(line.substr(27,2));
  double second = parseFloat(line.substr(30,5));
  
  return Date(month, day, year, hour, minute, second);
}

double Earthquake::getLongitude() {
  return parseFloat(line.substr(44,8));
  //return parseFloat(line.substr(37,7));
}

double Earthquake::getLatitude() {
  //return parseFloat(line.substr(44,8));
  return parseFloat(line.substr(37,7));
}

double Earthquake::getMagnitude() {
  return parseFloat(line.substr(66,4));
}

double Earthquake::parseFloat(std::string s) {
  stringstream ss(s);
  double f;
  ss >> f;
  return f;
}

int Earthquake::parseInt(std::string s) {
  stringstream ss(s);
  int i;
  ss >> i;
  return i;
}