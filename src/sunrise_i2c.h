/***************************************************************************
  This is a library for the Sunrise C02 sensor from Senseair
  Niclas Ivarsson 2020-05-28
****************************************************************************/



/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
    enum
    {
      ERROR_STATUS                        = 0x00, // 2 bytes  Done!
      CO2_FILTERED_COMPENSATED            = 0x06, // 2 bytes  Done!
      CHIP_TETMP                          = 0x08, // 2 bytes  Done!
      MEASUREMENT_COUNT                   = 0x0D, // 1 byte   Done!
      MEASUREMENT_CYCLE_TIME              = 0x0E, // 2 byte   Done!
      CO2_UNFILTERED_COMPENSATED          = 0x10, // 2 bytes  Done!
      CO2_FILTERED_UNCOMPENSATED          = 0x12, // 2 bytes  Done!
      CO2_UNFILTERED_UNCOMPENSATED        = 0x14, // 2 bytes  Done!
      CALIBRATION_STATUS                  = 0x81, // 1 byte   Done!
      CALIBRATION_COMMAND                 = 0x82, // 2 bytes  Done!
      CALIBRATION_TARGET                  = 0x84, // 2 bytes  Done!
      ABC_TIME                            = 0x88, // 2 bytes  Done! 
      ABC_0                               = 0x8A, // 2 byte
      ABC_1                               = 0x8C, // 2 byte
      ABC_2                               = 0x8E, // 2 byte
      ABC_3                               = 0x90, // 2 byte
      START_SINGLE_MEASUREMENT            = 0xC3, // 1 byte   Done!
      MEASUREMENT_MODE                    = 0x95, // 1 byte   Done!
      MEASUREMENT_PERIOD                  = 0x96, // 2 bytes  Done!
      NBR_SAMPLES                         = 0x98, // 2 bytes  Done!
      ABC_PERIOD                          = 0x9A, // 2 bytes  Done!
      CLEAR_ERROR_STATUS                  = 0x9D, // 1 byte   Done!
      ABC_TARGET                          = 0x9E, // 2 bytes  Done! 
      IIR_FILTER_PARMETER                 = 0xA1, // 1 byte   Done!
      RESET_SENSOR                        = 0xA3, // 1 byte   Done!
      METER_CONTROL                       = 0xA5, // 1 byte   Done! 
      I2C_ADDRESS                         = 0xA7, // 1 byte   Done!
      POWER_DOWN_DATA                     = 0xC4, // 24 byte
    };

/*=========================================================================*/

enum
{
  RESTORE_FACTORY_CALIBRATION             = 0x7C02,
  FORCED_ABC_CALIBRATION                  = 0x7C03,
  TARGET_CALIBRATION                      = 0x7C05,
  BACKGROUND_CALIBRATION                  = 0x7C06,
  ZERO_CALIBRATION                        = 0x7C07,  
};

enum
{
  CONTINOUS                               = 0x00,
  SINGLE                                  = 0x01,
};

class sunrise
{
  public:
    sunrise(void);
    void      initSunrise();
    int16_t   readErrorStatus(void);
    int16_t   readChipTemp(void);
    uint8_t   readMeasurementCount(void);
    int16_t   readMeasurementCycleTime(void);
    int16_t   readCO2(uint8_t CO2Type);
    int8_t    readCalibrationStatus(void);
    boolean   clearCalibrationStatus(void);
    boolean   setCalibrationCommand(uint16_t calibrationType);
    int16_t   readCalibrationTarget();
    boolean   writeCalibrationTarget(uint16_t calibrationTarget);
    int16_t   readABCTime(void);
    boolean   writeABCTime(uint16_t ABCTime);
	void	  incrementABCTime(void);
    boolean   startSingleMeasurement(void);
    boolean   setMeasurementMode(uint8_t measurementMode);
    boolean   setMeasurementPeriod(uint16_t measurementPeriod);
    boolean   setNbrSamples(uint16_t nbrS);    
    boolean   setABCPeriod(uint16_t ABCPeriod);
    boolean   clearErrorStatus(void);
    boolean   setABCTarget(uint16_t abcTarget);
    boolean   setIIRFilter(uint8_t IIRFilter);
    boolean   resetSensor(void);
    int8_t    readMeterControl(void);
    boolean   setMeterControl(byte);
    boolean   setI2cAddress(uint8_t i2caddress);
    boolean   readPowerDownData(void);
    boolean   writePowerDownData(void);
	uint16_t  getSingleReading(uint8_t co2_type, const uint8_t readyPin);
};
