/*
  ESP32 NOTE Explorer ♩ ♪ ♫ ♬
  Plays all ledc notes that are available 
  https://wokwi.com/projects/351175246893548120
  by dlloydev, December 2022.
*/

#include <LiquidCrystal_I2C.h>
#include <pwmWrite.h>

const int tonePin = 4;
const int notePin = 12;
const int octavePin = 13;
const int notes = 12;
const int octaves = 8;
const char *lcdNote[notes] = { "C ", "Cs", "D ", "Eb", "E ", "F ", "Fs", "G ", "Gs", "A ", "Bb", "B " };
const note_t note[notes] = { NOTE_C, NOTE_Cs, NOTE_D, NOTE_Eb, NOTE_E, NOTE_F, NOTE_Fs, NOTE_G, NOTE_Gs, NOTE_A, NOTE_Bb, NOTE_B };
const int octave[octaves] = { 1, 2, 3, 4, 5, 6, 7, 8 };
const int duration = 400;  // note duration ms
const int interval = 0;    // pause ms

byte sound[] = { // ♬
  B00001,
  B00011,
  B00101,
  B01001,
  B01001,
  B01011,
  B11011,
  B11000
};

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);
Pwm pwm = Pwm();

void setup() {
  Serial.begin(115200);
  LCD.init();
  LCD.backlight();
  LCD.createChar(0, sound);
  LCD.setCursor(0, 0);
  LCD.write(0);
  LCD.print("  ");
  LCD.write(0);
  LCD.print("  ");
  LCD.write(0);
  LCD.print("  NOTE_  ");
  LCD.setCursor(1, 1);
  LCD.write(0);
  LCD.print("  ");
  LCD.write(0);
  LCD.print("   Octave  ");
  pinMode(notePin, INPUT);
  pinMode(octavePin, INPUT);
  pwm.attach(notePin, 2);  // attach to ch 2
}

void loop() {
  static int nip = 1, oip = 1;
  int noteRead = analogRead(notePin);
  int octaveRead = analogRead(octavePin);

  int oi = map(octaveRead, 0, 4095, 0, octaves - 1);
  if (oip != oi) {
    oip = oi;
    LCD.setCursor(15, 1);
    LCD.print(oi + 1);
  }
  int ni = map(noteRead, 0, 4095, 0, notes - 1);
  if (nip != ni) {
    nip = ni;
    LCD.setCursor(14, 0);
    LCD.print(lcdNote[ni]);
  }
  pwm.note(tonePin, (note_t)note[ni], octave[oi], duration, interval);  // auto attaches to ch 0
  delay(2);
}
