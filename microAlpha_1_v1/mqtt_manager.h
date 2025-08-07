#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>

class MqttManager {
public:
    MqttManager(const char* mqttServer, uint16_t mqttPort);
    void begin();
    void loop();
    bool publish(const char* topic, const char* payload);
    bool isConnected();

    void setCredentials(const char* username, const char* password);
    void setClientId(const char* clientId);

private:
    void reconnect();

    const char* _mqttServer;
    uint16_t _mqttPort;
    const char* _username = nullptr;
    const char* _password = nullptr;
    const char* _clientId = "ESP32Client";

    WiFiClient _wifiClient;
    PubSubClient _mqttClient;
};

#endif // MQTTMANAGER_H
