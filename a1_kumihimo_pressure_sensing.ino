/*
  Gentle Dual Blink - Fade In/Out on Two LEDs without delay()

  Green LED = "carry on" (pin 11)
  White LED = "be gentle" (pin 12)

  References:
  - Fade effect technique adapted from:
    https://github.com/DigitalFuturesOCADU/CC2025/blob/d2d4f1ba8b68a398844e9cd4729d82e5c48f15fc/experiment1/leds/ledPattern_fade4/ledPattern_fade4.ino
    (DigitalFuturesOCADU/CC2025, experiment1/leds/ledPattern_fade4/ledPattern_fade4.ino)
  - Non-blocking timing with millis() inspired by:
    https://www.digikey.ca/en/maker/tutorials/2022/how-to-avoid-using-the-delay-function-in-arduino-sketches
*/

int sensorValue = 0;
long sensorMax = 1023;
long sensorMin = 0;

int sensorPin = A6;
int carryOnPin = 11;   // Green LED
int beGentlePin = 12;  // White LED

float mappedValue = 0.0;
int threshold = 14;

// Separate fade states for each LED
int carryOnBrightness = 0;
int beGentleBrightness = 0;
int fadeAmount = 1;
bool carryOnFadingUp = true;
bool beGentleFadingUp = true;

unsigned long lastUpdate = 0;
unsigned long fadeInterval = 10; // ms

void setup() {
  Serial.begin(9600);
  pinMode(carryOnPin, OUTPUT);
  pinMode(beGentlePin, OUTPUT);
}

void loop() {
  sensorValue = analogRead(sensorPin);

  // Update min and max
  if (sensorValue < sensorMax) sensorMax = sensorValue;
  if (sensorValue > sensorMin) sensorMin = sensorValue;

  // Map min and max to 0~100 scale
  mappedValue = map(sensorValue, sensorMin, sensorMax, 0.0, 100.0);

  // Print in serial monitor
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print(" Mapped Value: ");
  Serial.print(mappedValue);
  Serial.print(" (Threshold: ");
  Serial.print(threshold);
  Serial.print(")\n");

  // use the millis()-method to make the Arduino execute a piece of code with a specified frequency
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= fadeInterval) {
    lastUpdate = currentMillis;

    if (mappedValue < threshold) {
      // Fade beGentle (white), turn off carryOn (green)
      carryOnBrightness = 0;
      analogWrite(carryOnPin, carryOnBrightness);

      if (beGentleFadingUp) {
        beGentleBrightness += fadeAmount;
        if (beGentleBrightness >= 255) {
          beGentleBrightness = 255;
          beGentleFadingUp = false;
        }
      } else {
        beGentleBrightness -= fadeAmount;
        if (beGentleBrightness <= 0) {
          beGentleBrightness = 0;
          beGentleFadingUp = true;
        }
      }
      analogWrite(beGentlePin, beGentleBrightness);
    } else {
      // Fade carryOn (green), turn off beGentle (white)
      beGentleBrightness = 0;
      analogWrite(beGentlePin, beGentleBrightness);

      if (carryOnFadingUp) {
        carryOnBrightness += fadeAmount;
        if (carryOnBrightness >= 255) {
          carryOnBrightness = 255;
          carryOnFadingUp = false;
        }
      } else {
        carryOnBrightness -= fadeAmount;
        if (carryOnBrightness <= 0) {
          carryOnBrightness = 0;
          carryOnFadingUp = true;
        }
      }
      analogWrite(carryOnPin, carryOnBrightness);
    }
  }
}
