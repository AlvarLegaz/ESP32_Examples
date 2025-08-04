#include "main_program.h"

AppProgram myApp;

void setup() { 
  myApp.init();
}

void loop() {
  // Maneja las solicitudes del servidor
  myApp.run();
}

