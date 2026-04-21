#include "clock_mode.h"
#include "display.h"
#include "wifi_module.h"

enum ClockPage {
  PAGE_TIME = 0,
  PAGE_INFO,
  PAGE_CHANGE_WIFI,
  PAGE_AP_PORTAL
};

static ClockPage currentPage = PAGE_TIME;
static unsigned long pageEnterTime = 0;

void clock_mode_init() {
  currentPage = PAGE_TIME;
}

void clock_mode_toggle_info() {
  if (currentPage == PAGE_AP_PORTAL) {
    wifi_stop_ap_portal();
    currentPage = PAGE_TIME;
    return;
  }

  if (currentPage == PAGE_TIME) {
    currentPage = PAGE_INFO;
  } else if (currentPage == PAGE_INFO) {
    currentPage = PAGE_CHANGE_WIFI;
    pageEnterTime = millis();
  } else if (currentPage == PAGE_CHANGE_WIFI) {
    currentPage = PAGE_TIME;
  }
}

void clock_mode_update() {
  if (currentPage == PAGE_AP_PORTAL) {
    wifi_update(); 
    // ยอมให้วาดหน้าจอซ้ำได้ เพราะโหมด non-blocking จะต้องให้หน้าจอยังคงค้างไว้
    // เราจะวาดหน้าจออีกครั้ง
  }

  oled.clearBuffer();

  if (currentPage == PAGE_INFO) {
    // === หน้า 2: แสดง WiFi Info ===
    
    // หัวข้อ
    oled.setFont(u8g2_font_helvB08_tr);
    const char* title = "WIFI INFO";
    int tw = oled.getStrWidth(title);
    oled.drawStr((SCREEN_W - tw) / 2, 12, title);
    oled.drawHLine(5, 15, SCREEN_W - 10);

    // ชื่อ WiFi
    oled.setFont(u8g2_font_6x12_tr);
    oled.drawStr(4, 30, "SSID:");
    oled.setFont(u8g2_font_5x7_tr);
    oled.drawStr(36, 30, wifi_get_ssid());

    // รหัสผ่าน
    oled.setFont(u8g2_font_6x12_tr);
    oled.drawStr(4, 44, "PASS:");
    oled.setFont(u8g2_font_5x7_tr);
    
    // โชว์ดอกจันถ้ารหัสผ่านยาวไป หรือโชว์ตรงๆ ก็ได้
    oled.drawStr(36, 44, "********");

    // สถานะ เชื่อมต่อ / กำลังเชื่อมต่อ
    oled.setFont(u8g2_font_5x7_tr);
    const char* st = wifi_is_connected() ? "CONNECTED" : "CONNECTING...";
    int sw = oled.getStrWidth(st);
    oled.drawStr((SCREEN_W - sw) / 2, 60, st);

  } else if (currentPage == PAGE_CHANGE_WIFI) {
    // === หน้า 3: นับถอยหลังเปลี่ยน WiFi ===
    unsigned long elapsed = millis() - pageEnterTime;
    int timeLeft = 3 - (elapsed / 1000);
    
    if (timeLeft <= 0) {
      currentPage = PAGE_AP_PORTAL;
      wifi_start_ap_portal();
      return;
    }

    oled.setFont(u8g2_font_helvB08_tr);
    const char* t = "CHANGE WIFI?";
    int tw = oled.getStrWidth(t);
    oled.drawStr((SCREEN_W - tw)/2, 20, t);

    oled.setFont(u8g2_font_logisoso24_tf);
    char buf[10];
    sprintf(buf, "%d", timeLeft);
    int bw = oled.getStrWidth(buf);
    oled.drawStr((SCREEN_W - bw)/2, 50, buf);
    
    oled.setFont(u8g2_font_5x7_tr);
    const char* hint = "TAP to Cancel";
    int hw = oled.getStrWidth(hint);
    oled.drawStr((SCREEN_W - hw)/2, 62, hint);

  } else if (currentPage == PAGE_AP_PORTAL) {
    // === หน้า 4: กำลังแชร์ WiFi ===
    oled.setFont(u8g2_font_helvB08_tr);
    oled.drawStr(5, 15, "WIFI SETUP MODE");
    oled.setFont(u8g2_font_5x7_tr);
    oled.drawStr(5, 30, "Connect to:");
    oled.drawStr(5, 42, "EYE_ROBOT_WIFI");
    oled.drawStr(5, 54, "URL: 192.168.4.1");
    
    const char* hint = "TAP to Cancel";
    int hw = oled.getStrWidth(hint);
    oled.drawStr((SCREEN_W - hw)/2, 64, hint);
  } else {
    // === หน้า 1: หน้านาฬิกาปกติ ===

    // เวลาตัวใหญ่กลางจอ
    oled.setFont(u8g2_font_logisoso24_tf);
    const char* timeStr = wifi_get_time_string();
    int tw = oled.getStrWidth(timeStr);
    oled.drawStr((SCREEN_W - tw) / 2, 38, timeStr);

    // สถานะ WiFi ด้านล่าง (ON / OFF)
    oled.setFont(u8g2_font_5x7_tr);
    const char* wifiStatus = wifi_is_connected() ? "WIFI ON" : "WIFI OFF";
    int sw = oled.getStrWidth(wifiStatus);
    oled.drawStr((SCREEN_W - sw) / 2, 52, wifiStatus);

    // คำใบ้กดปุ่ม
    oled.setFont(u8g2_font_4x6_tf);
    const char* hint = "TAP to change display";
    int hw = oled.getStrWidth(hint);
    oled.drawStr((SCREEN_W - hw) / 2, 62, hint);
  }

  oled.sendBuffer();
}

