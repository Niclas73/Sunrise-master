
#include <Wire.h>
#include <Arduino.h>
#include "sunrise_i2c.h"


const uint8_t					DATA_PIN                 = 21;    // Data pin
const uint8_t					CLOCK_PIN                = 22;    // Clock pin
const int						ATTEMPTS                 = 5;     // Wake up attempts
const uint8_t					SUNRISE_ADDRESS          = 0x68;  // Sunrise i2c address
const int						EEPROM_WRITE_DELAY       = 25;    // Write delay for EEPROM
RTC_DATA_ATTR uint8_t			powerDownData[]			 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};         // Array for sensor state


sunrise::sunrise() {};


boolean wakeUp() {    // Done!
  int attemps = ATTEMPTS;
  int error;
  do {
    uint8_t byte_0;
    /* */
    Wire.beginTransmission(SUNRISE_ADDRESS);
    error = Wire.endTransmission(true);
  } while (((error != 0 ) && (error != 2 ) && (error != 1)) && (--attemps > 0));
  if (error == 4) {
    Wire.begin(DATA_PIN, CLOCK_PIN);
    Wire.setClock(100000);
    return false;
  }
  return (attemps > 0);
}


int8_t read8bitSigned (uint8_t offsetReg) {   // Done!
  int8_t myReturn = -1;

  if (!(wakeUp())) {
    Serial.println("No response from sensor on wake up!");
    return false;
  }

  Wire.beginTransmission (SUNRISE_ADDRESS);
  Wire.write(offsetReg);
  Wire.endTransmission(false);
  if (Wire.requestFrom(SUNRISE_ADDRESS, (byte)1) == 1) {
    while (Wire.available())                         // slave may send less than requested
    {
      myReturn = (int8_t)Wire.read();
    }
  }
  return myReturn;
}


boolean write8bit(uint8_t offsetReg, int8_t myValue) {    // DONE!
  boolean ok = false;

  if (!(wakeUp())) {
    Serial.println("No response from sensor on wake up!");
    return false;
  }

  Wire.beginTransmission (SUNRISE_ADDRESS);
  Wire.write(offsetReg);
  Wire.write(myValue);
  if (Wire.endTransmission(true) == 0) {
    ok = true;
  } else {
    Serial.println("Failed tosend request!");
  }
  delay(EEPROM_WRITE_DELAY);    // Only necessary when updating EEPROM, but added in general
  return ok;
}


int16_t read16bitSigned (uint8_t offsetReg) {         // DONE!
  int16_t myReturn = -1;

  if (!(wakeUp())) {
    Serial.println("No response from sensor on wake up!");
    return myReturn;
  }

  Wire.beginTransmission (SUNRISE_ADDRESS);
  Wire.write(offsetReg);
  Wire.endTransmission(false);
  if (Wire.requestFrom(SUNRISE_ADDRESS, (byte)2) == 2) {
    while (Wire.available())                         // slave may send less than requested
    {
      uint8_t hiByte = Wire.read();
      uint8_t loByte = Wire.read();
      myReturn = ((uint16_t)(int8_t) hiByte << 8) | (uint16_t)loByte;
    }
  } else {
    Serial.println("Failed to write to sensor!");
  }
  return myReturn;
}


boolean write16bit(uint8_t offsetReg, int8_t hiByte, int8_t loByte) {     // DONE!
  boolean ok = false;

  if (!(wakeUp())) {
    Serial.println("No response from sensor on wake up!");
    return false;
  }

  Wire.beginTransmission (SUNRISE_ADDRESS);
  Wire.write(offsetReg);
  Wire.write(hiByte);
  Wire.write(loByte);
  if (Wire.endTransmission(true) == 0) {
    ok = true;
  } else {
    Serial.println("Failed to send request!");
  }
  delay(EEPROM_WRITE_DELAY);  // Only necessary when updating EEPROM, but added in general
  return ok;
}


void sunrise::initSunrise() {     // DONE!
  Wire.begin(DATA_PIN, CLOCK_PIN);
  Wire.setClock(100000);
  Wire.beginTransmission(SUNRISE_ADDRESS);
  while (Wire.endTransmission () != 2)  // Receive 2 = success (Address NACK response)
  {
    Serial.print("Waiting for sunrise response from address 0x");
    Serial.println(SUNRISE_ADDRESS, HEX);
    delay(1000);
    Wire.begin(DATA_PIN, CLOCK_PIN);
    Wire.setClock(100000);
    Wire.beginTransmission(SUNRISE_ADDRESS);
  }
  Serial.println("I2C ready!");
  delay(100);
}


uint16_t sunrise::getSingleReading(uint8_t co2_type, uint8_t readyPin) {

  int error;
  boolean ok;
  uint16_t co2 = 0;

  boolean powerDownDataIsPopulated = false;
  for (int i=0 ; i<24 ; i++) {
	if(powerDownData[i] > 0){
		powerDownDataIsPopulated = true;
		break;
		;
	}
  }

if (!powerDownDataIsPopulated) {
  if (!readPowerDownData()) {
    Serial.println("Failed to save power down data!");
  } else {
	Serial.println("Power down data was NOT populated, data read from sensor for next measurement.");
  }
} else {
  if (!writePowerDownData()) {
    Serial.println("Failed to save power down data!");
    return 0;
  } else {
	Serial.println("Power down data was populated, data was written to sensor.");
  }
}
 
 

 if(!startSingleMeasurement()){
	Serial.println("Failed to start single measurement!");
 	 return 0;
 }

  uint8_t cnt = 0;
  uint8_t maxWaitLoops = 300;
  while(digitalRead(readyPin) && cnt < maxWaitLoops) {
	Serial.println("Waiting for measurement...");
	cnt++;
	delay(200);
  }
  if(cnt == maxWaitLoops){
	Serial.println("Measurement timeout, exiting...");
	return 0;
  }
  
  error = readErrorStatus();
  if(error != 0 ) {  
    Serial.println("An error occured...");
  } else {
	co2 = readCO2(co2_type);
  }

 ok = readPowerDownData();

 return co2;

}

void sunrise::incrementABCTime(){
	uint16_t abc_time = ((int16_t)(int8_t) powerDownData[0] << 8) | (uint16_t)powerDownData[1];
    abc_time = abc_time + 1;
    powerDownData[0] = abc_time >> 8;
    powerDownData[1] = abc_time & 0x00FF;
    Serial.print("ABC time updated, current ABC-time : ");
    Serial.println(abc_time);
}


int16_t sunrise::readErrorStatus(void) {
  return read16bitSigned(ERROR_STATUS);
}


int16_t sunrise::readChipTemp(void) {
  return read16bitSigned(CHIP_TETMP);
}


uint8_t sunrise::readMeasurementCount(void) {
  return read8bitSigned(MEASUREMENT_COUNT);
}


int16_t  sunrise::readMeasurementCycleTime(void) {
  return read16bitSigned(MEASUREMENT_CYCLE_TIME);
}


int16_t  sunrise::readCO2(uint8_t CO2Type) {
  return read16bitSigned(CO2Type);
}


int8_t sunrise::readCalibrationStatus(void) {
  return read8bitSigned(CALIBRATION_STATUS);
}


boolean sunrise::clearCalibrationStatus(void) {
  boolean ok = write8bit(CALIBRATION_STATUS, 0);
  return ok;
}


boolean sunrise::setCalibrationCommand(uint16_t calibrationType) {
  int8_t low = calibrationType >> 8;
  int8_t high = calibrationType & 0x00FF;
  boolean ok = write16bit(CALIBRATION_COMMAND, low , high);
  return ok;
}


int16_t sunrise::readCalibrationTarget(void) {
  return read16bitSigned(CALIBRATION_TARGET);
}


boolean sunrise::writeCalibrationTarget(uint16_t calibrationTarget) {
  int8_t low = calibrationTarget >> 8;
  int8_t high = calibrationTarget & 0x00FF;
  boolean ok = write16bit(CALIBRATION_TARGET, low , high);
  return ok;
}


int16_t  sunrise::readABCTime() {
  return read16bitSigned(ABC_TIME);
}


boolean sunrise::writeABCTime(uint16_t ABCTime) {
  int8_t low = ABCTime >> 8;
  int8_t high = ABCTime & 0x00FF;
  boolean ok = write16bit(ABC_TIME, low , high);
  return ok;
}


boolean sunrise::startSingleMeasurement(void) {
  boolean ok = write8bit(START_SINGLE_MEASUREMENT, 1);
  return ok;
}


boolean sunrise::setMeasurementMode(uint8_t measurementMode) {
  boolean ok = write8bit(MEASUREMENT_MODE, measurementMode);
  return ok;
}


boolean sunrise::setMeasurementPeriod(uint16_t measurementPeriod) {
  int8_t low = measurementPeriod >> 8;
  int8_t high = measurementPeriod & 0x00FF;
  boolean ok = write16bit(MEASUREMENT_PERIOD, low , high);
  return ok;
}


boolean sunrise::setNbrSamples(uint16_t nbrSamples) {
  int8_t low = nbrSamples >> 8;
  int8_t high = nbrSamples & 0x00FF;
  boolean ok = write16bit(NBR_SAMPLES, low , high);
  return ok;
}


boolean sunrise::setABCPeriod(uint16_t ABCPeriod) {
  int8_t low = ABCPeriod >> 8;
  int8_t high = ABCPeriod & 0x00FF;
  boolean ok = write16bit(ABC_PERIOD, low , high);
  return ok;
}


boolean sunrise::clearErrorStatus(void) {
  boolean ok = write8bit(CLEAR_ERROR_STATUS, 1);
  delay(500);
  return ok;
}


boolean sunrise::setABCTarget(uint16_t ABCTarget) {
  int8_t low = ABCTarget >> 8;
  int8_t high = ABCTarget & 0x00FF;
  boolean ok = write16bit(ABC_TARGET, low , high);
  return ok;
}


boolean sunrise::setIIRFilter(uint8_t IIRFilter) {
  boolean ok = write8bit(IIR_FILTER_PARMETER, IIRFilter);
  return ok;
}


boolean sunrise::resetSensor() {
  delay(100);
  boolean ok = write8bit(RESET_SENSOR, 0xFF);
  delay(100);
  return ok;
}


boolean sunrise::setMeterControl(byte controlByte) {
  boolean ok = write8bit(METER_CONTROL, (uint8_t)controlByte);
  return ok;
}


int8_t sunrise::readMeterControl(void) {
  return read8bitSigned(METER_CONTROL);
}


boolean sunrise::setI2cAddress(uint8_t i2caddress) {
  boolean ok = write8bit(I2C_ADDRESS, i2caddress);
  resetSensor();
  return ok;
}


boolean sunrise::readPowerDownData() {
  boolean ok = false;

  if (!(wakeUp())) {
    Serial.println("No response from sensor on wake up!");
    return false;
  }

  Wire.beginTransmission (SUNRISE_ADDRESS);
  Wire.write(POWER_DOWN_DATA);
  Wire.endTransmission(false);
  if (Wire.requestFrom(SUNRISE_ADDRESS, (byte)24) == 24) {
    uint8_t count = 0;
    while (Wire.available() && count < 24)                       // slave may send less than requested
    {
      powerDownData[count++] = Wire.read();
    }
    ok = true;
  }
  return ok;
}


boolean sunrise::writePowerDownData() {
  boolean ok = false;

  if (!(wakeUp())) {
    Serial.println("No response from sensor on wake up!");
    return false;
  }

  Wire.beginTransmission (SUNRISE_ADDRESS);
  Wire.write(POWER_DOWN_DATA);
  uint8_t count = 0;
  while (count < 24) {
    Wire.write(powerDownData[count++]);
  }
  if (Wire.endTransmission() == 0) {
    ok = true;
  } else {
    Serial.println("Failed to send request!");
  }
  delay(EEPROM_WRITE_DELAY);  // Only necessary when updating EEPROM, but added in general
  return ok;
}
