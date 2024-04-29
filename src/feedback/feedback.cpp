#include "feedback.h"

void Feedback::loop() {
    unlockLed.loop();
    lockLed.loop();
    buzzer.loop();
}
