#include <ctime>
#include "Date.h"

using namespace std;

const int SECONDS_PER_4_YEARS=126230400;

Date::Date() {
  year = 0;
  month = 0;
  day = 0;
  hour = 0;
  minute = 0;
  second = 0;
  ctime = 0;
  fouryears = 0;
}
Date::Date(double seconds) {
  double tweakedSeconds = seconds;
  fouryears = 0;
  while (tweakedSeconds < 0) {
    tweakedSeconds += SECONDS_PER_4_YEARS;
    fouryears++;
  }
  ctime = int(tweakedSeconds);
  tm *t;
  t = localtime(&ctime);
  year = t->tm_year + 1900 - 4 * fouryears;
  month = t->tm_mon + 1;
  day = t->tm_mday;
  hour = t->tm_hour;
  minute = t->tm_min;
  second = t->tm_sec + (tweakedSeconds - ctime);
}

Date::Date(int m, int d, int y) {
  month = m;
  day = d;
  year = y;
  hour = 0;
  minute = 0;
  second = 0;
  fouryears = 0;
  tm t;
  while (year + fouryears*4 < 1970) {
    fouryears++;
  }
  t.tm_year = year - 1900 + fouryears * 4;
  t.tm_mon = month - 1;
  t.tm_mday = day;
  t.tm_hour = 0;
  t.tm_min = 0;
  t.tm_sec = 0;
  ctime = mktime(&t);
}

Date::Date(int m, int d, int y, int hr, int min, double sec) {
  month = m;
  day = d;
  year = y;
  hour = hr;
  minute = min;
  second = sec;
  fouryears = 0;
  tm t;
  while (year + fouryears*4 < 1970) {
    fouryears++;
  }
  t.tm_year = year - 1900 + fouryears * 4;
  t.tm_mon = month - 1;
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min = minute;
  t.tm_sec = int(second);
  ctime = mktime(&t);
}

bool Date::operator<(const Date& other) {
  return asSeconds() < other.asSeconds();
}

bool Date::operator>(const Date& other) {
  return asSeconds() > other.asSeconds();
}

double Date::asSeconds() const {
  return ctime + second - int(second) - double(fouryears) * SECONDS_PER_4_YEARS;
}

double Date::secondsUntil(const Date& other) const {
  return asSeconds() - other.asSeconds();
}


int Date::getYear() {
  return year;
}

int Date::getMonth() {
  return month;
}

int Date::getDay() {
  return day;
}

int Date::getHour() {
  return hour;
}

int Date::getMinute() {
  return minute;
}

double Date::getSecond() {
  return second;
}
