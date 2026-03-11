---
description: Build, Upload, and Monitor logs in one command
---

## Steps
1. ตรวจสอบว่าต่อสาย USB-C เข้าบอร์ดแล้ว
2. รันคำสั่ง:
```bash
cd c:/MCU/ESP32/ESP32_C3/EYE_ROBOT_01 && pio run -t upload -t monitor
```
3. หากสำเร็จ โปรแกรมจะเริ่มรันและโชว์ Serial Monitor ทันที
4. กด `Ctrl+C` เพื่อหยุด Monitor
