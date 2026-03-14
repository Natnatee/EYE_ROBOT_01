#include <Arduino.h>
#include "display_module.h"
#include "eye_module.h"

unsigned long last_emotion_change = 0;
const unsigned long INTERVAL = 3000; // 3 วินาที
int current_state = 0;

void setup() {
    Serial.begin(115200);
    
    // เริ่มต้นระบบหน้าจอ
    display_init();
    eye_init();
    
    Serial.println("EYE_ROBOT_01: Display System Ready!");
}

void loop() {
    unsigned long now = millis();
    
    if (now - last_emotion_change >= INTERVAL) {
        last_emotion_change = now;
        
        EyeEmotion emotion = (EyeEmotion)current_state;
        
        // แสดงชื่ออารมณ์ใน Serial เพื่อ Debug
        switch(emotion) {
            case EMOTION_NORMAL: Serial.println("Emotion: Normal"); break;
            case EMOTION_HAPPY:  Serial.println("Emotion: Happy"); break;
            case EMOTION_ANGRY:  Serial.println("Emotion: Angry"); break;
            case EMOTION_SLEEPY: Serial.println("Emotion: Sleepy"); break;
            case EMOTION_CRAZY:  Serial.println("Emotion: Crazy"); break;
        }
        
        eye_draw(emotion);
        
        // วนสถานะ 0-4
        current_state = (current_state + 1) % 5;
    }
}
