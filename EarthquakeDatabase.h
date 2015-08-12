#ifndef EarthquakeDatabase_h
#define EarthquakeDatabase_h

#include <G3D/G3DAll.h>
#include <vector>

#include "Earthquake.h"
#include "Date.h"

class EarthquakeDatabase {
public:
  
  // Creates an empty EarthquakeDatabase
  EarthquakeDatabase() { }
  
  // Creates an EarthquakeDatabase from file
  EarthquakeDatabase(G3D::String filename);
  
  // Returns Earthquake given index in file
  Earthquake getByIndex(int index);
  
  // Returns minimum index.  Note that this is not zero!
  int getMinIndex();
  
  // Returns maximum valid index.  Running
  // getByIndex(getMaxIndex()) WILL return the last earthquake in the file
  int getMaxIndex();
  
  // Returns the index of the most recent earthquake as of given date
  int getIndexByDate(Date d);
  
protected:
  std::vector<Earthquake> earthquakes;
};

#endif
