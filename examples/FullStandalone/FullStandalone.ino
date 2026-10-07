#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp8266_SSL.h>
#include <EasyButton.h>
#include <DHT.h>

#define BLYNK_PRINT Serial
#define BLYNK_SSL_USE_LETSENCRYPT

#define BUTTON_1_PIN 0
#define BUTTON_2_PIN 9   
#define BUTTON_3_PIN 10  
#define BUTTON_4_PIN 14

EasyButton button1(BUTTON_1_PIN);
EasyButton button2(BUTTON_2_PIN);
EasyButton button3(BUTTON_3_PIN);
EasyButton button4(BUTTON_4_PIN);

#define OUTPUT_1_PIN 12
#define OUTPUT_2_PIN 4   
#define OUTPUT_3_PIN 5   
#define OUTPUT_4_PIN 16  

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

int port = 9443;
char ssid[] = "TP-Link_BA47";
char pass[] = "99982515";
char auth[] = "oH4pH8RH4VmegPLsptm91FVkIy7pwhQ1";

IPAddress local_IP(10, 64, 20, 22);
IPAddress gateway(10, 64, 20, 254);    
IPAddress subnet(255, 255, 255, 0);   
IPAddress primaryDNS(8, 8, 8, 8);      

BLYNK_CONNECTED() {
  digitalWrite(13, 0); 
} 

BLYNK_WRITE(V1) { digitalWrite(OUTPUT_1_PIN, param.asInt()); }
BLYNK_WRITE(V2) { digitalWrite(OUTPUT_2_PIN, param.asInt()); }
BLYNK_WRITE(V3) { digitalWrite(OUTPUT_3_PIN, param.asInt()); }
BLYNK_WRITE(V4) { digitalWrite(OUTPUT_4_PIN, param.asInt()); }

BLYNK_WRITE(V10) {
  int L1 = param.asInt();
  digitalWrite(12, L1);
  Blynk.virtualWrite(V1, digitalRead(12));
}

void onButton1Pressed() {
  digitalWrite(OUTPUT_1_PIN, !digitalRead(OUTPUT_1_PIN));
  Blynk.virtualWrite(V1, digitalRead(OUTPUT_1_PIN));
}

void onButton2Pressed() {
  digitalWrite(OUTPUT_2_PIN, !digitalRead(OUTPUT_2_PIN));
  Blynk.virtualWrite(V2, digitalRead(OUTPUT_2_PIN));
}

void onButton3Pressed() {
  digitalWrite(OUTPUT_3_PIN, !digitalRead(OUTPUT_3_PIN));
  Blynk.virtualWrite(V3, digitalRead(OUTPUT_3_PIN));
}

void onButton4Pressed() {
  digitalWrite(OUTPUT_4_PIN, !digitalRead(OUTPUT_4_PIN));
  Blynk.virtualWrite(V4, digitalRead(OUTPUT_4_PIN));
}

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void setup() {
  Serial.begin(115200);

  pinMode(OUTPUT_1_PIN, OUTPUT);
  pinMode(OUTPUT_2_PIN, OUTPUT);
  pinMode(OUTPUT_3_PIN, OUTPUT);
  pinMode(OUTPUT_4_PIN, OUTPUT);
  
  pinMode(13, OUTPUT); 
  digitalWrite(13, 1); 
  
  dht.begin();
  button1.begin(); button1.onPressed(onButton1Pressed);
  button2.begin(); button2.onPressed(onButton2Pressed);
  button3.begin(); button3.onPressed(onButton3Pressed);
  button4.begin(); button4.onPressed(onButton4Pressed);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
    Serial.println("Failed to configure Static IP");
  }
  
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100); 
    Serial.print(".");
  }
  
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Blynk.config(auth, "thanin.tak.rmutl.ac.th", port);
  Blynk.connect();
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();

  button1.read();
  button2.read();
  button3.read();
  button4.read();
}
