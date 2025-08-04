#include "main_program.h"
#include <MyWebServer.h>
#include "Hardware.h"

#include <WiFi.h>

#define SERIAL_SPEED 115200
#define SERVER_PORT 80

#define PRES_BUTT_MODE 0
#define SWITCH_MODE 1

MyWebServer server(SERVER_PORT);
Preferences preferences; 


void AppProgram::init(){
  HardwareManager::init();
  int normal_mode = HardwareManager::getMode();

  String ssid_apmode = "DotIOT_NET";
  String password_apmode = "123456789";

  preferences.begin("wifi", true);  // true = solo lectura
  String ssid = preferences.getString("ssid", "");
  String password = preferences.getString("password", "");
  preferences.end();

  Serial.begin(SERIAL_SPEED);

  if(normal_mode!=1){
    Serial.println("Arranque en modo AP");
    WiFi.softAP(ssid_apmode.c_str(), password_apmode.c_str());
    Serial.println("AP iniciado");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.softAPIP());

    server.initEndpoints();
    Serial.println("HTTP server started");
  }
  else{
    // Conéctate a la red WiFi
    Serial.println("Arranque en modo Host");

    if (ssid != "") {
      Serial.println("Conectando a: " + ssid);
      WiFi.begin(ssid.c_str(), password.c_str());

      // Espera hasta que el ESP32 se conecte a la red WiFi
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
      }
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.print("Dirección IP: ");
      Serial.println(WiFi.localIP());
      
      server.initEndpoints();
      Serial.println("HTTP server started");
      

    } else {
       Serial.println("No hay SSID guardado");
       error();
    }
  }
}

void AppProgram::run() {
  server.handleClient();
  handleButtonPress(1,1,PRES_BUTT_MODE);
  HardwareManager::runBlinkingLed();
}

void AppProgram::stop() {
  ;;
}

void AppProgram::error() {
  ;;
}

void AppProgram::handleButtonPress(int buttonPin, int ledPin, int button_mode) {
    static bool lastButtonState = HIGH;

    bool currentState = HardwareManager::getDInput(buttonPin);

    // Flanco de subida
    if (currentState == HIGH && lastButtonState == LOW) {
        delay(50);  // Antirrebote
        if (HardwareManager::getDInput(buttonPin) == HIGH) {
            if (button_mode == 1) {
                bool currentSwitchState = HardwareManager::getOutput(ledPin);
                Serial.print("Flanco de subida. Estado actual: ");
                Serial.println(currentSwitchState);
                HardwareManager::setOutput(!currentSwitchState);
            }
        }
    }

    // Flanco de bajada
    else if (currentState == LOW && lastButtonState == HIGH) {
        delay(50);  // Antirrebote
        if (HardwareManager::getDInput(buttonPin) == LOW) {
            if (button_mode == 1 || button_mode == 0) {
                bool currentSwitchState = HardwareManager::getOutput(ledPin);
                Serial.print("Flanco de bajada. Estado actual: ");
                Serial.println(currentSwitchState);
                HardwareManager::setOutput(!currentSwitchState);
            }
        }
    }

    lastButtonState = currentState;
}

