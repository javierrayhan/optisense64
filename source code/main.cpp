#include <Arduino.h>
// // --- CD4051 Selector Pins
// const int pinA = 16;
// const int pinB = 17;
// const int pinC = 4;

// // --- COM pin (sambung ke Z/CD4051 common pin)
// const int comPin = 36; // GPIO36 = ADC1_CHANNEL_0

// void selectChannel(int channel);

// void setup() {
//   Serial.begin(115200);

//   // Set pin selector sebagai output
//   pinMode(pinA, OUTPUT);
//   pinMode(pinB, OUTPUT);
//   pinMode(pinC, OUTPUT);

//   // Optional: set default state
//   digitalWrite(pinA, LOW);
//   digitalWrite(pinB, LOW);
//   digitalWrite(pinC, LOW);
// }

// void loop() {
//   Serial.print("=====================\n");
//   // Loop semua channel dari Y0–Y7
//   for (int ch = 0; ch < 8; ch++) {
//     selectChannel(ch); // pilih channel

//     delay(10); // delay stabilisasi switching

//     int val = analogRead(comPin); // baca analog dari channel aktif
//     Serial.print("Channel ");
//     Serial.print(ch);
//     Serial.print(": ");
//     Serial.println(val);

//     delay(100); // jeda antar channel
//   }

//   delay(1000); // jeda antar loop penuh
//   Serial.print("\n=====================");
// }

// // --- Fungsi untuk pilih channel 0–7
// void selectChannel(int channel) {
//   digitalWrite(pinA, bitRead(channel, 0));
//   digitalWrite(pinB, bitRead(channel, 1));
//   digitalWrite(pinC, bitRead(channel, 2));
// }




#include <Arduino.h>

// Include liblary
#include "bmp_writer.h"
#include "button_input.h"

// Function Declaration
void resultPrint(bool use8bit = false);
void convertTo8bit();
void readValue();

// CD4051 MUX channel mapping (binary 3-bit)
int mux[8][3] = {
    {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, 
    {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}
};

int result[64];          // Stores the 8x8 matrix readings
uint8_t result8bit[64];  // Stores the converted 8x8 matrix value
float ans;

// Define ESP32 GPIOs for MUX control pins
#define C1 4    // MUX1 - control pin C1
#define B1 17   // MUX1 - control pin B1
#define A1 16   // MUX1 - control pin A1
#define C2 22   // MUX2 - control pin C2
#define B2 21   // MUX2 - control pin B2
#define A2 15   // MUX2 - control pin A2

const int analogPin = 36;   // ADC1_CHANNEL_0
const int shutterPin = 32;  // SHUTTER PIN 32

void setup() {
  Serial.begin(115200);             // Serial init

  initButton(shutterPin);           // Shutter pin init

  setupSPI_SD();                    // SPI sdcard init

  analogSetAttenuation(ADC_11db);   // Analog 3.3v
  
  

  // Set all mux control pins as OUTPUT
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

void convertTo8bit() {
  for (int i = 0; i < 64; i++) {
    // Scale 0-4095 to 0-255 using float division, then round to int
    result8bit[i] = (uint8_t) round((result[i] / 4095.0) * 255.0);
  }
}

void readValue(){
  int iter = 0;

  Serial.println("Reading matrix...\n");

  for (int row = 0; row < 8; row++) {
    // Aktifkan ROW (MUX1 sebagai output selector)
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

void loop() {
  updateButton();

  if(isButtonPressedShort()){
    readValue();
  } 
  else if (isButtonPressedMedium()) {
    
  } 
  else if (isButtonPressedLong()) {
    
  }

}
