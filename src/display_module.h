#pragma once

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// I2C Pins (ย้ายเป็นขา 21, 10 ตามที่เคยใช้งานได้)
#define OLED_SDA 10
#define OLED_SCL 21

// Display instance (ใช้ Full Buffer Mode เพื่อการกระพริบที่น้อยที่สุด)
// Note: ถ้าจอ 1.3" แล้วภาพเพี้ยน ให้เปลี่ยน SSD1306 เป็น SH1106
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

void display_init();
void display_clear();
void display_update();
