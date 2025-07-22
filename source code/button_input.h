#ifndef BUTTON_INPUT_H
#define BUTTON_INPUT_H

#include <Arduino.h>

void initButton(uint8_t pin);
void updateButton();

bool isButtonPressedShort();   // < 1s
bool isButtonPressedMedium();  // >= 1s && < 2s
bool isButtonPressedLong();    // >= 2s

#endif
