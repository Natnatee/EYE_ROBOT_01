#include <Arduino.h>
#include "wifi_module.h"

void setup() {
    Serial.begin(115200);
    delay(2000); // รอ Serial พร้อม

    wifi_init();
}

void loop() {
    wifi_update();
}
