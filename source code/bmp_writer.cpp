#include "bmp_writer.h"

// #define SD_CS 26
// #define MOSI 13
// #define MISO 12
// #define SCK 14

#define SD_CS 13
#define MOSI 23
#define MISO 19
#define SCK 18

bool setupSPI_SD() {
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  delay(10); 

  // SPI Start
  SPI.begin(SCK, MISO, MOSI, SD_CS);
  delay(100); 
  
  // Init SD card 
  if (!SD.begin(SD_CS)) {
    Serial.println("[ERROR] SD init failed!");
    return false;
  }

  // Debug
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("[ERROR] No SD card detected.");
    return false;
  } else {
    Serial.print("[INFO] SD card type: ");
    switch (cardType) {
      case CARD_MMC: Serial.println("MMC"); break;
      case CARD_SD: Serial.println("SDSC"); break;
      case CARD_SDHC: Serial.println("SDHC"); break;
      default: Serial.println("Unknown");
    }
  }

  Serial.println("[SUCCESS] SD ready!");
  return true;
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
  const int paletteSize = 256 * 4;
  const int rowSize = ((width + 3) / 4) * 4;  // Pad to multiple of 4 bytes
  const int pixelDataSize = rowSize * height;
  const int fileSize = headerSize + paletteSize + pixelDataSize;

  uint8_t bmpHeader[54] = {
    'B','M',
    (uint8_t)(fileSize), (uint8_t)(fileSize >> 8), (uint8_t)(fileSize >> 16), (uint8_t)(fileSize >> 24),
    0,0, 0,0,
    (uint8_t)(headerSize + paletteSize), (uint8_t)((headerSize + paletteSize) >> 8),
    (uint8_t)((headerSize + paletteSize) >> 16), (uint8_t)((headerSize + paletteSize) >> 24),
    40,0,0,0,
    (uint8_t)(width), (uint8_t)(width >> 8), (uint8_t)(width >> 16), (uint8_t)(width >> 24),
    (uint8_t)(height), (uint8_t)(height >> 8), (uint8_t)(height >> 16), (uint8_t)(height >> 24),
    1,0, // planes
    8,0, // bits per pixel
    0,0,0,0, // compression
    (uint8_t)(pixelDataSize), (uint8_t)(pixelDataSize >> 8),
    (uint8_t)(pixelDataSize >> 16), (uint8_t)(pixelDataSize >> 24),
    0x13,0x0B,0,0, // X pixels per meter (72 DPI)
    0x13,0x0B,0,0, // Y pixels per meter (72 DPI)
    0,1,0,0,       // colors used (set to 256 explicitly!)
    0,0,0,0        // important colors
  };

  file.write(bmpHeader, sizeof(bmpHeader));

  // Grayscale palette
  for (int i = 0; i < 256; i++) {
    file.write(i); // Blue
    file.write(i); // Green
    file.write(i); // Red
    file.write(0); // Reserved
  }

  // Bottom-up pixel data
  for (int row = height - 1; row >= 0; row--) {
    file.write(&data[row * width], width);  
    for (int p = 0; p < rowSize - width; p++) {
      file.write((uint8_t)0); // padding
    }
  }
}


bool saveBMPToSD(const uint8_t* data, int width, int height) {
  String filename = generateBMPFilename();
  File bmpFile = SD.open(filename, FILE_WRITE);
  if (!bmpFile) {
    Serial.println("Failed to open file: " + filename);
    return false;
  }

  writeBMP(bmpFile, data, width, height);
  bmpFile.close();
  Serial.println("Saved: " + filename);
  return true;
}
