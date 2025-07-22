#include "bmp_writer.h"

#define SD_CS 26
#define MOSI 13
#define MISO 12
#define SCK 14

void setupSPI_SD() {
  SPI.begin(SCK, MISO, MOSI, SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("SD init failed!");
    return;
  }
  Serial.println("SD ready!");
}

String generateBMPFilename() {
  int index = 1;
  String filename;

  do {
    filename = "/EBF" + String(index) + ".BMP";
    index++;
  } while (SD.exists(filename));

  return filename;
}

void writeBMP(File file, const uint8_t* data, int width, int height) {
  const int headerSize = 54;
  const int paletteSize = 1024;
  const int pixelDataSize = width * height;
  const int fileSize = headerSize + paletteSize + pixelDataSize;

  uint8_t bmpHeader[54] = {
    'B','M',
    (uint8_t)(fileSize), (uint8_t)(fileSize >> 8), (uint8_t)(fileSize >> 16), (uint8_t)(fileSize >> 24),
    0,0, 0,0,
    (uint8_t)(headerSize + paletteSize),0,0,0,
    40,0,0,0,
    (uint8_t)(width), (uint8_t)(width >> 8), 0,0,
    (uint8_t)(height), (uint8_t)(height >> 8), 0,0,
    1,0,
    8,0,
    0,0,0,0,
    0,0,0,0,
    0x13,0x0B,0,0,
    0x13,0x0B,0,0,
    255,0,0,0,
    0,0,0,0
  };

  file.write(bmpHeader, 54);

  // Palette: grayscale
  for (int i = 0; i < 256; i++) {
    file.write(i); file.write(i); file.write(i); file.write(0);
  }

  // Pixel data (bottom-up BMP format)
  for (int row = height - 1; row >= 0; row--) {
    for (int col = 0; col < width; col++) {
      file.write(data[row * width + col]);
    }
  }
}

void saveBMPToSD(const uint8_t* data, int width, int height) {
  String filename = generateBMPFilename();
  File bmpFile = SD.open(filename, FILE_WRITE);
  if (!bmpFile) {
    Serial.println("Failed to open file: " + filename);
    return;
  }

  writeBMP(bmpFile, data, width, height);
  bmpFile.close();
  Serial.println("Saved: " + filename);
}
