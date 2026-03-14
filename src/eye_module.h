#pragma once

#include <Arduino.h>

enum EyeEmotion {
    EMOTION_NORMAL,
    EMOTION_HAPPY,
    EMOTION_ANGRY,
    EMOTION_SLEEPY,
    EMOTION_CRAZY
};

void eye_init();
void eye_draw(EyeEmotion emotion);
void eye_animate(); // สำหรับอนิเมชั่นลื่นๆ (ถ้าต้องการ)
