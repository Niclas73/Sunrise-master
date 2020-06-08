/*
**********************************************************************************
 * @file        Continous.ino
 * @brief       Example to perform continous CO2 measurement as described in
 *              the "I2C on Senseair Sunrise" documentation that can be found
 *              in the "extras" folder for the library, also available 
 *              on the www.senseair.com website.
 *              
 * @details     Tested on ESP32 Dev board
 *              
 * @author      Niclas Ivarsson
 * @version     1.0.0
 * @date        2020-06-01
 * 
 *********************************************************************************
 */

#include <Wire.h>
#include "sunrise_i2c.h"

sunrise sunrise;

void setup()
{
  Serial.begin (115200);
  sunrise.initSunrise();

  //---------- Check error status end return error bits, 0 means no error -----------------------//
  Serial.print("Error status : ");
  Serial.println(sunrise.readErrorStatus(), BIN);           

  //---------- Set measurement period in seconds (range from 2 to 65534) ------------------------//
  Serial.print("Set measurement period, response :  ");
  Serial.println(sunrise.setMeasurementPeriod(2));   

  //---------- Set number of samples in one measurement (range from 1 to 1024) ------------------//
  Serial.print("Set number of samples, response :   ");
  Serial.println(sunrise.setNbrSamples(16)); 

  //---------- Set measurement mode to continous -------------------------------------------------//
  Serial.print("Set measurement mode, response :    ");
  Serial.println(sunrise.setMeasurementMode(CONTINOUS));  

  Serial.print("Reset sensor, response :            ");
  Serial.println(sunrise.resetSensor());

  //---------- Check bit in error register to verify that the first measurement is available ----//
  while(sunrise.readErrorStatus() >> 7){
    Serial.println("Waiting for first measurement to arrive...");  
    delay(1000);
  }

}


void loop() {
  Serial.println("********************** CO2 READINGS ****************************");
  uint16_t CO2 = sunrise.readCO2(CO2_FILTERED_COMPENSATED);
  Serial.print("CO2 reading (filtered and pressure compensated) :          ");
  Serial.println(CO2);
  CO2 = sunrise.readCO2(CO2_UNFILTERED_COMPENSATED);
  Serial.print("CO2 reading (unfiltered and pressure compensated) :        ");
  Serial.println(CO2);
  CO2 = sunrise.readCO2(CO2_FILTERED_UNCOMPENSATED);
  Serial.print("CO2 reading (filtered, not pressure compensated) :         ");
  Serial.println(CO2);
  CO2 = sunrise.readCO2(CO2_UNFILTERED_UNCOMPENSATED);
  Serial.print("CO2 reading (unfiltered, not pressure compensated) :       ");
  Serial.println(CO2);
  Serial.println("****************************************************************");
  Serial.println();
  delay(1000);
}