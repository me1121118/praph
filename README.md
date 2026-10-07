# praph

ระบบควบคุมอุปกรณ์ IoT ผ่าน **Blynk (SSL)** ร่วมกับ **ESP8266**, เซ็นเซอร์วัดอุณหภูมิและความชื้น **DHT22**, สวิตช์ปุ่มกด 4 ช่อง (**EasyButton**) และรีเลย์ควบคุมเอาต์พุต 4 ช่อง

---

## 📌 ภาพรวมโครงการ (Project Overview)

**praph** เป็นโค้ดควบคุมไมโครคอนโทรลเลอร์ ESP8266 (เช่น NodeMCU หรือ WeMos D1 Mini) สำหรับการจัดการระบบ Smart Home / IoT Device โดยมีฟังก์ชันหลักดังนี้:
- เชื่อมต่อเครือข่าย WiFi ด้วย Static IP Configuration
- สื่อสารกับ Blynk Server แบบเข้ารหัสความปลอดภัย SSL ผ่านพอร์ต 9443 (`thanin.tak.rmutl.ac.th`)
- อ่านค่าอุณหภูมิและความชื้นจากเซ็นเซอร์ **DHT22** และส่งขึ้น Blynk ทุก 1 วินาที
- ควบคุมเอาต์พุต 4 ช่องสัญญาณ (Relay/LED) ผ่านแอปพลิเคชัน Blynk และสวิตช์ปุ่มกดหน้างาน (Toggle Mode)
- ตรวจจับการกดปุ่มแบบไม่สะท้อนสัญญาณ (Debounce) ด้วยไลบรารี **EasyButton**
- มีไฟ LED แสดงสถานะการเชื่อมต่อ Blynk (Pin 13)

---

## 🔌 ตารางการต่อขาใช้งาน (Hardware Pin Mapping)

| หน้าที่การทำงาน (Function) | อุปกรณ์ / สัญญาณ | ขา ESP8266 (GPIO) | หมายเหตุ |
|---|---|---|---|
| **Button 1** | สวิตช์ปุ่มกด 1 | GPIO 0 (D3) | ควบคุม Output 1 / Virtual Pin V1 |
| **Button 2** | สวิตช์ปุ่มกด 2 | GPIO 9 (SD2) | ควบคุม Output 2 / Virtual Pin V2 |
| **Button 3** | สวิตช์ปุ่มกด 3 | GPIO 10 (SD3) | ควบคุม Output 3 / Virtual Pin V3 |
| **Button 4** | สวิตช์ปุ่มกด 4 | GPIO 14 (D5) | ควบคุม Output 4 / Virtual Pin V4 |
| **Output 1** | รีเลย์ / หลอดไฟ 1 | GPIO 12 (D6) | สั่งการผ่าน V1 หรือ V10 |
| **Output 2** | รีเลย์ / หลอดไฟ 2 | GPIO 4 (D2) | สั่งการผ่าน V2 |
| **Output 3** | รีเลย์ / หลอดไฟ 3 | GPIO 5 (D1) | สั่งการผ่าน V3 |
| **Output 4** | รีเลย์ / หลอดไฟ 4 | GPIO 16 (D0) | สั่งการผ่าน V4 |
| **DHT22 Sensor** | Data Pin | GPIO 2 (D4) | วัดค่าความชื้นและอุณหภูมิ |
| **Status LED** | ไฟแสดงสถานะ Blynk | GPIO 13 (D7) | ดับเมื่อเชื่อมต่อ Blynk สำเร็จ |

---

## 📱 ตาราง Virtual Pin บน Blynk

| Virtual Pin | ประเภทข้อมูล | ทิศทาง | คำอธิบาย |
|---|---|---|---|
| **V1** | Integer (0/1) | Input / Output | สั่งเปิด-ปิด Output 1 (และอัปเดตสถานะตามปุ่มกด 1) |
| **V2** | Integer (0/1) | Input / Output | สั่งเปิด-ปิด Output 2 (และอัปเดตสถานะตามปุ่มกด 2) |
| **V3** | Integer (0/1) | Input / Output | สั่งเปิด-ปิด Output 3 (และอัปเดตสถานะตามปุ่มกด 3) |
| **V4** | Integer (0/1) | Input / Output | สั่งเปิด-ปิด Output 4 (และอัปเดตสถานะตามปุ่มกด 4) |
| **V5** | Float (%) | Output to App | แสดงค่าความชื้นสัมพัทธ์ (Humidity) จาก DHT22 |
| **V6** | Float (°C) | Output to App | แสดงค่าอุณหภูมิ (Temperature) จาก DHT22 |
| **V10** | Integer (0/1) | Input from App | สั่งควบคุม Output 1 และ Sync ค่ากลับไปยัง V1 |

---

## 📦 ไลบรารีที่จำเป็น (Dependencies & Libraries)

ต้องติดตั้งไลบรารีต่อไปนี้ใน **Arduino IDE** (Sketch -> Include Library -> Manage Libraries):
1. **ESP8266 Board Package** (v3.x หรือใหม่กว่า)
2. **Blynk** โดย Volodymyr Shymanskyy (รองรับ BlynkSimpleEsp8266_SSL.h)
3. **EasyButton** โดย Evert Arias
4. **DHT sensor library** โดย Adafruit
5. **Adafruit Unified Sensor** โดย Adafruit

---

## ⚙️ การตั้งค่าและการใช้งาน (Configuration & Usage)

1. เปิดไฟล์ `praph.ino` ด้วย Arduino IDE
2. ปรับแต่งค่าพารามิเตอร์การเชื่อมต่อ WiFi และ Blynk:
   ```cpp
   char ssid[] = "YOUR_WIFI_SSID";
   char pass[] = "YOUR_WIFI_PASSWORD";
   char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
   
   // กำหนดค่า Static IP ตามโครงข่ายที่ต้องการ
   IPAddress local_IP(10, 64, 20, 22);
   IPAddress gateway(10, 64, 20, 254);
   IPAddress subnet(255, 255, 255, 0);
   IPAddress primaryDNS(8, 8, 8, 8);
   ```
3. เลือกบอร์ดเป็น `NodeMCU 1.0 (ESP-12E Module)` หรือบอร์ด ESP8266 ที่ใช้งาน
4. เลือก COM Port ให้ถูกต้องแล้วทำการ **Upload**

---

## 🔍 อธิบายการทำงานของโค้ดอย่างละเอียด (Code Explanation)

โค้ดนี้แบ่งออกเป็น 8 ส่วนหลักตามหน้าที่การทำงาน:

### 1. นำเข้าไลบรารี (Libraries Inclusion)
```cpp
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp8266_SSL.h>
#include <EasyButton.h>
#include <DHT.h>
```
* **ESP8266WiFi.h / WiFiClientSecure.h:** จัดการระบบ Wi-Fi และการเชื่อมต่อเครือข่ายความปลอดภัยสูงผ่าน TLS/SSL
* **BlynkSimpleEsp8266_SSL.h:** ไลบรารีเชื่อมต่อกับ Blynk Server ผ่านพอร์ต SSL
* **EasyButton.h:** จัดการปุ่มกด ลดปัญหาสัญญาณกระเพื่อม (Debounce) และรองรับระบบ Callback เมื่อกดปุ่ม
* **DHT.h:** ไลบรารีอ่านค่าอุณหภูมิและความชื้นจากเซ็นเซอร์ตระกูล DHT

### 2. การกำหนดค่าระบบและความปลอดภัย (Definitions & Constants)
```cpp
#define BLYNK_PRINT Serial
#define BLYNK_SSL_USE_LETSENCRYPT
```
* **BLYNK_PRINT Serial:** ให้ Blynk พ่นข้อความสถานะและการเชื่อมต่อ (Debug Logs) ออกทาง Serial Monitor (115200 bps)
* **BLYNK_SSL_USE_LETSENCRYPT:** กำหนดให้ใช้ Root Certificate ของ Let's Encrypt สำหรับการตรวจสอบใบรับรอง SSL กับเซิร์ฟเวอร์

### 3. กำหนดขาพินฮาร์ดแวร์และสร้าง Object (Pin Assignment & Object Initialization)
```cpp
#define BUTTON_1_PIN 0
#define BUTTON_2_PIN 9   
#define BUTTON_3_PIN 10  
#define BUTTON_4_PIN 14

EasyButton button1(BUTTON_1_PIN);
...
#define OUTPUT_1_PIN 12
...
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
```
* กำหนดขาปุ่มกด 4 ปุ่ม (GPIO 0, 9, 10, 14) และผูกเข้ากับ Object `button1` ถึง `button4`
* กำหนดขาเอาต์พุต 4 ช่อง (GPIO 12, 4, 5, 16) สำหรับควบคุม Relay หรือ LED
* สร้าง Object `dht` สำหรับอ่านเซ็นเซอร์ DHT22 ที่ต่ออยู่ที่ขา GPIO 2
* สร้าง `BlynkTimer timer` เพื่อใช้จับเวลาทำงานแบบ Non-blocking (ไม่ต้องใช้ `delay()`)

### 4. การตั้งค่าการเชื่อมต่อเครือข่ายและ Blynk (Network Configuration)
```cpp
int port = 9443;
char ssid[] = "TP-Link_BA47";
char pass[] = "99982515";
char auth[] = "oH4pH8RH4VmegPLsptm91FVkIy7pwhQ1";

IPAddress local_IP(10, 64, 20, 22);
...
```
* **port (9443):** พอร์ต SSL ของ Private Blynk Server
* **ssid / pass:** ชื่อ WiFi และรหัสผ่านที่ ESP8266 ใช้เชื่อมต่อ
* **auth:** โทเคนประจำโปรเจกต์จากแอปพลิเคชัน Blynk
* **IPAddress:** กำหนดค่าเน็ตเวิร์กแบบ Static IP (IP, Gateway, Subnet, DNS)

### 5. การสื่อสารกับ Blynk (Blynk Handlers)
```cpp
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
```
* **BLYNK_CONNECTED():** ฟังก์ชันจะทำงานอัตโนมัติเมื่อ ESP8266 ต่อเซิร์ฟเวอร์ Blynk ติด โดยสั่งดับไฟ LED ที่ขา 13 (Active-Low) เพื่อแจ้งว่าออนไลน์แล้ว
* **BLYNK_WRITE(V1..V4):** เมื่อกดปุ่มบนมือถือ ค่า 0 หรือ 1 (`param.asInt()`) จะถูกสั่งไปยังขาเอาต์พุตโดยตรง
* **BLYNK_WRITE(V10):** คำสั่งพิเศษสำหรับควบคุม Output 1 (GPIO 12) และอ่านสถานะกลับไปอัปเดตปุ่ม V1 บนแอปให้ตรงกัน (Sync State)

### 6. ฟังก์ชันการกดปุ่มหน้างาน (Physical Button Callback Functions)
```cpp
void onButton1Pressed() {
  digitalWrite(OUTPUT_1_PIN, !digitalRead(OUTPUT_1_PIN));
  Blynk.virtualWrite(V1, digitalRead(OUTPUT_1_PIN));
}
// (onButton2Pressed ถึง onButton4Pressed ทำงานแบบเดียวกัน)
```
* ทำงานแบบ **Toggle Switch**: เมื่อกดปุ่ม จะสลับสถานะปัจจุบันของเอาต์พุต (ถ้าเปิดอยู่จะปิด ถ้าปิดอยู่จะเปิด) ด้วยคำสั่ง `!digitalRead(OUTPUT_PIN)`
* ส่งสถานะใหม่กลับขึ้นแอป Blynk ผ่าน `Blynk.virtualWrite(...)` ทันที ทำให้สถานะบนแอปและหน้างานตรงกันตลอดเวลา

### 7. ฟังก์ชันอ่านและส่งค่าเซ็นเซอร์ (Sensor Data Acquisition)
```cpp
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
```
* อ่านค่าความชื้นสัมพัทธ์ (`h`) และอุณหภูมิ (`t`)
* ตรวจสอบความถูกต้องด้วย `isnan()` (Is Not a Number) หากเซ็นเซอร์อ่านค่าไม่ได้จะแสดง Error ใน Serial และหยุดทำงานรอบนั้น
* ส่งค่าความชื้นขึ้นพิน `V5` และส่งค่าอุณหภูมิขึ้นพิน `V6` บน Blynk

### 8. ฟังก์ชันหลัก (Setup & Loop)
```cpp
void setup() { ... }
void loop() { ... }
```
* **setup():**
  1. เริ่มต้น Serial Monitor 115200 bps
  2. กำหนดโหมดขาพินทั้งหมด (`pinMode`)
  3. สั่งเริ่มต้น `dht.begin()` และผูกฟังก์ชันปุ่มกดด้วย `button.onPressed(...)`
  4. ตั้งค่า Static IP และเชื่อมต่อ WiFi
  5. เชื่อมต่อ Blynk Server ด้วยคำสั่ง `Blynk.config(auth, "thanin.tak.rmutl.ac.th", port);` และ `Blynk.connect();`
  6. ตั้งเวลาเรียกฟังก์ชัน `sendSensor()` ทุก ๆ 1 วินาที (1000 ms) ด้วย `timer.setInterval(...)`
* **loop():**
  1. `Blynk.run()`: รักษาการเชื่อมต่อและรับส่งข้อมูลกับเซิร์ฟเวอร์ Blynk
  2. `timer.run()`: ตรวจสอบและประมวลผลงานของตัวจับเวลา BlynkTimer
  3. `button1.read()` ถึง `button4.read()`: ตรวจสอบสถานะการกดของปุ่มกดอย่างต่อเนื่องและแม่นยำ

---

## 📄 ใบอนุญาต (License)

โปรเจกต์นี้เผยแพร่ภายใต้สัญญาอนุญาต [MIT License](LICENSE)
