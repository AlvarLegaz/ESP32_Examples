#include "mqtt_manager.h"

MqttManager::MqttManager(const char* mqttServer, uint16_t mqttPort)
    : _mqttServer(mqttServer), _mqttPort(mqttPort), _mqttClient(_wifiClient) {}

void MqttManager::begin() {
    _mqttClient.setServer(_mqttServer, _mqttPort);
}

void MqttManager::loop() {
    if (!_mqttClient.connected()) {
        reconnect();
    }
    _mqttClient.loop();
}

bool MqttManager::publish(const char* topic, const char* payload) {
    if (_mqttClient.connected()) {
        return _mqttClient.publish(topic, payload);
    }
    return false;
}

void MqttManager::reconnect() {
    static unsigned long lastAttempt = 0;
    unsigned long now = millis();

    if (_mqttClient.connected()) return;

    if (now - lastAttempt > 5000) {
        lastAttempt = now;
        if (_mqttClient.connect(_clientId, _username, _password)) {
            Serial.println("Conectado a MQTT");
        } else {
            Serial.print("Fallo en conexión MQTT. Estado: ");
            Serial.println(_mqttClient.state());
        }
    }
}

bool MqttManager::isConnected(){
    return _mqttClient.connected();
}

void MqttManager::setCredentials(const char* username, const char* password) {
    _username = username;
    _password = password;
}

void MqttManager::setClientId(const char* clientId) {
    _clientId = clientId;
}
