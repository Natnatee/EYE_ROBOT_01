#include "wifi_module.h"
#include <WiFi.h>
#include <time.h>
#include "display.h"
#include <WiFiManager.h>

// NTP Server settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7 * 3600; // GMT+7 for Thailand
const int   daylightOffset_sec = 0;

static char timeString[16] = "--:--";
static char statusString[16] = "DISCONNECTED";
static unsigned long lastTimeCheck = 0;
static unsigned long lastWifiCheck = 0;
static bool timeSynced = false;
static int cachedHour = -1;
static int cachedMinute = -1;
static char currSsidBuf[32] = "";

static WiFiManager wm;
static bool portalActive = false;

void wifi_start_ap_portal() {
  oled.clearBuffer();
  oled.setFont(u8g2_font_helvB08_tr);
  oled.drawStr(5, 15, "WIFI SETUP MODE");
  oled.setFont(u8g2_font_5x7_tr);
  oled.drawStr(5, 30, "Connect to:");
  oled.drawStr(5, 42, "EYE_ROBOT_WIFI");
  oled.drawStr(5, 54, "URL: 192.168.4.1");
  oled.sendBuffer();

  // ทำให้ไม่บล็อก loop 
  wm.setConfigPortalBlocking(false);
  wm.startConfigPortal("EYE_ROBOT_WIFI");
  
  // ในโหมด Non-blocking ข้ามการเช็คคืนค่า เพราะมันจะ return false เสมอถ้ายังต่อไม่สำเร็จ แต่มันเปิด AP ไปแล้ว
  portalActive = true;
  Serial.println("AP Portal started in background!");
}

void wifi_stop_ap_portal() {
  if (portalActive) {
    wm.stopConfigPortal();
    portalActive = false;
    // พยายามกลับไปต่อ WiFi เดิมเผื่อมีข้อมูลอยู่
    WiFi.begin();
  }
}

bool wifi_is_portal_active() {
  return portalActive;
}

void wifi_init() {
  WiFi.mode(WIFI_STA);
  // อ่านค่าจาก Flash ภายใน
  WiFi.begin(); 
  
  String savedSsid = WiFi.SSID();
  if (savedSsid == "") savedSsid = "No Saved WiFi";
  strcpy(currSsidBuf, savedSsid.c_str());

  Serial.print("Connecting to WiFi: ");
  Serial.println(savedSsid);
  
  display_show_wifi_status(currSsidBuf, "Connecting...");
  
  unsigned long wifiStart = millis();
  const unsigned long WIFI_TIMEOUT = 10000; // 10 วินาที
  
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - wifiStart >= WIFI_TIMEOUT) {
      Serial.println("\nWiFi Timeout! Skipping...");
      display_show_wifi_status(currSsidBuf, "TIMEOUT - SKIP");
      delay(1000);
      strcpy(statusString, "NO WIFI");
      return;
    }
    
    Serial.print(".");
    delay(100);
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  display_show_wifi_status(currSsidBuf, "CONNECTED!");
  delay(1000); 

  // Initialize NTP time
  display_show_wifi_status(currSsidBuf, "SYNC TIME...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  struct tm timeinfo;
  int retry = 0;
  while (!getLocalTime(&timeinfo) && retry < 10) {
    Serial.print(".");
    delay(500);
    retry++;
  }
  
  if (retry < 10) {
    Serial.println("\nTime Synced!");
    strcpy(statusString, "WIFI OK");
    timeSynced = true;
    cachedHour = timeinfo.tm_hour;
    cachedMinute = timeinfo.tm_min;
  } else {
    Serial.println("\nTime Sync Failed!");
    strcpy(statusString, "TIME FAILED");
  }
}

void wifi_update() {
  if (portalActive) {
    wm.process(); // ประมวลผล web server
    return;
  }
  
  unsigned long currentMillis = millis();

  // Check WiFi status every 5 seconds
  if (currentMillis - lastWifiCheck >= 5000) {
    lastWifiCheck = currentMillis;
    if (WiFi.status() != WL_CONNECTED) {
      strcpy(statusString, "NO WIFI");
    } else {
      strcpy(statusString, "WIFI OK");
      if (!timeSynced) {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      }
    }
  }

  // Update time cache every 1 second
  if (currentMillis - lastTimeCheck >= 1000) {
    lastTimeCheck = currentMillis;
    
    // เฉพาะตอน WiFi ต่อแล้วถึงจะลองดึงเวลา
    if (WiFi.status() == WL_CONNECTED) {
      struct tm timeinfo;
      // timeout 10ms ไม่ให้บล็อก loop
      if (getLocalTime(&timeinfo, 10)) {
        sprintf(timeString, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
        cachedHour = timeinfo.tm_hour;
        cachedMinute = timeinfo.tm_min;
        timeSynced = true;
      }
    }
  }
}

bool wifi_is_connected() {
  return WiFi.status() == WL_CONNECTED;
}

const char* wifi_get_time_string() {
  return timeString;
}

const char* wifi_get_status_string() {
  return statusString;
}

int wifi_get_hour() {
  return cachedHour;
}

int wifi_get_minute() {
  return cachedMinute;
}

const char* wifi_get_ssid() {
  if (WiFi.status() == WL_CONNECTED) {
    String s = WiFi.SSID();
    if (s.length() > 0) {
      strncpy(currSsidBuf, s.c_str(), sizeof(currSsidBuf)-1);
    }
  }
  return currSsidBuf;
}
