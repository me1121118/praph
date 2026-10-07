#ifndef PRAPH_H
#define PRAPH_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define BLYNK_PRINT Serial
#define BLYNK_SSL_USE_LETSENCRYPT
#include <BlynkSimpleEsp8266_SSL.h>
#include <EasyButton.h>
#include <DHT.h>

struct PraphConfig {
    const char* ssid;
    const char* pass;
    const char* auth;
    const char* server;
    int port;
    
    // Static IP network configuration
    IPAddress local_IP;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress primaryDNS;

    // Pin definitions with defaults
    uint8_t button1Pin;
    uint8_t button2Pin;
    uint8_t button3Pin;
    uint8_t button4Pin;

    uint8_t output1Pin;
    uint8_t output2Pin;
    uint8_t output3Pin;
    uint8_t output4Pin;

    uint8_t dhtPin;
    uint8_t dhtType;
    uint8_t statusLedPin;

    PraphConfig() :
        ssid(""),
        pass(""),
        auth(""),
        server("thanin.tak.rmutl.ac.th"),
        port(9443),
        local_IP(10, 64, 20, 22),
        gateway(10, 64, 20, 254),
        subnet(255, 255, 255, 0),
        primaryDNS(8, 8, 8, 8),
        button1Pin(0),
        button2Pin(9),
        button3Pin(10),
        button4Pin(14),
        output1Pin(12),
        output2Pin(4),
        output3Pin(5),
        output4Pin(16),
        dhtPin(2),
        dhtType(DHT22),
        statusLedPin(13)
    {}
};

class PraphClass {
public:
    PraphClass();
    ~PraphClass();

    void begin(const PraphConfig& config);
    void begin(const char* ssid, const char* pass, const char* auth, const char* server = "thanin.tak.rmutl.ac.th", int port = 9443);
    void run();

    void toggleOutput(uint8_t outputIndex);
    void setOutput(uint8_t outputIndex, int state);
    void sendSensor();
    void onConnected();

    static PraphClass* instance;

private:
    PraphConfig _config;
    EasyButton* _buttons[4];
    DHT* _dht;
    BlynkTimer _timer;
    bool _initialized;
};

extern PraphClass Praph;

#endif // PRAPH_H
