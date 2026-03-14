#include "eye_module.h"
#include "display_module.h"

// ค่าคงที่สำหรับตำแหน่งดวงตา
const int EYE_Y = 32;       // กึ่งกลางแนวตั้ง
const int EYE_L_X = 40;     // ตาซ้าย
const int EYE_R_X = 88;     // ตาขวา
const int EYE_WIDTH = 25;   // ความกว้างตา
const int EYE_HEIGHT = 35;  // ความสูงตา

void _draw_eye_shape(int x, int y, int w, int h, int r, EyeEmotion emotion, bool isLeft) {
    switch (emotion) {
        case EMOTION_NORMAL:
            u8g2.drawRBox(x - w/2, y - h/2, w, h, r);
            break;
            
        case EMOTION_HAPPY:
            // ตาโค้งยิ้ม (วาดกล่องทับครึ่งล่าง)
            u8g2.drawRBox(x - w/2, y - h/2, w, h, r);
            u8g2.setDrawColor(0); // สีดำ (ลบออก)
            u8g2.drawBox(x - w/2 - 2, y, w + 4, h); // ตัดครึ่งล่าง
            u8g2.setDrawColor(1); // กลับเป็นสีขาวยังเดิม
            break;
            
        case EMOTION_ANGRY:
            // ตาเฉียง (วาดกล่องแล้วตัดเฉียง)
            u8g2.drawRBox(x - w/2, y - h/2, w, h, r);
            u8g2.setDrawColor(0);
            if (isLeft) {
                u8g2.drawTriangle(x - w/2 - 2, y - h/2 - 2, x + w/2 + 2, y - h/2 - 2, x + w/2 + 2, y - h/4);
            } else {
                u8g2.drawTriangle(x + w/2 + 2, y - h/2 - 2, x - w/2 - 2, y - h/2 - 2, x - w/2 - 2, y - h/4);
            }
            u8g2.setDrawColor(1);
            break;
            
        case EMOTION_SLEEPY:
            // ตาปรือ (แบนลง)
            u8g2.drawRBox(x - w/2, y, w, 8, 2);
            break;
            
        case EMOTION_CRAZY:
            // ตาโตไม่เท่ากัน
            if (isLeft) u8g2.drawDisc(x, y, 15);
            else u8g2.drawRBox(x - 5, y - 5, 10, 10, 2);
            break;
    }
}

void eye_init() {
    // โค้ดเตรียมการดวงตา (ถ้ามี)
}

void eye_draw(EyeEmotion emotion) {
    display_clear();
    
    // วาดตาซ้าย
    _draw_eye_shape(EYE_L_X, EYE_Y, EYE_WIDTH, EYE_HEIGHT, 6, emotion, true);
    // วาดตาขวา
    _draw_eye_shape(EYE_R_X, EYE_Y, EYE_WIDTH, EYE_HEIGHT, 6, emotion, false);
    
    display_update();
}
