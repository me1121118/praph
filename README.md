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

## 📄 ใบอนุญาต (License)

โปรเจกต์นี้เผยแพร่ภายใต้สัญญาอนุญาต [MIT License](LICENSE)
