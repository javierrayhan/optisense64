#include "pixel_read.h"

int mux[8][3] = {
    {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, 
    {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}
};

int result[64];          // Stores the 8x8 matrix readings
uint8_t result8bit[64];  // Stores the converted 8x8 matrix value
float ans;

void setupPixel(){
  pinMode(C1, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(A2, OUTPUT);

  // Optional: initialize all to LOW
  digitalWrite(C1, LOW);
  digitalWrite(B1, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(C2, LOW);
  digitalWrite(B2, LOW);
  digitalWrite(A2, LOW);
}

void readValue(const int analogPin){
  int iter = 0;

  Serial.println("Reading matrix...\n");

  for (int row = 0; row < 8; row++) {
    // Activate (MUX1 as output selector)
    digitalWrite(C1, mux[row][0]);
    digitalWrite(B1, mux[row][1]);
    digitalWrite(A1, mux[row][2]);

    for (int col = 0; col < 8; col++) {
      // Activate COLUMN (MUX2 as input selector)
      digitalWrite(C2, mux[col][0]);
      digitalWrite(B2, mux[col][1]);
      digitalWrite(A2, mux[col][2]);

      delayMicroseconds(300); // Mux stabilize mu
      
      // Read Analog
      ans = analogRead(analogPin);
      result[iter] = ans;

      // Debug log
      Serial.print("Row ");
      Serial.print(row + 1);
      Serial.print(", Col ");
      Serial.print(col + 1);
      Serial.print(" => ");
      Serial.println(ans);

      iter++;
    }
  }

  // Convert 12bit to 8bit value
  convertTo8bit();

  // Print result
  resultPrint(true);
}

void convertTo8bit() {
  for (int i = 0; i < 64; i++) {
    float scaled = (result[i] / 4095.0) * 255.0;
    Serial.printf("raw: %.2f, scaled: %.2f -> %d\n", result[i], scaled, (int)round(scaled));
    result8bit[i] = (uint8_t) round(scaled);
  }
}

void resultPrint(bool use8bit) {
  Serial.println(use8bit ? "\n-------- Matrix Output (8x8) [8-bit] --------"
                         : "\n-------- Matrix Output (8x8) [RAW 12-bit] --------");

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      int index = row * 8 + col;

      if (use8bit) {
        Serial.print(result8bit[index]);
      } else {
        Serial.print(result[index]);
      }

      Serial.print("\t");
    }
    Serial.println();
  }

  Serial.println("\nDone!\n");
}
