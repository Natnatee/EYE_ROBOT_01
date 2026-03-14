#include "display_module.h"

// เปลี่ยนเป็น SH1106 สำหรับจอ 1.3" (SSD1306 จะใช้กับจอ 0.96")
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ OLED_SCL, /* data=*/ OLED_SDA);

void display_init() {
    Serial.println("\n[I2C Scanner] เริ่มต้นการค้นหาอุปกรณ์...");
    Serial.printf("[I2C Scanner] ขาที่ใช้: SDA=%d, SCL=%d\n", OLED_SDA, OLED_SCL);
    
    Wire.begin(OLED_SDA, OLED_SCL);
    
    byte error, address;
    int nDevices = 0;

    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.printf("[I2C Scanner] ✅ พบอุปกรณ์ที่ Address: 0x%02X\n", address);
            nDevices++;
        }
        else if (error == 4) {
            Serial.printf("[I2C Scanner] ❓ พบ Error ที่ Address: 0x%02X\n", address);
        }
    }

    if (nDevices == 0) {
        Serial.println("[I2C Scanner] ❌ ไม่พบอุปกรณ์ I2C เลยสักชิ้น!");
    } else {
        Serial.printf("[I2C Scanner] ค้นหาเสร็จสิ้น พบทั้งหมด %d เครื่อง\n", nDevices);
    }

    Serial.println("[Display] กำลังเริ่มต้นจอ SH1106...");
    if (u8g2.begin()) {
        Serial.println("[Display] ✅ คำสั่ง u8g2.begin() สำเร็จ!");
    } else {
        Serial.println("[Display] ❌ u8g2.begin() ล้มเหลว!");
    }
    
    u8g2.setContrast(255);
}

void display_clear() {
    u8g2.clearBuffer();
}

void display_update() {
    u8g2.sendBuffer();
}
