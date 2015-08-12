#include "App.h"
#include "config.h"
#include "EarthquakeDatabase.h"
#include <iostream>
#include <sstream>

const int PLAYBACK_WINDOW = 365 * 24 * 60 * 60;

using namespace std;

App::App(const GApp::Settings& settings) : GApp(settings) {
  renderDevice->setColorClearValue(Color3(0.1, 0.12, 0.15));
  renderDevice->setSwapBuffersAutomatically(true);
}


void App::onInit() {
  // Typical G3D::GApp initialization options
  createDeveloperHUD();
  debugWindow->setVisible(false);
  developerWindow->setVisible(false);
  developerWindow->cameraControlWindow->setVisible(false);
  showRenderingStats = false;
  m_cameraManipulator.reset();

  activeCamera()->setPosition(Vector3(0,0,2));
    activeCamera()->lookAt(Vector3(0,0,0), Vector3(0,1,0));
    activeCamera()->setFarPlaneZ(-100);

	shared_ptr<Light> light = Light::directional("downlight", Vector3(.5,.5,-1), Color3(1.4, 1.3, 1.2));

	earth.reset(new Earth(activeCamera(), light));

  // Initialization for this Application
  // Note:  DATA_PATH is set in config.h
  eqd = EarthquakeDatabase(DATA_PATH);
  playbackScale = 86400;
  currentTime = eqd.getByIndex(eqd.getMinIndex()).getDate().asSeconds();
  font = GFont::fromFile("$G3D10DATA/font/arial.fnt");
  rotate = CoordinateFrame( Matrix3::fromAxisAngle( Vector3(0,1,0), pi()/2 ) * Matrix3::fromAxisAngle( Vector3(1,0,0), -pi()/2 ) );
  rotation = rotate.rotation;
  playing = true;
}


void App::onUserInput(UserInput *ui) {
  // Speedup or slowdown the playback
  if (ui->keyDown(GKey::LEFT)) {
    playbackScale /= 1.3;
  }
  if (ui->keyDown(GKey::RIGHT)) {
    playbackScale *= 1.3;
  }

  // Dolly the camera closer or farther away from the earth
  if (ui->keyDown(GKey::UP)) {
      Vector3 newCamPos = activeCamera()->frame().translation + Vector3(0,0,-0.01);
      if (newCamPos[2] > 1.2) {
        activeCamera()->setPosition(newCamPos);
      }
  }
  if (ui->keyDown(GKey::DOWN)) {
      Vector3 newCamPos = activeCamera()->frame().translation + Vector3(0,0,0.01);
      activeCamera()->setPosition(newCamPos);
  }

  // Rotate the earth when the user clicks and drags the mouse
  if (ui->keyDown(GKey::LEFT_MOUSE)) {
    // TODO: Update the "rotation" matrix based on how the user has dragged the mouse
    // Note: You can get mouse movement since the last frame with:  
	Vector2 dxy = ui->mouseDXY();
	if (!dxy.isZero()) {
		rotation = Matrix3::fromAxisAngle(-Vector3(dxy.x, dxy.y, 0).cross(-Vector3::unitZ()).unit(), -dxy.length()/500) * rotate.rotation;
	}
  }
}


void App::onSimulation(RealTime rdt, SimTime sdt, SimTime idt) {
  GApp::onSimulation(rdt, sdt, idt);

  rotate = CoordinateFrame(rotation);

  // Advance the current time and loop back to the start if time is past the last earthquake
  currentTime += playbackScale * rdt;
  if (currentTime > eqd.getByIndex(eqd.getMaxIndex()).getDate().asSeconds()) {
    currentTime = eqd.getByIndex(eqd.getMinIndex()).getDate().asSeconds();
  }
  if (currentTime < eqd.getByIndex(eqd.getMinIndex()).getDate().asSeconds()) {
    currentTime = eqd.getByIndex(eqd.getMaxIndex()).getDate().asSeconds();
  }
}



void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D) {
  rd->clear();
  rd->setShadeMode(RenderDevice::SHADE_SMOOTH);


  // Set a rotation matrix to apply when drawing the earth and earthquakes
  rd->pushState();
  rd->setObjectToWorldMatrix(CoordinateFrame(rotate));
  earth->draw(rd);

  // Draw earthquakes
  int start = eqd.getIndexByDate(Date(currentTime - PLAYBACK_WINDOW));
  int end = eqd.getIndexByDate(Date(currentTime));
  for (int x=start; x<end; x++) {
    Earthquake e = eqd.getByIndex(x);
	double radius = ((e.getMagnitude() / 10.0 - .5) / (1.0 - .5) / 6);
	Color3 color = 	color = Color3((e.getMagnitude() / 10.0 - .6) / (.8 - .6), 1 - (e.getMagnitude() / 10.0 - .6) / (.8 - .6), 0);
	if (e.getMagnitude() < 6.0 && e.getMagnitude() > 5.0) {
		color = Color3(0, 1, 0);
	}
	else if (e.getMagnitude() < 10.0 && e.getMagnitude() > 8.0) {
		color = Color3(1, 0, 0);
	}
	Sphere ball(earth->getPosition(e.getLatitude(), e.getLongitude()), radius);
	Draw::sphere(ball, rd, color, Color4::clear());
  }

  rd->popState();

  // Call to make the GApp show the output of debugDraw
  drawDebugShapes();
}


void App::onGraphics2D(RenderDevice* rd, Array<Surface2D::Ref>& posed2D) {
  Surface2D::sortAndRender(rd, posed2D);
  Date d(currentTime);
  stringstream s;
  s << "Current date: " << d.getMonth()
    << "/" << d.getDay()
    << "/" << d.getYear()
    << "  " << d.getHour()
    << ":" << d.getMinute();
  const G3D::String g3dString = s.str().c_str();
  font->draw2D(rd, g3dString, Point2(30,30), 12, Color3::white());
}




