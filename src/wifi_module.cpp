#include "wifi_module.h"

// ============================================
// Internal Variables
// ============================================
static unsigned long _last_check_ms = 0;
static const unsigned long CHECK_INTERVAL_MS = 5000; // เช็คสถานะทุก 5 วินาที
static bool _was_connected = false;

// ============================================
// WiFi Event Callback
// ============================================
static void _wifi_event_handler(WiFiEvent_t event) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.println("[WiFi] ✅ เชื่อมต่อ AP สำเร็จ!");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.print("[WiFi] 📡 ได้รับ IP: ");
            Serial.println(WiFi.localIP());
            Serial.print("[WiFi] 📶 RSSI: ");
            Serial.print(WiFi.RSSI());
            Serial.println(" dBm");
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("[WiFi] ❌ ขาดการเชื่อมต่อ! กำลังลองใหม่...");
            WiFi.reconnect();
            break;
        default:
            break;
    }
}

// ============================================
// WiFi Init
// ============================================
void wifi_init() {
    Serial.println("========================================");
    Serial.println("   ESP32-C3 WiFi Test");
    Serial.println("========================================");
    Serial.print("[WiFi] SSID: ");
    Serial.println(WIFI_SSID);
    Serial.println("[WiFi] กำลังเชื่อมต่อ...");

    // ตั้งค่า LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH); // ปิด LED (Active LOW)

    // ลงทะเบียน Event
    WiFi.onEvent(_wifi_event_handler);

    // เริ่มเชื่อมต่อ
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // รอเชื่อมต่อ (blocking แค่ตอน init)
    unsigned long start_ms = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start_ms > WIFI_TIMEOUT_MS) {
            Serial.println("[WiFi] ⏰ TIMEOUT! ไม่สามารถเชื่อมต่อได้");
            Serial.println("[WiFi] ตรวจสอบ SSID/Password แล้วลองใหม่");
            return;
        }
        digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // กะพริบ LED ระหว่างรอ
        delay(250);
        Serial.print(".");
    }
    Serial.println();

    // เชื่อมต่อสำเร็จ - เปิด LED ค้าง
    digitalWrite(LED_PIN, LOW); // เปิด LED (Active LOW)
    _was_connected = true;

    // แสดงข้อมูลเต็ม
    wifi_print_status();
}

// ============================================
// WiFi Update (เรียกใน loop)
// ============================================
void wifi_update() {
    unsigned long now = millis();
    if (now - _last_check_ms < CHECK_INTERVAL_MS) return;
    _last_check_ms = now;

    bool connected = wifi_is_connected();

    // ตรวจจับการเปลี่ยนสถานะ
    if (connected && !_was_connected) {
        Serial.println("[WiFi] 🔄 กลับมาเชื่อมต่อได้แล้ว!");
        digitalWrite(LED_PIN, LOW); // เปิด LED
        wifi_print_status();
    } else if (!connected && _was_connected) {
        Serial.println("[WiFi] ⚠️ สูญเสียการเชื่อมต่อ!");
        digitalWrite(LED_PIN, HIGH); // ปิด LED
    } else if (connected) {
        // ถ้าเชื่อมต่ออยู่ปกติ ให้แสดง RSSI ทุก 10 วินาที (2 รอบของ CHECK_INTERVAL)
        static int _print_count = 0;
        if (++_print_count >= 2) { 
            Serial.print("[WiFi] 📶 Signal Strength: ");
            Serial.print(WiFi.RSSI());
            Serial.println(" dBm (ยังเชื่อมต่อปกติ)");
            _print_count = 0;
        }
    }

    _was_connected = connected;

    // กะพริบ LED ถ้ายังไม่เชื่อมต่อ
    if (!connected) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
}

// ============================================
// Check Connection
// ============================================
bool wifi_is_connected() {
    return WiFi.status() == WL_CONNECTED;
}

// ============================================
// Print Full Status
// ============================================
void wifi_print_status() {
    Serial.println("----------------------------------------");
    Serial.println("[WiFi] 📊 สถานะการเชื่อมต่อ:");
    Serial.print("  SSID     : ");
    Serial.println(WiFi.SSID());
    Serial.print("  IP       : ");
    Serial.println(WiFi.localIP());
    Serial.print("  Gateway  : ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("  Subnet   : ");
    Serial.println(WiFi.subnetMask());
    Serial.print("  DNS      : ");
    Serial.println(WiFi.dnsIP());
    Serial.print("  MAC      : ");
    Serial.println(WiFi.macAddress());
    Serial.print("  RSSI     : ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.print("  Channel  : ");
    Serial.println(WiFi.channel());
    Serial.println("----------------------------------------");
}
