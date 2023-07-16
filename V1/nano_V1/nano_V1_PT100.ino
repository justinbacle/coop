#include <Adafruit_MAX31865.h>
#include <SPI.h>

// SPI: 10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK)
// Brunal (retour)
// EchCes, Ecs, Tampon
// Ces  (, aCes, dCh, rCh)
// Ambiance (HG)
//                   (CLK, CS, DO, DI)
Adafruit_MAX31865 Tc1(5,   4,   3,  2);
Adafruit_MAX31865 Tc2(9,   8,   7,  6);
Adafruit_MAX31865 Tc3(13,  12, 11, 10);

// RREF
// PT100 -> 430.0
// PT1000 -> 4300.0
float RREF = 430.0 ;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(250);
  delay(1000);
  // max.begin(MAX31865_2WIRE)
  // max.begin(MAX31865_3WIRE)
  // max.begin(MAX31865_4WIRE)
  Tc1.begin(MAX31865_2WIRE);
  Tc2.begin(MAX31865_2WIRE);
  Tc3.begin(MAX31865_2WIRE);
}

void loop() {
  if (Serial.available() > 0) {
    delay(100);char inData[8] = "";int index = 0;
    while (Serial.available()) {inData[index] = Serial.read();index++;}
    //                            PT100 -> temperature(100,  RREF_PT100)
    //                           PT1000 -> temperature(1000, RREF_PT1000)
    if (!strcmp(inData, "TC1\r\n")) {double c = Tc1.temperature(100, RREF);Serial.println(c);}
    if (!strcmp(inData, "TC2\r\n")) {double c = Tc2.temperature(100, RREF);Serial.println(c);}
    if (!strcmp(inData, "TC3\r\n")) {double c = Tc3.temperature(100, RREF);Serial.println(c);}
  }
  delay(100);
}