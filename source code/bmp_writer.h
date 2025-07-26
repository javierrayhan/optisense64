#ifndef BMP_WRITER_H
#define BMP_WRITER_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

// Setup
bool setupSPI_SD();

// Filename system
String generateBMPFilename();

// BMP Writer
void writeBMP(File file, const uint8_t* data, int width, int height);
bool saveBMPToSD(const uint8_t* data, int width, int height);

#endif
