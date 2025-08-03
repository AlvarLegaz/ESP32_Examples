#include <WiFi.h>
#include <WebServer.h>
#include "camera_driver_OV2640.h"

// Tu red WiFi (modo estación)
const char *ssid = "MOVISTAR-WIFI6-48A8";
const char *password= "aMYFm4cR4f3c79T4Xc3X";

WebServer server(80);

// Manejador del endpoint /capture
void handleCapture() {
  uint8_t *img_buf = nullptr;
  size_t img_len = 0;
  uint16_t width = 0, height = 0;

  if (getImage(&img_buf, &img_len, &width, &height) == ESP_OK) {
    server.setContentLength(img_len);
    server.send(200, "image/jpeg", "");  // Inicia la respuesta sin cuerpo
    server.sendContent((const char*)img_buf, img_len);
    free(img_buf);
  } else {
    server.send(500, "text/plain", "Error al capturar imagen");
  }
}

void setup() {
  Serial.begin(115200);

  if (!initCamera()) {
    Serial.println("Fallo en cámara");
    while (true);
  }

  // Conexión WiFi
  
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());

  // Endpoint
  server.on("/capture", handleCapture);
  server.begin();
  Serial.println("Servidor HTTP listo");
  
}

void loop() {
  server.handleClient();

}
