#include <Wire.h>
#include "sunrise_i2c.h"

//--- Sunrise sensor -----------------------------------------------------------------------
sunrise sunrise;


void setup()
{
  Serial.begin (115200);

  sunrise.initSunrise();

  /********************* WRITE CALIBRATION TARGET ***********************************************************/
  // Set target for calibration using setCalibrationCommand(TARGET_CALIBRATION)
  uint16_t calibrationTarget = 400;
  sunrise.writeCalibrationTarget(calibrationTarget);


  /********************* SET ABC TARGET *********************************************************************/
  // Set target for calibration using setCalibrationCommand() using arguments FORCED_ABC_CALIBRATION,
  // BACKGROUND_CALIBRATION. Also used as target for automatic ABC calibration.
  uint16_t abcTarget = 400;
  sunrise.setABCTarget(abcTarget);


  /********************* SET CALIBRATION COMMAND ***********************************************************/
  // Set command for type of calibration. Available options below:
  //  RESTORE_FACTORY_CALIBRATION
  //    - Restores calibration parameters to factory calibration values.
  //  FORCED_ABC_CALIBRATION
  //    - Sensor will perform an ABC calibration after receiving this command if sensor has valid ABC data.
  //  TARGET_CALIBRATION
  //    - Calibration using Calibration target using value from writeCalibrationTarget() as target.
  //  BACKGROUND_CALIBRATION
  //    - Calibration using ABC target from setABCTarget() as calibration target
  //  ZERO_CALIBRATION
  //    - Calibration using 0 ppm CO2 as calibration target.
  sunrise.setCalibrationCommand(RESTORE_FACTORY_CALIBRATION);


  /********************* SET ABC PERIOD *********************************************************************/
  // Period for ABC cycle in hours (range from 1 to 65534). Default is 180 hours
  uint16_t ABCPeriod = 180;
  sunrise.setABCPeriod(ABCPeriod);


  /********************* SET MEASUREMENT MODE ***************************************************************/
  // There are two measurement modes to choose between.
  //    Value = 0, continuous measurement mode (default)
  //    Value = 1, single measurement mode.
  // A system reset is required after changing measurement mode
  uint8_t measurementMode = 0;
  sunrise.setMeasurementMode(measurementMode);


  /********************* SET MEASUREMENT PERIOD *************************************************************/
  // Measurement period in seconds (range from 2 to 65534). Odd numbers will be rounded up to nearest
  // even number. A system reset is required after changing configuration.
  uint16_t measurementPeriod = 2;
  sunrise.setMeasurementPeriod(measurementPeriod);
  sunrise.resetSensor();


  /********************* SET NUMBER OF SAMPLES **************************************************************/
  // Number of samples in one measurement (range from 1 to 1024). A higher number leads to a better
  // accuracy but also a higher power consumption. A system reset is required after changing configuration.
  // Default is 8 samples. One sample takes < 200ms, this means that  Number of samples  * 0.2s should be
  // less than or equal to time between measurements. If time for executing all samples in a measurement is
  // longer than time between measurements, sensor will execute all samples and after that start a new measurement.
  // This means that actual time between measurements will be longer than expected
  uint16_t nbrSamples = 8;
  sunrise.setNbrSamples(nbrSamples);
  sunrise.resetSensor();


  /********************* SET METER CONTROL ******************************************************************/
  // Bit field used to enable/disable sensor functions
  // Bit 0:   0 - nRDY enabled (default)
  //          1 - nRDY disabled
  // Bit 1:   0 - ABC enabled (default)
  //          1 - ABC disabled
  // Bit 2:   0   Static IIR filter enabled (default)
  //          1 - Static IIR filter disabled
  // Bit 3:   0   Dynamic IIR filter enabled (default)
  //          1   Dynamic IIR filter disabled
  // Bit 4:   0 - Pressure compensation enabled
  //          1 - Pressure compensation disabled (default)
  // Bit 5-7: Unused
  byte controlByte = B00001000;
  sunrise.setMeterControl(controlByte);


  /********************* SET IIR FILTER ********************************************************************/
  // Parameter for static IIR filter, range from 2   10. A higher value corresponds to a harder filtration.
  uint8_t setIIRFilter = 5;
  sunrise.setIIRFilter(setIIRFilter);


}

void loop() {

}