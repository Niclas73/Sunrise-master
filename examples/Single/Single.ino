/*
**********************************************************************************
   @file        Single.ino
   @brief       Example to perform single CO2 measurement as described in
                the "I2C on Senseair Sunrise" documentation that can be found
                in the "extras" folder for the library, also available
                on the www.senseair.com website.

   @details     Tested on ESP32 Dev board

   @author      Niclas Ivarsson
   @version     1.0.0
   @date        2020-06-01

 *********************************************************************************
*/
#include <Wire.h>
#include "sunrise_i2c.h"

#define TIME_TO_SLEEP  5

const uint8_t                     SUNRISE_EN                        = 12;       // GPIO for EN-pin
const uint8_t                     SUNRISE_RDY                       = 19;       // GPIO for nRDY-pin
const uint8_t                     LED_BUILTIN                       = 2;        // Built in led for mcu
RTC_DATA_ATTR unsigned long       time_since_abc_time_incr          = 0;        // Timer for ABC-time

sunrise sunrise;

void setup()
{

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 1000000);

  pinMode(SUNRISE_RDY, INPUT);                          // Initialize pin to check if measurement is ready (nRDY-pin)

  pinMode(LED_BUILTIN, OUTPUT);                         // Initialize pin to drive built-in led

  pinMode(SUNRISE_EN, OUTPUT);                          // Initialize pin to drive supply for EN-pin (don't drive directly from pin!)
  digitalWrite(SUNRISE_EN, HIGH);
  delay(50);                                            // Wait for sensor to stabilize

  Serial.begin(115200);

  sunrise.initSunrise();

  /*

    //---------- Check error status end return error bits, 0 means no error -----------------------//
    Serial.print("Error status :                ");
    Serial.println(sunrise.readErrorStatus(), BIN);
    delay(50);

    //---------- Set number of samples in one measurement (range from 1 to 1024) ------------------//
    Serial.print("Set number of samples :               ");
    if (sunrise.setNbrSamples(8)) {
    Serial.println("SUCCESS");
    } else {
    Serial.println("FAILED");
    }

    //---------- Set measurement mode to single ----------------------------------------------------//
    Serial.print("Set measurement mode :                ");
    if (sunrise.setMeasurementMode(SINGLE)) {
    Serial.println("SUCCESS");
    } else {
    Serial.println("FAILED");
    }

    //---------- Set meter control -----------------------------------------------------------------//
    Serial.print("Set meter control byte :              ");
    if (sunrise.setMeterControl(B00001000)) {
    Serial.println("SUCCESS");
    } else {
    Serial.println("FAILED");
    }

    //---------- Reset sensor before starting measurements -----------------------------------------//
    Serial.print("Reset sensor :                        ");
    if (sunrise.resetSensor()) {
    Serial.println("SUCCESS");
    } else {
    Serial.println("FAILED");
    }

    sunrise.startSingleMeasurement();

    //---------- Check bit in error register to verify that the first measurement is available -----//
    while (sunrise.readErrorStatus() >> 7) {
    Serial.println("Waiting for first measurement to arrive...");
    delay(1000);
    }

  */

}


void loop() {
  digitalWrite(SUNRISE_EN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);                                            // Wait for sensor to stabilize

  //---------- Update ABC-time register if an hour has passed since  last ABC-time update ---------//
  if (3600000 < time_since_abc_time_incr) {
    sunrise.incrementABCTime();
    time_since_abc_time_incr = 0;
  }

  //---------- Get a new single measurement -------------------------------------------------------//
  uint16_t co2 = sunrise.getSingleReading(CO2_FILTERED_COMPENSATED, SUNRISE_RDY); // Returns 0 on error!
  if (co2 != 0) {
    Serial.print("CO2 : ");
    Serial.print(co2);
    Serial.println("ppm");
  } else {
    Serial.println("Measurement didn't complete, trying again in next cycle...");
  }

  digitalWrite(SUNRISE_EN, LOW);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("************************* GOING TO SLEEP ******************************");
  Serial.flush();
  time_since_abc_time_incr = time_since_abc_time_incr + millis() + TIME_TO_SLEEP * 1000;
  esp_deep_sleep_start();
}