#include <Praph.h>

PraphConfig config;

void setup() {
  Serial.begin(115200);

  // WiFi & Blynk Settings
  config.ssid   = "TP-Link_BA47";
  config.pass   = "99982515";
  config.auth   = "oH4pH8RH4VmegPLsptm91FVkIy7pwhQ1";
  config.server = "thanin.tak.rmutl.ac.th";
  config.port   = 9443;

  // Static IP Network
  config.local_IP   = IPAddress(10, 64, 20, 22);
  config.gateway    = IPAddress(10, 64, 20, 254);
  config.subnet     = IPAddress(255, 255, 255, 0);
  config.primaryDNS = IPAddress(8, 8, 8, 8);

  // Start Praph Library
  Praph.begin(config);
}

void loop() {
  Praph.run();
}
