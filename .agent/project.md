# 📋 Project: ESP32-C3 WiFi Test

## 1. Project Overview
โปรเจกต์ทดสอบการเชื่อมต่อ WiFi ของบอร์ด ESP32-C3 Super Mini
เป้าหมาย: ตรวจสอบว่าบอร์ดสามารถเชื่อมต่อ WiFi ได้ถูกต้อง พร้อมแสดงข้อมูลสถานะผ่าน Serial Monitor

## 2. Hardware Specs
- **Board:** ESP32-C3 Super Mini
- **MCU:** ESP32-C3 (RISC-V single core, 160MHz)
- **Flash:** 4MB
- **SRAM:** 400KB
- **WiFi:** 2.4GHz 802.11 b/g/n
- **Bluetooth:** BLE 5.0
- **USB:** USB-C (Native USB)
- **LED:** GPIO8 (Built-in, Active LOW)

### ⚠️ Hardware Issues
- USB-C ต้องใช้สายที่รองรับ Data (ไม่ใช่สายชาร์จอย่างเดียว)
- GPIO8 = Built-in LED (Active LOW: LOW=เปิด, HIGH=ปิด)

## 3. Pin Mapping Table
| Pin   | Function    | Device       | Direction | Note              |
|-------|-------------|--------------|-----------|-------------------|
| GPIO8 | LED         | Built-in LED | Output    | Active LOW        |
| USB   | Serial/Upload | USB-C      | I/O       | Native USB        |

## 4. Library Specs
- **WiFi** (Built-in ESP32 Arduino Core)

## 5. Config & Settings
- **Baud Rate:** 115200
- **WiFi SSID:** DigitalNatives3
- **WiFi Password:** BC202$1$9@
- **WiFi Timeout:** 15 วินาที

## 6. Current State
- ✅ สร้างโปรเจกต์เริ่มต้นเรียบร้อย
- ⏳ รอทดสอบอัปโหลดและเชื่อมต่อ WiFi

## 7. Issues & Solutions Log
_(ยังไม่มีบันทึก)_
