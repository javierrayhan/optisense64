#include "button_input.h"

static uint8_t buttonPin;
static bool lastState = HIGH;
static unsigned long pressStartTime = 0;
static bool isPressed = false;
static unsigned long pressDuration = 0;
static bool wasReleased = false;

void initButton(uint8_t pin) {
    buttonPin = pin;
    pinMode(buttonPin, INPUT_PULLUP); // default: HIGH
}

void updateButton() {
    bool currentState = digitalRead(buttonPin); // LOW = ditekan, HIGH = lepas

    if (lastState == HIGH && currentState == LOW) {
        // Tombol baru saja ditekan
        pressStartTime = millis();
        isPressed = true;
        wasReleased = false;
    } 
    else if (lastState == LOW && currentState == HIGH && isPressed) {
        // Tombol baru saja dilepas
        pressDuration = millis() - pressStartTime;
        isPressed = false;
        wasReleased = true;
    }

    lastState = currentState;
}

bool isButtonPressedShort() {
    if (wasReleased && pressDuration < 1000) {
        wasReleased = false;
        return true;
    }
    return false;
}

bool isButtonPressedMedium() {
    if (wasReleased && pressDuration >= 1000 && pressDuration < 2000) {
        wasReleased = false;
        return true;
    }
    return false;
}

bool isButtonPressedLong() {
    if (wasReleased && pressDuration >= 2000) {
        wasReleased = false;
        return true;
    }
    return false;
}
