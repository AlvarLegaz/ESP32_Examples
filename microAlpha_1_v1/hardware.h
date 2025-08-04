#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <Arduino.h>
#include <Preferences.h>

extern Preferences preferences;  // definición única del objeto

class HardwareManager {
public:
  static void init();

  static int getMode();

  static bool getOutput(int output);
  static void setOutput(bool value);

  static bool getDInput(int dinput);

  static void runBlinkingLed();
};

#endif // HARDWARE_H_
