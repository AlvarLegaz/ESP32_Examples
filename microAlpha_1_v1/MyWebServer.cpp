#include "MyWebServer.h"
#include "HtmlResponses.h"
#include "Hardware.h"
#include <ArduinoJson.h> 
#include <WiFi.h>

MyWebServer::MyWebServer(int port) : server(port) {}


void MyWebServer::initEndpoints() {
  server.on("/", [this]() { handleIndex(); });
  server.on("/health", [this]() { handleHealth(); });

  server.on("/info", HTTP_GET, [this]() { handleInfo(); });

  server.on("/switch", HTTP_GET, [this]() { handleOutput(); });
  server.on("/switch", HTTP_PUT, [this]() { handleSetOutput(); });

  server.on("/input", HTTP_GET, [this]() { handleDInput(); });

  server.on("/config", HTTP_GET, [this]() { handleConfig(); });
  server.on("/config", HTTP_POST, [this]() { handleConfigPost(); });

  server.begin();
}


void MyWebServer::handleClient() {
  server.handleClient();
}


void MyWebServer::stop() {
  server.stop();
}


void MyWebServer::handleHealth() {
  server.send(200, "text/html", health_response);
}


void MyWebServer::handleIndex() {
  server.send(200, "text/html", index_response);
}


void MyWebServer::handleInfo() {
  StaticJsonDocument<256> json;

  // Agregar múltiples campos al objeto JSON
  json["BoardName"]   = HardwareManager::getBoardName();
  json["DeviceID"]    = HardwareManager::generateID();
  json["FirmwareVer"] = HardwareManager::getFirmwareVersion();
  json["Status"]      = "OK";
  json["Uptime"]      = millis() / 1000; // tiempo en segundos

  String response;
  serializeJson(json, response);

  server.send(200, "application/json", response);
}


void MyWebServer::handleOutput() {
  StaticJsonDocument<100> json;
  json["switch"] = HardwareManager::getOutput(1);

  String response;
  serializeJson(json, response);

  server.send(200, "application/json", response);
}


void MyWebServer::handleSetOutput() {
  // Parsear JSON del body
  if (server.hasArg("plain")) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, server.arg("plain"));

    if (error) {
      server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }

    // Obtener valor booleano
    if (!doc.containsKey("switch")) {
      server.send(400, "application/json", "{\"error\":\"Missing 'switch' key\"}");
      return;
    }

    bool state = doc["switch"];
    HardwareManager::setOutput(state);

    // Respuesta de confirmación
    String response;
    StaticJsonDocument<100> resDoc;
    resDoc["switch"] = state;
    serializeJson(resDoc, response);
    server.send(200, "application/json", response);
  } else {
    server.send(400, "application/json", "{\"error\":\"No body provided\"}");
  }
}


void MyWebServer::handleDInput() {
  StaticJsonDocument<100> json;
  json["input"] = HardwareManager::getDInput(1);

  String response;
  serializeJson(json, response);

  server.send(200, "application/json", response);
}


void MyWebServer::handleConfig() {
  String wifiList = "";
  int n = WiFi.scanNetworks();

  if (n == 0) {
    wifiList = "<p>No se encontraron redes WiFi.</p>";
  } else {
    wifiList += "<ul>";
    for (int i = 0; i < n; ++i) {
      wifiList += "<li>" + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + " dBm)";
      wifiList += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " [Abierta]" : " [Segura]";
      wifiList += "</li>";
    }
    wifiList += "</ul>";
  }

  String response = String(HTML_CONFIG_HEADER) + wifiList + String(HTML_CONFIG_FOOTER);
  server.send(200, "text/html", response);

  WiFi.scanDelete();
}


void MyWebServer::handleConfigPost() {
  String ssid = server.arg("ssid");
  String password = server.arg("password");

  Serial.println("SSID:"+ssid);
  Serial.println("PASS:"+password);

  // Guardar SSID/Password en NVS
  preferences.begin("wifi", false); // false = lectura y escritura
  preferences.putString("ssid", ssid);
  preferences.putString("password", password);
  preferences.end();

  server.send(200, "text/html",
    "<html><body><h1>Configuración Guardada. Reinicie el dispositivo.</h1></body></html>");
}