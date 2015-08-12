#ifndef Date_h
#define Date_h

#include <ctime>
#include <iostream>

class Date {
public:
  Date();
  Date(double seconds);
  Date(int m, int d, int y);
  Date(int m, int d, int y, int hr, int min, double sec);
  bool operator<(const Date& other);
  bool operator>(const Date& other);
  
  double asSeconds() const;
  
  double secondsUntil(const Date& other) const;
  int getYear();
  int getMonth();
  int getDay();
  int getHour();
  int getMinute();
  double getSecond();
protected:
  std::time_t ctime;
  int fouryears;
  int year;
  int month;
  int day;
  int hour;
  int minute;
  double second;
};

#endif
