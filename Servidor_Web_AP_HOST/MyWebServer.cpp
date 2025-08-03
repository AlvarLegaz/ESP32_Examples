#include "MyWebServer.h"
#include "HtmlResponses.h"
#include "Hardware.h"
#include <ArduinoJson.h> 
#include <WiFi.h>

MyWebServer::MyWebServer(int port) : server(port) {}

void MyWebServer::initEndpoints() {
  server.on("/", [this]() { handleIndex(); });
  server.on("/health", [this]() { handleHealth(); });
  server.on("/config", HTTP_GET, [this]() { handleConfig(); });
  server.on("/config", HTTP_POST, [this]() { handleConfigPost(); });

  // /output → todas las salidas.
  server.on("/output", HTTP_GET, [this]() { handleOutput(); });

  // /dinput → todas las entradas digitales.
  server.on("/dinput", HTTP_GET, [this]() { handleDInput(); });

  // /output/xx → salida específica
  server.onNotFound([this]() {
    String uri = server.uri();
    if (uri.startsWith("/output/")) {
      handleOutput();  // manejará ruta específica
    } 
    else if(uri.startsWith("/dinput/")){
      handleDInput(); // manejará ruta específica
    }
    else {
      server.send(404, "application/json", "{\"error\":\"Not found\"}");
    }
  });

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

  server.send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html lang="es">
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body {
          font-family: Arial, sans-serif;
          padding: 20px;
          text-align: center;
        }

        .container {
          display: flex;
          flex-direction: row;
          flex-wrap: wrap;
          justify-content: space-around;
          gap: 40px;
          width: 100%;
          max-width: 800px;
          margin: 0 auto;
          text-align: left;
        }

        .form-container, .wifi-container {
          flex: 1;
          min-width: 250px;
        }

        form input[type="text"] {
          width: 100%;
          padding: 8px;
          margin: 6px 0;
          box-sizing: border-box;
        }

        input[type="submit"] {
          padding: 10px;
          width: 100%;
        }

        ul {
          list-style: none;
          padding: 0;
        }

        @media (max-width: 600px) {
          .container {
            flex-direction: column;
          }
        }
      </style>
      <title>Configuración WiFi</title>
    </head>
    <body>
      <h1>DotIOT</h1>

      <div class="container">
        <div class="form-container">
          <h3>Configurar WiFi</h3>
          <form action="/config" method="post">
            <label for="ssid">SSID:</label><br>
            <input type="text" id="ssid" name="ssid"><br>
            <label for="password">Contraseña:</label><br>
            <input type="text" id="password" name="password"><br><br>
            <input type="submit" value="Enviar">
          </form>
        </div>
        <div class="wifi-container">
          <h3>Redes disponibles</h3>
          )rawliteral" + wifiList + R"rawliteral(
        </div>
      </div>
    </body>
    </html>
  )rawliteral");

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

void MyWebServer::handleOutput() {
  String path = server.uri();

  if (path == "/output") {
    // Todas las salidas
    StaticJsonDocument<256> json;
    json["output_1"] = HardwareManager::getOutput(1);
    json["output_2"] = HardwareManager::getOutput(2);

    String response;
    serializeJson(json, response);
    server.send(200, "application/json", response);
    return;
  }

  // Salida específica /output/XX
  int pin = path.substring(path.lastIndexOf("/") + 1).toInt();

  if (pin != 1 && pin != 2) {
    server.send(400, "application/json", "{\"error\":\"Pin no válido\"}");
    return;
  }

  bool state = HardwareManager::getOutput(pin);

  StaticJsonDocument<128> json;
  json["output"] = pin;
  json["state"] = state;

  String response;
  serializeJson(json, response);
  server.send(200, "application/json", response);
}

void MyWebServer::handleDInput() {
  String path = server.uri();

    if (path == "/dinput") {
    // Todas las salidas
    StaticJsonDocument<256> json;
    json["digital_input_1"] = HardwareManager::getDInput(1);
    json["digital_input_2"] = HardwareManager::getDInput(2);

    String response;
    serializeJson(json, response);
    server.send(200, "application/json", response);
    return;
  }

    // Entrada específica /dinput/XX
  int pin = path.substring(path.lastIndexOf("/") + 1).toInt();

  if (pin != 1 && pin != 2) {
    server.send(400, "application/json", "{\"error\":\"Pin no válido\"}");
    return;
  }

  bool state = HardwareManager::getDInput(pin);

  StaticJsonDocument<128> json;
  json["dinput"] = pin;
  json["state"] = state;

  String response;
  serializeJson(json, response);
  server.send(200, "application/json", response);

}

   