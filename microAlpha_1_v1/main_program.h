#ifndef APPPROGRAM_H
#define APPPROGRAM_H

#include <Arduino.h>


class AppProgram {
  
public:
  void init();
  String getDeviceID();
  String getBoardName();
  void run();
  void stop();

private:
  int majorVersion;
  int minorVersion;
  int subVersion;
  String boardName;
  String deviceID;
  bool isRunning_;
  int button_mode;

  void handleButtonPress(int buttonPin, int ledPin, int button_mode);
  void error();
};

#endif // APPPROGRAM_H