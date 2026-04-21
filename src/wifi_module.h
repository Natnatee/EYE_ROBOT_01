#pragma once
#include <Arduino.h>

const char* wifi_get_ssid();

void wifi_init();
void wifi_start_ap_portal();
void wifi_stop_ap_portal();
bool wifi_is_portal_active();
void wifi_update();
bool wifi_is_connected();
const char* wifi_get_time_string();
const char* wifi_get_status_string();
int wifi_get_hour();    // -1 ถ้ายังไม่ได้ sync เวลา
int wifi_get_minute();  // -1 ถ้ายังไม่ได้ sync เวลา
