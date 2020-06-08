#include <Wire.h>
#include "sunrise_i2c.h"

//--- Sunrise sensor -----------------------------------------------------------------------
sunrise sunrise;


void setup()
{
  Serial.begin (115200);
  
  sunrise.initSunrise();

  uint8_t newAddress = 0x68;  // Don't forget to update #define in library file sunrise_i2c.cpp afterwards
  while (!sunrise.setI2cAddress(newAddress)) {
    Serial.println("Trying to update address...");
    delay(1000);
    sunrise.initSunrise();
  }

  Serial.print("New address is set to 0x");
  Serial.println(newAddress, HEX);

}

void loop() {

}