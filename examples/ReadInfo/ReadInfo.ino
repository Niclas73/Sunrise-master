#include <Wire.h>
#include "sunrise_i2c.h"

//--- Sunrise sensor -----------------------------------------------------------------------
sunrise sunrise;


void setup()
{
  Serial.begin (115200);

  /********************* INITIALIZE SUNRISE SENSOR **********************************************************/
  // Initializes and waits for NACK on address transmission. Returns true when ready.
  sunrise.initSunrise();
}

void loop() {

  Serial.println("******************** SENSOR INFO *************************");

  /********************* READ AND CLEAR ERROR STATUS ***************'******************************************/
  // Get two bytes representing error status according to below :
  // Bit 0  - Low internal regulated voltage
  // Bit 1  - Measurement timeout
  // Bit 2  - Reserved
  // Bit 3  - Reserved
  // Bit 4  - Reserved
  // Bit 5  - Reserved
  // Bit 6  - Reserved
  // Bit 7  - Reserved
  // Bit 0  - Fatal error
  // Bit 1  - I2C error
  // Bit 2  - Algorithm error
  // Bit 3  - Calibration error
  // Bit 4  - Self diagnostics error
  // Bit 5  - Out of range
  // Bit 6  - Memory error
  // Bit 7  - No measurement completed
  int16_t errorStatus = sunrise.readErrorStatus();
  Serial.print("Error status :           ");
  Serial.println(errorStatus);
  sunrise.clearErrorStatus();


  /********************* READ CHIP TEMPERATURE ***********************'***************************************/
  // Chip temperature, unit °C x100. For example 2223 means 22.23°C
  int16_t chipTemp = sunrise.readChipTemp();
  Serial.print("Chip temperature :       ");
  Serial.println((float)chipTemp * 0.01);


  /********************* READ MEASUREMENT COUNT ***********************'***************************************/
  // Counter incremented after each measurement, range 0 – 255. The counter wraps around after the maximum
  // value is reached.
  uint8_t measurementCounter = sunrise.readMeasurementCount();
  Serial.print("Measurement counter :    ");
  Serial.println(measurementCounter);


  /********************* READ METER CONTROL ***************************'***************************************/
  // Bit field used to enable/disable sensor functions
  // Bit 0:   0 - nRDY enabled (default)
  //          1 - nRDY disabled
  // Bit 1:   0 - ABC enabled (default)
  //          1 - ABC disabled
  // Bit 2:   0 – Static IIR filter enabled (default)
  //          1 - Static IIR filter disabled
  // Bit 3:   0 – Dynamic IIR filter enabled (default)
  //          1 – Dynamic IIR filter disabled
  // Bit 4:   0 - Pressure compensation enabled
  //          1 - Pressure compensation disabled (default)
  // Bit 5-7: Unused set to 1 by default
  // Default value = B11110000.
  uint8_t meterControl = sunrise.readMeterControl();
  Serial.print("Meter control :          ");
  Serial.println(meterControl, BIN);



  /********************* READ MEASUREMENT CYCLE TIME ****************'***************************************/
  // Measurement cycle time shows current time in present measurement cycle, incremented every 2 seconds.
  // For example, Measurement cycle time = 3 means 6 seconds has passed in current measurement cycle.
  // Value is set to 0 when sensor starts a new measurement.
  uint16_t measurementCycleTime = sunrise.readMeasurementCycleTime();
  Serial.print("Measurement cycle time : ");
  Serial.println(measurementCycleTime);


  /********************* READ CO2 ****************'********************************************************/
  // Current CO2 reading. Supply argument to control what type of reading you want to return according to below:
  // CO2_FILTERED_COMPENSATED     -   Filtered and pressure compensated reading
  // CO2_UNFILTERED_COMPENSATED   -   Unfiltered and pressure compensated reading
  // CO2_FILTERED_UNCOMPENSATED   -   Filtered reading but without pressure compensation
  // CO2_UNFILTERED_UNCOMPENSATED -   Unfiltered reading and without pressure compensation
  uint16_t CO2 = sunrise.readCO2(CO2_FILTERED_COMPENSATED);
  Serial.print("CO2 reading FC :         ");
  Serial.println(CO2);
  CO2 = sunrise.readCO2(CO2_UNFILTERED_COMPENSATED);
  Serial.print("CO2 reading UFC :        ");
  Serial.println(CO2);
  CO2 = sunrise.readCO2(CO2_FILTERED_UNCOMPENSATED);
  Serial.print("CO2 reading FUC :        ");
  Serial.println(CO2);
  CO2 = sunrise.readCO2(CO2_UNFILTERED_UNCOMPENSATED);
  Serial.print("CO2 reading UFUC :       ");
  Serial.println(CO2);


  /********************* READ CALIBRATION STATUS ********************'***************************************/
  // These bits are set after successful calibrations. The bits need to be cleared/reset by host system,
  // it is recommended to do this before starting a calibration.
  // Bit 0 Not used
  // Bit 1 Not used
  // Bit 2 Factory calibration restored
  // Bit 3 ABC calibration
  // Bit 4 Target calibration
  // Bit 5 Background calibration
  // Bit 6 Zero calibration
  // Bit 7 Not used
  uint8_t calibrationStatus = sunrise.readCalibrationStatus();
  Serial.print("Calibration status :     ");
  Serial.println(calibrationStatus);


  /********************* READ CALIBRATION TARGET ****************'******************************************/
  // Calibration target used by target calibration.
  uint16_t calibrationTarget = sunrise.readCalibrationTarget();
  Serial.print("Calibration target :     ");
  Serial.println(calibrationTarget);


  /********************* READ CALIBRATION TARGET ****************'******************************************/
  // Time passed since last ABC calibration in hours. If ABC is enabled when sensor is used in single
  // measurement mode and powered down between measurements the host system must increment this value every hour.
  uint16_t abcTime = sunrise.readABCTime();
  Serial.print("ABC time :               ");
  Serial.println(abcTime);


  Serial.println("**********************************************************");
  Serial.println();
  delay(1000);
}