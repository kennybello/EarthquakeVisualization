#ifndef App_h
#define App_h

#include <G3D/G3DAll.h>
#include "EarthquakeDatabase.h"
#include "Earth.h"

class App : public GApp {
public:
  
  App(const GApp::Settings& settings = GApp::Settings());
  
  virtual void onInit();

  virtual void onUserInput(UserInput *ui);
  virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt);

  virtual void onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface);
  virtual void onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& surface2D);

  
protected:

	CoordinateFrame rotate;

  double currentTime;
  bool playing;
  double playbackScale;
  
  // Database through which you can access the earthquakes
  EarthquakeDatabase eqd;
  
  // Object representing the earth
  shared_ptr<Earth> earth;
    
  // Font reference, used to draw text to the screen
  shared_ptr<GFont> font;

  // Rotation matrix for the earth and displayed earthquakes
  Matrix3 rotation;

  Vector3 planeNormal;
  
};

#endif