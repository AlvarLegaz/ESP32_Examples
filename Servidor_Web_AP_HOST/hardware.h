#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <Arduino.h>
#include <Preferences.h>

extern Preferences preferences;  // definición única del objeto

class HardwareManager {
public:
  static void init();

  static int getMode();

  static int getOutput(int output);
  static int setOutput(int output, bool value);

  static int getDInput(int dinput);
};

#endif // HARDWARE_H_
