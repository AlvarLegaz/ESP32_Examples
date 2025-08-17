#include "main_program.h"
#include <MyWebServer.h>
#include "Hardware.h"
#include "mqtt_manager.h"
#include "config.h"

#include <WiFi.h>

#define SERIAL_SPEED 115200
#define SERVER_PORT 80

#define PRES_BUTT_MODE 0
#define SWITCH_MODE 1

MyWebServer server(SERVER_PORT);

const char* mqtt_server = "192.168.1.74";  // IP del servidor MQTT
const uint16_t mqtt_port = 1883;
MqttManager mqtt(mqtt_server, mqtt_port);

Preferences preferences; 

void AppProgram::init(){

  boardName = BOARD_NAME;
  deviceID = HardwareManager::generateID();

  HardwareManager::init();

  int normal_mode = HardwareManager::getMode();

  String ssid_apmode = "DotIOT_NET";
  String password_apmode = "123456789";

  preferences.begin("wifi", true);  // true = solo lectura
  String ssid = preferences.getString("ssid", "");
  String password = preferences.getString("password", "");
  preferences.end();

  // Configurar cliente MQTT
  mqtt.setClientId("ESP32_test_publisher");
  // mqtt.setCredentials("usuario", "contraseña"); // solo si tu broker lo requiere
  mqtt.begin();

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

String AppProgram::getDeviceID(){
  return deviceID;
}

String AppProgram::getBoardName(){
  return boardName;
}

void AppProgram::run() {
    server.handleClient();
    handleButtonPress(1, 1, PRES_BUTT_MODE);
    HardwareManager::runBlinkingLed();

    /*
    mqtt.loop();

    static unsigned long lastSent = 0;
    unsigned long now = millis();

    if (now - lastSent > 5000) {
        lastSent = now;

        if (mqtt.isConnected()) {
            bool ok = mqtt.publish("test/topic", "Mensaje desde ESP32");
            Serial.println(ok ? "Publicado correctamente" : "Error al publicar");
        } else {
            Serial.println("No conectado a MQTT");
        }
    }
    */
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

