#pragma once

#include <Arduino.h>
#include <WiFi.h>

// ============================================
// WiFi Configuration
// ============================================
#define WIFI_SSID       "Digitalnatives3"
#define WIFI_PASSWORD   "BC202$1$9@"
#define WIFI_TIMEOUT_MS 15000  // Timeout 15 วินาที

// ============================================
// LED Pin (ESP32-C3 Super Mini built-in LED)
// ============================================
#define LED_PIN         8  // GPIO8 = Built-in LED (Active LOW)

// ============================================
// Function Declarations
// ============================================
void wifi_init();
void wifi_update();
bool wifi_is_connected();
void wifi_print_status();
