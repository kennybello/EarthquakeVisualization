#include "App.h"

// Tells C++ to invoke command-line main() function even on OS X and Win32.
G3D_START_AT_MAIN();

int main(int argc, const char* argv[]) {
  (void)argc; (void)argv;
  GApp::Settings settings(argc, argv);
  
  // Change the window and other startup parameters by modifying the
  // settings class.  For example:
  settings.window.width       = 1024; 
  settings.window.height      = 768;
  settings.window.resizable = true;
  //settings.film.enabled = false;
  return App(settings).run();
}

