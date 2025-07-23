#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

// Include liblary
#include "button_input.h"
#include "pixel_read.h"
#include "bmp_writer.h"

const int analogPin = 36;   // ADC1_CHANNEL_0
const int shutterPin = 32;  // SHUTTER PIN 32
const int RGBled = 27;      // WS2812B LED PIN 27

void colorWipe(uint32_t color, int wait);

Adafruit_NeoPixel strip(1, RGBled, NEO_GRB + NEO_KHZ800);

void setup() {
  // Serial init
  Serial.begin(115200);             

  // WS2812B init
  strip.begin();
  strip.show();
  strip.setBrightness(50);
  colorWipe(strip.Color(0, 255, 0), 100);
  colorWipe(strip.Color(0, 0, 0), 50);

  // Pixel pin init
  setupPixel();                     
  colorWipe(strip.Color(0, 255, 0), 100);
  colorWipe(strip.Color(0, 0, 0), 50);

  // Analog 3.3v
  analogSetAttenuation(ADC_11db);
  colorWipe(strip.Color(0, 255, 0), 100);
  colorWipe(strip.Color(0, 0, 0), 50);

  // Shutter pin init
  initButton(shutterPin);           
  colorWipe(strip.Color(0, 255, 0), 100);
  colorWipe(strip.Color(0, 0, 0), 50);

   // SPI sdcard init
  if (setupSPI_SD()){
    colorWipe(strip.Color(0, 255, 0), 100);
  }

  else if (!setupSPI_SD()){
    colorWipe(strip.Color(255, 0, 0), 100);
  }    

  colorWipe(strip.Color(0, 0, 0), 50);

}

void loop() {
  updateButton();

  if(isButtonPressedShort()){
    readValue(analogPin);

  } 

  else if (isButtonPressedMedium()) {
    readValue(analogPin);

    if(saveBMPToSD(result8bit, 8, 8)){
      colorWipe(strip.Color(0, 255, 0), 100);
      colorWipe(strip.Color(0, 0, 0), 50);
    }
    else if (!saveBMPToSD(result8bit, 8, 8)){
      colorWipe(strip.Color(255, 0, 0), 100);
      colorWipe(strip.Color(0, 0, 0), 50);
    }
  } 

  else if (isButtonPressedLong()) {
    
  }

}

void colorWipe(uint32_t color, int wait) {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

