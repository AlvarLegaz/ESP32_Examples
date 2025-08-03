#include <WiFi.h>
#include <MyWebServer.h>
#include "Hardware.h"

#define SERIAL_SPEED 115200
#define SERVER_PORT 80

MyWebServer server(SERVER_PORT);
int normal_mode = 0;

Preferences preferences; 

void setup() {
  HardwareManager::init();
  normal_mode = HardwareManager::getMode();

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

void loop() {
  // Maneja las solicitudes del servidor
  server.handleClient();
}

void error() {
  ;;
}
