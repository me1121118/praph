#include "Praph.h"

PraphClass* PraphClass::instance = nullptr;

// Blynk Handlers
BLYNK_CONNECTED() {
    if (PraphClass::instance) {
        PraphClass::instance->onConnected();
    }
}

BLYNK_WRITE(V1) {
    if (PraphClass::instance) {
        PraphClass::instance->setOutput(0, param.asInt());
    }
}

BLYNK_WRITE(V2) {
    if (PraphClass::instance) {
        PraphClass::instance->setOutput(1, param.asInt());
    }
}

BLYNK_WRITE(V3) {
    if (PraphClass::instance) {
        PraphClass::instance->setOutput(2, param.asInt());
    }
}

BLYNK_WRITE(V4) {
    if (PraphClass::instance) {
        PraphClass::instance->setOutput(3, param.asInt());
    }
}

BLYNK_WRITE(V10) {
    if (PraphClass::instance) {
        int val = param.asInt();
        PraphClass::instance->setOutput(0, val);
        Blynk.virtualWrite(V1, val);
    }
}

// Button callback delegates
static void onButton1PressedDelegate() {
    if (PraphClass::instance) PraphClass::instance->toggleOutput(0);
}

static void onButton2PressedDelegate() {
    if (PraphClass::instance) PraphClass::instance->toggleOutput(1);
}

static void onButton3PressedDelegate() {
    if (PraphClass::instance) PraphClass::instance->toggleOutput(2);
}

static void onButton4PressedDelegate() {
    if (PraphClass::instance) PraphClass::instance->toggleOutput(3);
}

static void onSensorTimerDelegate() {
    if (PraphClass::instance) {
        PraphClass::instance->sendSensor();
    }
}

PraphClass::PraphClass() : _dht(nullptr), _initialized(false) {
    instance = this;
    for (int i = 0; i < 4; i++) {
        _buttons[i] = nullptr;
    }
}

PraphClass::~PraphClass() {
    delete _dht;
    for (int i = 0; i < 4; i++) {
        delete _buttons[i];
    }
}

void PraphClass::begin(const PraphConfig& config) {
    _config = config;
    _initialized = true;

    // Pin Modes
    uint8_t outPins[4] = {_config.output1Pin, _config.output2Pin, _config.output3Pin, _config.output4Pin};
    for (int i = 0; i < 4; i++) {
        pinMode(outPins[i], OUTPUT);
    }

    pinMode(_config.statusLedPin, OUTPUT);
    digitalWrite(_config.statusLedPin, 1); // initial OFF

    // DHT sensor
    _dht = new DHT(_config.dhtPin, _config.dhtType);
    _dht->begin();

    // Buttons
    uint8_t btnPins[4] = {_config.button1Pin, _config.button2Pin, _config.button3Pin, _config.button4Pin};
    void (*callbacks[4])() = {
        onButton1PressedDelegate,
        onButton2PressedDelegate,
        onButton3PressedDelegate,
        onButton4PressedDelegate
    };

    for (int i = 0; i < 4; i++) {
        _buttons[i] = new EasyButton(btnPins[i]);
        _buttons[i]->begin();
        _buttons[i]->onPressed(callbacks[i]);
    }

    // Static IP WiFi Setup
    if (!WiFi.config(_config.local_IP, _config.gateway, _config.subnet, _config.primaryDNS)) {
        Serial.println(F("[Praph] Failed to configure Static IP"));
    }

    WiFi.begin(_config.ssid, _config.pass);
    Serial.print(F("[Praph] Connecting to WiFi"));
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(F("."));
    }
    Serial.println(F("\n[Praph] WiFi Connected!"));
    Serial.print(F("[Praph] IP Address: "));
    Serial.println(WiFi.localIP());

    // Blynk SSL Setup
    Blynk.config(_config.auth, _config.server, _config.port);
    Blynk.connect();

    // 1-second sensor timer
    _timer.setInterval(1000L, onSensorTimerDelegate);
}

void PraphClass::begin(const char* ssid, const char* pass, const char* auth, const char* server, int port) {
    PraphConfig cfg;
    cfg.ssid = ssid;
    cfg.pass = pass;
    cfg.auth = auth;
    cfg.server = server;
    cfg.port = port;
    begin(cfg);
}

void PraphClass::run() {
    if (!_initialized) return;
    Blynk.run();
    _timer.run();
    for (int i = 0; i < 4; i++) {
        if (_buttons[i]) {
            _buttons[i]->read();
        }
    }
}

void PraphClass::onConnected() {
    digitalWrite(_config.statusLedPin, 0); // Active-low LED ON
}

void PraphClass::setOutput(uint8_t outputIndex, int state) {
    uint8_t outPins[4] = {_config.output1Pin, _config.output2Pin, _config.output3Pin, _config.output4Pin};
    if (outputIndex < 4) {
        digitalWrite(outPins[outputIndex], state);
    }
}

void PraphClass::toggleOutput(uint8_t outputIndex) {
    uint8_t outPins[4] = {_config.output1Pin, _config.output2Pin, _config.output3Pin, _config.output4Pin};
    if (outputIndex < 4) {
        int newState = !digitalRead(outPins[outputIndex]);
        digitalWrite(outPins[outputIndex], newState);
        // Sync to Blynk V1..V4
        Blynk.virtualWrite(V1 + outputIndex, newState);
    }
}

void PraphClass::sendSensor() {
    if (!_dht) return;
    float h = _dht->readHumidity();
    float t = _dht->readTemperature();

    if (isnan(h) || isnan(t)) {
        Serial.println(F("[Praph] Failed to read from DHT sensor!"));
        return;
    }
    Blynk.virtualWrite(V5, h);
    Blynk.virtualWrite(V6, t);
}

PraphClass Praph;
