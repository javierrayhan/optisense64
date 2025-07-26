#ifndef PIXEL_READ_H
#define PIXEL_READ_H

#include <Arduino.h>// Function Declaration

void setupPixel();
void readValue(const int analogPin);
void convertTo8bit();
void resultPrint(bool use8bit = false);

// CD4051 MUX channel mapping (binary 3-bit)
extern int mux[8][3];

extern int result[64];          // Stores the 8x8 matrix readings
extern uint8_t result8bit[64];  // Stores the converted 8x8 matrix value
extern float ans;

// Define ESP32 GPIOs for MUX control pins
// #define C1 4    // MUX1 - control pin C1
// #define B1 17   // MUX1 - control pin B1
// #define A1 16   // MUX1 - control pin A1
// #define C2 22   // MUX2 - control pin C2
// #define B2 21   // MUX2 - control pin B2
// #define A2 15   // MUX2 - control pin A2

#define C2 4    // MUX1 - control pin C1
#define B2 17   // MUX1 - control pin B1
#define A2 16   // MUX1 - control pin A1
#define C1 22   // MUX2 - control pin C2
#define B1 21   // MUX2 - control pin B2
#define A1 15   // MUX2 - control pin A2

#endif
