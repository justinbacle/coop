#include <Adafruit_MAX31855.h>
#include <SPI.h>

// SPI: 10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK)
// Brunal (retour)
// EchCes, Ecs, Tampon
// Ces  (, aCes, dCh, rCh)
// Ambiance (HG)
//                   (CLK, CS, DO)
Adafruit_MAX31855 Tc1(4,   3,  2);
Adafruit_MAX31855 Tc2(7,   6,  5);
Adafruit_MAX31855 Tc3(10,  9,  8);
Adafruit_MAX31855 Tc4(13,  12,  11);

void setup() {Serial.begin(9600);Serial.setTimeout(250);delay(1000);}

void loop() {
  if (Serial.available() > 0) {
    delay(100);char inData[8] = "";int index = 0;
    while (Serial.available()) {inData[index] = Serial.read();index++;}
    if (!strcmp(inData, "TC1\r\n")) {double c = Tc1.readCelsius();Serial.println(c);}
    if (!strcmp(inData, "TC2\r\n")) {double c = Tc2.readCelsius();Serial.println(c);}
    if (!strcmp(inData, "TC3\r\n")) {double c = Tc3.readCelsius();Serial.println(c);}
    if (!strcmp(inData, "TC4\r\n")) {double c = Tc4.readCelsius();Serial.println(c);}
  }
  delay(100);
}
