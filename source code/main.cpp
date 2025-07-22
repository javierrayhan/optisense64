#include <Arduino.h>

// Include liblary
#include "button_input.h"
#include "pixel_read.h"
#include "bmp_writer.h"

const int analogPin = 36;   // ADC1_CHANNEL_0
const int shutterPin = 32;  // SHUTTER PIN 32

void setup() {
  Serial.begin(115200);             // Serial init
  setupPixel();                     // Pixel pin init

  analogSetAttenuation(ADC_11db);   // Analog 3.3v

  initButton(shutterPin);           // Shutter pin init

  setupSPI_SD();                    // SPI sdcard init
}

void loop() {
  updateButton();

  if(isButtonPressedShort()){
    readValue(analogPin);
  } 
  else if (isButtonPressedMedium()) {
    
  } 
  else if (isButtonPressedLong()) {
    
  }

}
