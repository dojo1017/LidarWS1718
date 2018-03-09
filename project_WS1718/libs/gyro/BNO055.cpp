#include "BNO055.h"
#include <math.h>
#include <limits.h>
#include <chrono>

BNO055::BNO055(int address, int reset = 0) {

	//Standardadresse lautet 0x28
	//Wird PS1 gesetzt ist die Adresse 0x29
	wiringPiSetup();
	fd = wiringPiI2CSetup(address); /*Use i2cdetect command to find your respective device address*/

	resetPin = reset;

	if (fd == -1) {
		cout << "Keine Verbindung zum BNO055 möglich! Verkabelung korrekt?" << endl;
	}

}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Sets up the HW
*/
/**************************************************************************/
bool BNO055::begin(adafruit_bno055_opmode_t mode) {

	/* Make sure we have the right device */
	uint8_t id = read8(BNO055_CHIP_ID_ADDR);
	if (id != BNO055_ID) {
		usleep(1000000 * 0.001 * 1000); // hold on for boot
		id = read8(BNO055_CHIP_ID_ADDR);
		if (id != BNO055_ID) {
			return false;  // still not? ok bail
		}
	}

	/* Switch to config mode (just in case since this is the default) */
	setMode(OPERATION_MODE_CONFIG);

	/* Reset */
	write8(BNO055_SYS_TRIGGER_ADDR, 0x20);
	while (read8(BNO055_CHIP_ID_ADDR) != BNO055_ID) {
		usleep(1000000 * 0.001 * 10);
	}
	usleep(1000000 * 0.001 * 50);

	/* Set to normal power mode */
	write8(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);
	usleep(1000000 * 0.001 * 10);

	write8(BNO055_PAGE_ID_ADDR, 0);

	/* Set the output units */
	/*
	uint8_t unitsel = (0 << 7) | // Orientation = Android
					  (0 << 4) | // Temperature = Celsius
					  (0 << 2) | // Euler = Degrees
					  (1 << 1) | // Gyro = Rads
					  (0 << 0);  // Accelerometer = m/s^2
	write8(BNO055_UNIT_SEL_ADDR, unitsel);
	*/

	/* Configure axis mapping (see section 3.4) */
	/*
	write8(BNO055_AXIS_MAP_CONFIG_ADDR, REMAP_CONFIG_P2); // P0-P7, Default is P1
	usleep(1000000 * 0.001 * 10);
	write8(BNO055_AXIS_MAP_SIGN_ADDR, REMAP_SIGN_P2); // P0-P7, Default is P1
	usleep(1000000 * 0.001 * 10);
	*/

	write8(BNO055_SYS_TRIGGER_ADDR, 0x0);
	usleep(1000000 * 0.001 * 10);
	/* Set the requested operating mode (see section 3.3) */
	setMode(mode);
	usleep(1000000 * 0.001 * 20);

	return true;
}

/**************************************************************************/
/*!
    @brief  Puts the chip in the specified operating mode
*/
/**************************************************************************/
void BNO055::setMode(adafruit_bno055_opmode_t mode) {
	_mode = mode;
	write8(BNO055_OPR_MODE_ADDR, _mode);
	usleep(1000000 * 0.001 * 30);
}

/**************************************************************************/
/*!
    @brief  Changes the chip's axis remap
*/
/**************************************************************************/
void BNO055::setAxisRemap(adafruit_bno055_axis_remap_config_t remapcode) {
	adafruit_bno055_opmode_t modeback = _mode;

	setMode(OPERATION_MODE_CONFIG);
	usleep(1000000 * 0.001 * 25);
	write8(BNO055_AXIS_MAP_CONFIG_ADDR, remapcode);
	usleep(1000000 * 0.001 * 10);
	/* Set the requested operating mode (see section 3.3) */
	setMode(modeback);
	usleep(1000000 * 0.001 * 20);
}

/**************************************************************************/
/*!
    @brief  Changes the chip's axis signs
*/
/**************************************************************************/
void BNO055::setAxisSign(adafruit_bno055_axis_remap_sign_t remapsign) {
	adafruit_bno055_opmode_t modeback = _mode;

	setMode(OPERATION_MODE_CONFIG);
	usleep(1000000 * 0.001 * 25);
	write8(BNO055_AXIS_MAP_SIGN_ADDR, remapsign);
	usleep(1000000 * 0.001 * 10);
	/* Set the requested operating mode (see section 3.3) */
	setMode(modeback);
	usleep(1000000 * 0.001 * 20);
}


/**************************************************************************/
/*!
    @brief  Use the external 32.768KHz crystal
*/
/**************************************************************************/
void BNO055::setExtCrystalUse(boolean usextal) {
	adafruit_bno055_opmode_t modeback = _mode;

	/* Switch to config mode (just in case since this is the default) */
	setMode(OPERATION_MODE_CONFIG);
	usleep(1000000 * 0.001 * 25);
	write8(BNO055_PAGE_ID_ADDR, 0);
	if (usextal) {
		write8(BNO055_SYS_TRIGGER_ADDR, 0x80);
	} else {
		write8(BNO055_SYS_TRIGGER_ADDR, 0x00);
	}
	usleep(1000000 * 0.001 * 10);
	/* Set the requested operating mode (see section 3.3) */
	setMode(modeback);
	usleep(1000000 * 0.001 * 20);
}


/**************************************************************************/
/*!
    @brief  Gets the latest system status info
*/
/**************************************************************************/
void BNO055::getSystemStatus(uint8_t *system_status, uint8_t *self_test_result, uint8_t *system_error) {
	write8(BNO055_PAGE_ID_ADDR, 0);

	/* System Status (see section 4.3.58)
	   ---------------------------------
	   0 = Idle
	   1 = System Error
	   2 = Initializing Peripherals
	   3 = System Iniitalization
	   4 = Executing Self-Test
	   5 = Sensor fusio algorithm running
	   6 = System running without fusion algorithms */

	if (system_status != 0)
		*system_status = read8(BNO055_SYS_STAT_ADDR);

	/* Self Test Results (see section )
	   --------------------------------
	   1 = test passed, 0 = test failed
  
	   Bit 0 = Accelerometer self test
	   Bit 1 = Magnetometer self test
	   Bit 2 = Gyroscope self test
	   Bit 3 = MCU self test
  
	   0x0F = all good! */

	if (self_test_result != 0)
		*self_test_result = read8(BNO055_SELFTEST_RESULT_ADDR);

	/* System Error (see section 4.3.59)
	   ---------------------------------
	   0 = No error
	   1 = Peripheral initialization error
	   2 = System initialization error
	   3 = Self test result failed
	   4 = Register map value out of range
	   5 = Register map address out of range
	   6 = Register map write error
	   7 = BNO low power mode not available for selected operat ion mode
	   8 = Accelerometer power mode not available
	   9 = Fusion algorithm configuration error
	   A = Sensor configuration error */

	if (system_error != 0)
		*system_error = read8(BNO055_SYS_ERR_ADDR);

	usleep(1000000 * 0.001 * 200);
}

/**************************************************************************/
/*!
    @brief  Gets the chip revision numbers
*/
/**************************************************************************/
void BNO055::getRevInfo(adafruit_bno055_rev_info_t *info) {
	uint8_t a, b;

	memset(info, 0, sizeof(adafruit_bno055_rev_info_t));

	/* Check the accelerometer revision */
	info->accel_rev = read8(BNO055_ACCEL_REV_ID_ADDR);

	/* Check the magnetometer revision */
	info->mag_rev = read8(BNO055_MAG_REV_ID_ADDR);

	/* Check the gyroscope revision */
	info->gyro_rev = read8(BNO055_GYRO_REV_ID_ADDR);

	/* Check the SW revision */
	info->bl_rev = read8(BNO055_BL_REV_ID_ADDR);

	a = read8(BNO055_SW_REV_ID_LSB_ADDR);
	b = read8(BNO055_SW_REV_ID_MSB_ADDR);
	info->sw_rev = (((uint16_t) b) << 8) | ((uint16_t) a);
}

/**************************************************************************/
/*!
    @brief  Gets current calibration state.  Each value should be a uint8_t
            pointer and it will be set to 0 if not calibrated and 3 if
            fully calibrated.
*/
/**************************************************************************/
void BNO055::getCalibration(uint8_t *sys, uint8_t *gyro, uint8_t *accel, uint8_t *mag) {
	uint8_t calData = read8(BNO055_CALIB_STAT_ADDR);
	if (sys != NULL) {
		*sys = (calData >> 6) & 0x03;
	}
	if (gyro != NULL) {
		*gyro = (calData >> 4) & 0x03;
	}
	if (accel != NULL) {
		*accel = (calData >> 2) & 0x03;
	}
	if (mag != NULL) {
		*mag = calData & 0x03;
	}
}

/**************************************************************************/
/*!
    @brief  Gets the temperature in degrees celsius
*/
/**************************************************************************/
int8_t BNO055::getTemp(void) {
	int8_t temp = (int8_t) (read8(BNO055_TEMP_ADDR));
	return temp;
}

/**************************************************************************/
/*!
    @brief  Gets a vector reading from the specified source
*/
/**************************************************************************/
imu::Vector<3> BNO055::getVector(adafruit_vector_type_t vector_type) {
	imu::Vector<3> xyz;
	uint8_t buffer[6];
	memset(buffer, 0, 6);

	int16_t x, y, z;
	x = y = z = 0;

	/* Read vector data (6 bytes) */
	readLen((adafruit_bno055_reg_t) vector_type, buffer, 6);

	x = ((int16_t) buffer[0]) | (((int16_t) buffer[1]) << 8);
	y = ((int16_t) buffer[2]) | (((int16_t) buffer[3]) << 8);
	z = ((int16_t) buffer[4]) | (((int16_t) buffer[5]) << 8);

	/* Convert the value to an appropriate range (section 3.6.4) */
	/* and assign the value to the Vector type */
	switch (vector_type) {
		case VECTOR_MAGNETOMETER:
			/* 1uT = 16 LSB */
			xyz[0] = ((double) x) / 16.0;
			xyz[1] = ((double) y) / 16.0;
			xyz[2] = ((double) z) / 16.0;
			break;
		case VECTOR_GYROSCOPE:
			/* 1dps = 16 LSB */
			xyz[0] = ((double) x) / 16.0;
			xyz[1] = ((double) y) / 16.0;
			xyz[2] = ((double) z) / 16.0;
			break;
		case VECTOR_EULER:
			/* 1 degree = 16 LSB */
			xyz[0] = ((double) x) / 16.0;
			xyz[1] = ((double) y) / 16.0;
			xyz[2] = ((double) z) / 16.0;
			break;
		case VECTOR_ACCELEROMETER:
		case VECTOR_LINEARACCEL:
		case VECTOR_GRAVITY:
			/* 1m/s^2 = 100 LSB */
			xyz[0] = ((double) x) / 100.0;
			xyz[1] = ((double) y) / 100.0;
			xyz[2] = ((double) z) / 100.0;
			break;
	}

	return xyz;
}

/**************************************************************************/
/*!
    @brief  Gets a quaternion reading from the specified source
*/
/**************************************************************************/
imu::Quaternion BNO055::getQuat(void) {
	uint8_t buffer[8];
	memset(buffer, 0, 8);

	int16_t x, y, z, w;
	x = y = z = w = 0;

	/* Read quat data (8 bytes) */
	readLen(BNO055_QUATERNION_DATA_W_LSB_ADDR, buffer, 8);
	w = (((uint16_t) buffer[1]) << 8) | ((uint16_t) buffer[0]);
	x = (((uint16_t) buffer[3]) << 8) | ((uint16_t) buffer[2]);
	y = (((uint16_t) buffer[5]) << 8) | ((uint16_t) buffer[4]);
	z = (((uint16_t) buffer[7]) << 8) | ((uint16_t) buffer[6]);

	/* Assign to Quaternion */
	/* See http://ae-bst.resource.bosch.com/media/products/dokumente/bno055/BST_BNO055_DS000_12~1.pdf
	   3.6.5.5 Orientation (Quaternion)  */
	const double scale = (1.0 / (1 << 14));
	imu::Quaternion quat(scale * w, scale * x, scale * y, scale * z);
	return quat;
}

/**************************************************************************/
/*!
    @brief  Provides the sensor_t data for this sensor
*/
/**************************************************************************/
void BNO055::getSensor(sensor_t *sensor) {
	/* Clear the sensor_t object */
	memset(sensor, 0, sizeof(sensor_t));

	/* Insert the sensor name in the fixed length char array */
	strncpy(sensor->name, "BNO055", sizeof(sensor->name) - 1);
	sensor->name[sizeof(sensor->name) - 1] = 0;
	sensor->version = 1;
	sensor->sensor_id = _sensorID;
	sensor->type = SENSOR_TYPE_ORIENTATION;
	sensor->min_delay = 0;
	sensor->max_value = 0.0F;
	sensor->min_value = 0.0F;
	sensor->resolution = 0.01F;
}

/**************************************************************************/
/*!
    @brief  Reads the sensor and returns the data as a sensors_event_t
*/
/**************************************************************************/
bool BNO055::getEvent(sensors_event_t *event) {
	/* Clear the event */
	memset(event, 0, sizeof(sensors_event_t));

	event->version = sizeof(sensors_event_t);
	event->sensor_id = _sensorID;
	event->type = SENSOR_TYPE_ORIENTATION;
	event->timestamp = _millis();

	/* Get a Euler angle sample for orientation */
	imu::Vector<3> euler = getVector(BNO055::VECTOR_EULER);
	event->orientation.x = euler.x();
	event->orientation.y = euler.y();
	event->orientation.z = euler.z();

	return true;
}

/**************************************************************************/
/*!
@brief  Reads the sensor's offset registers into a byte array
*/
/**************************************************************************/
bool BNO055::getSensorOffsets(uint8_t *calibData) {
	if (isFullyCalibrated()) {
		adafruit_bno055_opmode_t lastMode = _mode;
		setMode(OPERATION_MODE_CONFIG);

		readLen(ACCEL_OFFSET_X_LSB_ADDR, calibData, NUM_BNO055_OFFSET_REGISTERS);

		setMode(lastMode);
		return true;
	}
	return false;
}

/**************************************************************************/
/*!
@brief  Reads the sensor's offset registers into an offset struct
*/
/**************************************************************************/
bool BNO055::getSensorOffsets(adafruit_bno055_offsets_t &offsets_type) {
	if (isFullyCalibrated()) {
		adafruit_bno055_opmode_t lastMode = _mode;
		setMode(OPERATION_MODE_CONFIG);
		usleep(1000000 * 0.001 * 25);

		/* Accel offset range depends on the G-range:
		   +/-2g  = +/- 2000 mg
		   +/-4g  = +/- 4000 mg
		   +/-8g  = +/- 8000 mg
		   +/-1§g = +/- 16000 mg */
		offsets_type.accel_offset_x = (read8(ACCEL_OFFSET_X_MSB_ADDR) << 8) | (read8(ACCEL_OFFSET_X_LSB_ADDR));
		offsets_type.accel_offset_y = (read8(ACCEL_OFFSET_Y_MSB_ADDR) << 8) | (read8(ACCEL_OFFSET_Y_LSB_ADDR));
		offsets_type.accel_offset_z = (read8(ACCEL_OFFSET_Z_MSB_ADDR) << 8) | (read8(ACCEL_OFFSET_Z_LSB_ADDR));

		/* Magnetometer offset range = +/- 6400 LSB where 1uT = 16 LSB */
		offsets_type.mag_offset_x = (read8(MAG_OFFSET_X_MSB_ADDR) << 8) | (read8(MAG_OFFSET_X_LSB_ADDR));
		offsets_type.mag_offset_y = (read8(MAG_OFFSET_Y_MSB_ADDR) << 8) | (read8(MAG_OFFSET_Y_LSB_ADDR));
		offsets_type.mag_offset_z = (read8(MAG_OFFSET_Z_MSB_ADDR) << 8) | (read8(MAG_OFFSET_Z_LSB_ADDR));

		/* Gyro offset range depends on the DPS range:
		  2000 dps = +/- 32000 LSB
		  1000 dps = +/- 16000 LSB
		   500 dps = +/- 8000 LSB
		   250 dps = +/- 4000 LSB
		   125 dps = +/- 2000 LSB
		   ... where 1 DPS = 16 LSB */
		offsets_type.gyro_offset_x = (read8(GYRO_OFFSET_X_MSB_ADDR) << 8) | (read8(GYRO_OFFSET_X_LSB_ADDR));
		offsets_type.gyro_offset_y = (read8(GYRO_OFFSET_Y_MSB_ADDR) << 8) | (read8(GYRO_OFFSET_Y_LSB_ADDR));
		offsets_type.gyro_offset_z = (read8(GYRO_OFFSET_Z_MSB_ADDR) << 8) | (read8(GYRO_OFFSET_Z_LSB_ADDR));

		/* Accelerometer radius = +/- 1000 LSB */
		offsets_type.accel_radius = (read8(ACCEL_RADIUS_MSB_ADDR) << 8) | (read8(ACCEL_RADIUS_LSB_ADDR));

		/* Magnetometer radius = +/- 960 LSB */
		offsets_type.mag_radius = (read8(MAG_RADIUS_MSB_ADDR) << 8) | (read8(MAG_RADIUS_LSB_ADDR));

		setMode(lastMode);
		return true;
	}
	return false;
}


/**************************************************************************/
/*!
@brief  Writes an array of calibration values to the sensor's offset registers
*/
/**************************************************************************/
void BNO055::setSensorOffsets(const uint8_t *calibData) {
	adafruit_bno055_opmode_t lastMode = _mode;
	setMode(OPERATION_MODE_CONFIG);
	usleep(1000000 * 0.001 * 25);

	/* Note: Configuration will take place only when user writes to the last
	   byte of each config data pair (ex. ACCEL_OFFSET_Z_MSB_ADDR, etc.).
	   Therefore the last byte must be written whenever the user wants to
	   changes the configuration. */

	/* A writeLen() would make this much cleaner */
	write8(ACCEL_OFFSET_X_LSB_ADDR, calibData[0]);
	write8(ACCEL_OFFSET_X_MSB_ADDR, calibData[1]);
	write8(ACCEL_OFFSET_Y_LSB_ADDR, calibData[2]);
	write8(ACCEL_OFFSET_Y_MSB_ADDR, calibData[3]);
	write8(ACCEL_OFFSET_Z_LSB_ADDR, calibData[4]);
	write8(ACCEL_OFFSET_Z_MSB_ADDR, calibData[5]);

	write8(MAG_OFFSET_X_LSB_ADDR, calibData[6]);
	write8(MAG_OFFSET_X_MSB_ADDR, calibData[7]);
	write8(MAG_OFFSET_Y_LSB_ADDR, calibData[8]);
	write8(MAG_OFFSET_Y_MSB_ADDR, calibData[9]);
	write8(MAG_OFFSET_Z_LSB_ADDR, calibData[10]);
	write8(MAG_OFFSET_Z_MSB_ADDR, calibData[11]);

	write8(GYRO_OFFSET_X_LSB_ADDR, calibData[12]);
	write8(GYRO_OFFSET_X_MSB_ADDR, calibData[13]);
	write8(GYRO_OFFSET_Y_LSB_ADDR, calibData[14]);
	write8(GYRO_OFFSET_Y_MSB_ADDR, calibData[15]);
	write8(GYRO_OFFSET_Z_LSB_ADDR, calibData[16]);
	write8(GYRO_OFFSET_Z_MSB_ADDR, calibData[17]);

	write8(ACCEL_RADIUS_LSB_ADDR, calibData[18]);
	write8(ACCEL_RADIUS_MSB_ADDR, calibData[19]);

	write8(MAG_RADIUS_LSB_ADDR, calibData[20]);
	write8(MAG_RADIUS_MSB_ADDR, calibData[21]);

	setMode(lastMode);
}

/**************************************************************************/
/*!
@brief  Writes to the sensor's offset registers from an offset struct
*/
/**************************************************************************/
void BNO055::setSensorOffsets(const adafruit_bno055_offsets_t &offsets_type) {
	adafruit_bno055_opmode_t lastMode = _mode;
	setMode(OPERATION_MODE_CONFIG);
	usleep(1000000 * 0.001 * 25);

	/* Note: Configuration will take place only when user writes to the last
	   byte of each config data pair (ex. ACCEL_OFFSET_Z_MSB_ADDR, etc.).
	   Therefore the last byte must be written whenever the user wants to
	   changes the configuration. */

	write8(ACCEL_OFFSET_X_LSB_ADDR, (offsets_type.accel_offset_x) & 0x0FF);
	write8(ACCEL_OFFSET_X_MSB_ADDR, (offsets_type.accel_offset_x >> 8) & 0x0FF);
	write8(ACCEL_OFFSET_Y_LSB_ADDR, (offsets_type.accel_offset_y) & 0x0FF);
	write8(ACCEL_OFFSET_Y_MSB_ADDR, (offsets_type.accel_offset_y >> 8) & 0x0FF);
	write8(ACCEL_OFFSET_Z_LSB_ADDR, (offsets_type.accel_offset_z) & 0x0FF);
	write8(ACCEL_OFFSET_Z_MSB_ADDR, (offsets_type.accel_offset_z >> 8) & 0x0FF);

	write8(MAG_OFFSET_X_LSB_ADDR, (offsets_type.mag_offset_x) & 0x0FF);
	write8(MAG_OFFSET_X_MSB_ADDR, (offsets_type.mag_offset_x >> 8) & 0x0FF);
	write8(MAG_OFFSET_Y_LSB_ADDR, (offsets_type.mag_offset_y) & 0x0FF);
	write8(MAG_OFFSET_Y_MSB_ADDR, (offsets_type.mag_offset_y >> 8) & 0x0FF);
	write8(MAG_OFFSET_Z_LSB_ADDR, (offsets_type.mag_offset_z) & 0x0FF);
	write8(MAG_OFFSET_Z_MSB_ADDR, (offsets_type.mag_offset_z >> 8) & 0x0FF);

	write8(GYRO_OFFSET_X_LSB_ADDR, (offsets_type.gyro_offset_x) & 0x0FF);
	write8(GYRO_OFFSET_X_MSB_ADDR, (offsets_type.gyro_offset_x >> 8) & 0x0FF);
	write8(GYRO_OFFSET_Y_LSB_ADDR, (offsets_type.gyro_offset_y) & 0x0FF);
	write8(GYRO_OFFSET_Y_MSB_ADDR, (offsets_type.gyro_offset_y >> 8) & 0x0FF);
	write8(GYRO_OFFSET_Z_LSB_ADDR, (offsets_type.gyro_offset_z) & 0x0FF);
	write8(GYRO_OFFSET_Z_MSB_ADDR, (offsets_type.gyro_offset_z >> 8) & 0x0FF);

	write8(ACCEL_RADIUS_LSB_ADDR, (offsets_type.accel_radius) & 0x0FF);
	write8(ACCEL_RADIUS_MSB_ADDR, (offsets_type.accel_radius >> 8) & 0x0FF);

	write8(MAG_RADIUS_LSB_ADDR, (offsets_type.mag_radius) & 0x0FF);
	write8(MAG_RADIUS_MSB_ADDR, (offsets_type.mag_radius >> 8) & 0x0FF);

	setMode(lastMode);
}

/**************************************************************************/
/*!
    @brief  Checks of all cal status values are set to 3 (fully calibrated)
*/
/**************************************************************************/
bool BNO055::isFullyCalibrated(void) {
	uint8_t system, gyro, accel, mag;
	getCalibration(&system, &gyro, &accel, &mag);
	if (system < 2 || gyro < 3 || accel < 3 || mag < 3)
		return false;
	return true;
}


/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over I2C
*/
/**************************************************************************/
bool BNO055::write8(adafruit_bno055_reg_t reg, byte value) {
	wiringPiI2CWriteReg8(fd, reg, value);

	/* ToDo: Check for error! */
	return true;
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
byte BNO055::read8(adafruit_bno055_reg_t reg) {
	byte value = wiringPiI2CReadReg8(fd, reg);

	return value;
}

/**************************************************************************/
/*!
    @brief  Reads the specified number of bytes over I2C
*/
/**************************************************************************/
bool BNO055::readLen(adafruit_bno055_reg_t reg, byte *buffer, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		buffer[i] = read8((adafruit_bno055_reg_t) ((uint8_t) reg + i));
	}

	/* ToDo: Check for errors! */
	return true;
}


BNO055::~BNO055() {

}


void BNO055::selfTest() {

	cout << "Starte Selbsttest" << endl;

	//In Config Modus wechseln
	setMode(OPERATION_MODE_CONFIG);

	//Selbsttest durchführen
	int sysTrigger = read8(BNO055_SYS_TRIGGER_ADDR);
	write8(BNO055_SYS_TRIGGER_ADDR, sysTrigger | 0x1);

	//Auf Test warten
	usleep(1000000 * 1);

	//Testergebnis lesen
	int selfTest = read8(BNO055_SELFTEST_RESULT_ADDR);

	//Ersten 4Bits sind reserviert und werden ignoriert
	selfTest = selfTest & 0x0F;

	//In Operationsmodus zurückkehren
	setMode(OPERATION_MODE_NDOF);

	//Status und Error Register lesen
	int status = read8(BNO055_SYS_STAT_ADDR);
	int error = read8(BNO055_SYS_ERR_ADDR);

	//Ausgabe Selbsttest
	cout << "Systemstatus: " << status << endl;
	cout << "Systemerror: " << error << endl;
	printf("Selbst Test: 0x0%X (0x0F ist normal)\n", selfTest);
	//cout << "Selbst Test: " << std::hex << selfTest << endl;

	cout << "Ende Selbsttest" << endl;


}

void BNO055::doReset() {
	//Führt einen Reset des BNO055 mittels des Hardware Reset Pin
	//Da es ein optionale Parameter ist, pruefen ob resetPin gesetzt

	if (resetPin != 0) {
		//Fuehre Reset aus
		// Wichtig: Hier wird das WiringPi Layout verwendet
		// Daher zuerst ermitteln welcher Pin es ist

		int wpPin = translatePinWiringPi(resetPin);

		pinMode(wpPin, OUTPUT);

		//Setze Low an
		digitalWrite(wpPin, 0);

		//Warte
		usleep(1000000 * 0.2);

		//Setze High an
		digitalWrite(wpPin, 1);


	}

}

int BNO055::translatePinWiringPi(int pin) {
	//Übersetze welcher Pin für WiringPi benötigt wird
	//Aktuelle Informationen unter http://raspberrypiguide.de/howtos/raspberry-pi-gpio-how-to/

	int wpPin = 0;

	switch (pin) {
		case 2:
			wpPin = 8;
		case 3:
			wpPin = 9;
		case 4:
			wpPin = 7;
		case 7:
			wpPin = 11;
		case 8:
			wpPin = 10;
		case 9:
			wpPin = 13;
		case 10:
			wpPin = 12;
		case 11:
			wpPin = 14;
		case 14:
			wpPin = 15;
		case 15:
			wpPin = 16;
		case 17:
			wpPin = 0;
		case 18:
			wpPin = 1;
		case 22:
			wpPin = 3;
		case 23:
			wpPin = 4;
		case 24:
			wpPin = 5;
		case 25:
			wpPin = 6;
		case 27:
			wpPin = 2;
		default:
			wpPin = 0;

	}

	return wpPin;

}

int BNO055::getTemperature() {
	//Temperaturangaben in Grad Celsius
	int temp = read8(BNO055_TEMP_ADDR);

	return temp;
}

int BNO055::getSoftwareRevision() {
	int swLsb = read8(BNO055_SW_REV_ID_LSB_ADDR);
	int swMsb = read8(BNO055_SW_REV_ID_MSB_ADDR);

	int sw = ((swMsb << 8) | swLsb) & 0xFFFF;

	return sw;
}

int BNO055::getBootloaderRevision() {

	return read8(BNO055_BL_REV_ID_ADDR);
}

int BNO055::getAccelerometerID() {

	return read8(BNO055_ACCEL_REV_ID_ADDR);
}

int BNO055::getMagnetometerID() {

	return read8(BNO055_MAG_REV_ID_ADDR);
}

int BNO055::getGyroID() {

	return read8(BNO055_GYRO_REV_ID_ADDR);
}

//Kalibrierungswerte der einzelnen Einheiten

/*  - System, 3=fully calibrated, 0=not calibrated
    - Gyroscope, 3=fully calibrated, 0=not calibrated
    - Accelerometer, 3=fully calibrated, 0=not calibrated
    - Magnetometer, 3=fully calibrated, 0=not calibrated
*/

int BNO055::getKalibrierungSys() {

	int calStatus = read8(BNO055_CALIB_STAT_ADDR);

	return (calStatus >> 6) & 0x03;

}

int BNO055::getKalibrierungGyro() {

	int calStatus = read8(BNO055_CALIB_STAT_ADDR);

	return (calStatus >> 4) & 0x03;
}

int BNO055::getKalibrierungAcc() {

	int calStatus = read8(BNO055_CALIB_STAT_ADDR);

	return (calStatus >> 2) & 0x03;
}

int BNO055::getKalibrierungMag() {

	int calStatus = read8(BNO055_CALIB_STAT_ADDR);

	return calStatus & 0x03;
}

//Eulersche Winkelangaben
//Winkelangaben in Grad

//Heading Bereich von 0 - 360 (0 = Nord)
double BNO055::getEulerHeading() {

	int heading = 0;

	//Berechne Heading
	do {
		int headingLsb = read8(BNO055_EULER_H_LSB_ADDR);
		int headingMsb = read8(BNO055_EULER_H_MSB_ADDR);

		heading = ((headingMsb << 8) | headingLsb) & 0xFFFF;

		if (heading > 32767) {
			heading -= 65536;
		}


	} while ((heading / 16.0) < -1500); //Wichtig bzg dem I2C Bug, es koennen teilweise unmögliche Werte gelsen werden

	return (heading / 16.0);

}

//Roll Bereich von -90 bis 90
double BNO055::getEulerRoll() {

	int roll = 0;

	//Berechne Roll
	do {
		int rollLsb = read8(BNO055_EULER_R_LSB_ADDR);
		int rollMsb = read8(BNO055_EULER_R_MSB_ADDR);

		roll = ((rollMsb << 8) | rollLsb) & 0xFFFF;

		if (roll > 32767) {
			roll -= 65536;
		}


	} while ((roll / 16.0) < -1500); //Wichtig bzg dem I2C Bug, es koennen teilweise unmögliche Werte gelsen werden

	return (roll / 16.0);
}

//Pitch Bereich von -180 bis 180
double BNO055::getEulerPitch() {

	int pitch = 0;

	//Berechne Pitch
	do {
		int pitchLsb = read8(BNO055_EULER_P_LSB_ADDR);
		int pitchMsb = read8(BNO055_EULER_P_MSB_ADDR);

		pitch = ((pitchMsb << 8) | pitchLsb) & 0xFFFF;

		if (pitch > 32767) {
			pitch -= 65536;
		}


	} while ((pitch / 16.0) < -1500); //Wichtig bzg dem I2C Bug, es koennen teilweise unmögliche Werte gelsen werden

	return (pitch / 16.0);
}

//Gyroskop Werte
//Die Gyroskopwerte sind Winkelgeschwindigkeiten in Grad pro Sekunde
double BNO055::getGyroX() {

	int gyroX = 0;

	//Berechne X Wert
	int gyroXLsb = read8(BNO055_GYRO_DATA_X_LSB_ADDR);
	int gyroXMsb = read8(BNO055_GYRO_DATA_X_MSB_ADDR);

	gyroX = ((gyroXMsb << 8) | gyroXLsb) & 0xFFFF;

	if (gyroX > 32767) {
		gyroX -= 65536;
	}


	return (gyroX / 900.0);

}

double BNO055::getGyroY() {

	int gyroY = 0;

	//Berechne Y Wert
	int gyroYLsb = read8(BNO055_GYRO_DATA_Y_LSB_ADDR);
	int gyroYMsb = read8(BNO055_GYRO_DATA_Y_MSB_ADDR);

	gyroY = ((gyroYMsb << 8) | gyroYLsb) & 0xFFFF;

	if (gyroY > 32767) {
		gyroY -= 65536;
	}


	return (gyroY / 900.0);

}

double BNO055::getGyroZ() {

	int gyroZ = 0;

	//Berechne Z Wert
	int gyroZLsb = read8(BNO055_GYRO_DATA_Z_LSB_ADDR);
	int gyroZMsb = read8(BNO055_GYRO_DATA_Z_MSB_ADDR);

	gyroZ = ((gyroZMsb << 8) | gyroZLsb) & 0xFFFF;

	if (gyroZ > 32767) {
		gyroZ -= 65536;
	}


	return (gyroZ / 900.0);

}

//Magnetometer
//Die Magnetometer Angaben sind in micro Tesla
double BNO055::getMagnetometerX() {

	int magX = 0;

	//Berechne X Wert
	int magXLsb = read8(BNO055_MAG_DATA_X_LSB_ADDR);
	int magXMsb = read8(BNO055_MAG_DATA_X_MSB_ADDR);

	magX = ((magXMsb << 8) | magXLsb) & 0xFFFF;

	if (magX > 32767) {
		magX -= 65536;
	}


	return (magX / 16.0);
}

double BNO055::getMagnetometerY() {

	int magY = 0;

	//Berechne Y Wert
	int magYLsb = read8(BNO055_MAG_DATA_Y_LSB_ADDR);
	int magYMsb = read8(BNO055_MAG_DATA_Y_MSB_ADDR);

	magY = ((magYMsb << 8) | magYLsb) & 0xFFFF;

	if (magY > 32767) {
		magY -= 65536;
	}


	return (magY / 16.0);
}

double BNO055::getMagnetometerZ() {

	int magZ = 0;

	//Berechne Z Wert
	int magZLsb = read8(BNO055_MAG_DATA_Z_LSB_ADDR);
	int magZMsb = read8(BNO055_MAG_DATA_Z_MSB_ADDR);

	magZ = ((magZMsb << 8) | magZLsb) & 0xFFFF;

	if (magZ > 32767) {
		magZ -= 65536;
	}


	return (magZ / 16.0);
}

// Accelerometer
//Die Beschleunigungsdaten sind in Meter / Sekunde ^ 2
double BNO055::getAccelerometerX() {

	int accX = 0;

	//Berechne X Wert
	int accXLsb = read8(BNO055_ACCEL_DATA_X_LSB_ADDR);
	int accXMsb = read8(BNO055_ACCEL_DATA_X_MSB_ADDR);

	accX = ((accXMsb << 8) | accXLsb) & 0xFFFF;

	if (accX > 32767) {
		accX -= 65536;
	}

	return (accX / 100.0);
}

double BNO055::getAccelerometerY() {

	int accY = 0;

	//Berechne Y Wert
	int accYLsb = read8(BNO055_ACCEL_DATA_Y_LSB_ADDR);
	int accYMsb = read8(BNO055_ACCEL_DATA_Y_MSB_ADDR);

	accY = ((accYMsb << 8) | accYLsb) & 0xFFFF;

	if (accY > 32767) {
		accY -= 65536;
	}

	return (accY / 100.0);
}

double BNO055::getAccelerometerZ() {

	int accZ = 0;

	//Berechne Z Wert
	int accZLsb = read8(BNO055_ACCEL_DATA_Z_LSB_ADDR);
	int accZMsb = read8(BNO055_ACCEL_DATA_Z_MSB_ADDR);

	accZ = ((accZMsb << 8) | accZLsb) & 0xFFFF;

	if (accZ > 32767) {
		accZ -= 65536;
	}

	return (accZ / 100.0);
}

//Linear Acceleration
//Die Beschleunigungsdaten sind in Meter / Sekunde ^ 2
double BNO055::getLinearAccelerationX() {

	int accX = 0;

	//Berechne X Wert
	int accXLsb = read8(BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR);
	int accXMsb = read8(BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR);

	accX = ((accXMsb << 8) | accXLsb) & 0xFFFF;

	if (accX > 32767) {
		accX -= 65536;
	}

	return (accX / 100.0);
}

double BNO055::getLinearAccelerationY() {

	int accY = 0;

	//Berechne Y Wert
	int accYLsb = read8(BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR);
	int accYMsb = read8(BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR);

	accY = ((accYMsb << 8) | accYLsb) & 0xFFFF;

	if (accY > 32767) {
		accY -= 65536;
	}

	return (accY / 100.0);
}

double BNO055::getLinearAccelerationZ() {

	int accZ = 0;

	//Berechne Z Wert
	int accZLsb = read8(BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR);
	int accZMsb = read8(BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR);

	accZ = ((accZMsb << 8) | accZLsb) & 0xFFFF;

	if (accZ > 32767) {
		accZ -= 65536;
	}

	return (accZ / 100.0);
}

//Gravity
//Die Gravitationsdaten sind in Meter / Sekunde ^ 2

double BNO055::getGravityX() {

	int gravX = 0;

	//Berechne X Wert
	int gravXLsb = read8(BNO055_GRAVITY_DATA_X_LSB_ADDR);
	int gravXMsb = read8(BNO055_GRAVITY_DATA_X_MSB_ADDR);

	gravX = ((gravXMsb << 8) | gravXLsb) & 0xFFFF;

	if (gravX > 32767) {
		gravX -= 65536;
	}

	return (gravX / 100.0);
}

double BNO055::getGravityY() {

	int gravY = 0;

	//Berechne Y Wert
	int gravYLsb = read8(BNO055_GRAVITY_DATA_Y_LSB_ADDR);
	int gravYMsb = read8(BNO055_GRAVITY_DATA_Y_MSB_ADDR);

	gravY = ((gravYMsb << 8) | gravYLsb) & 0xFFFF;

	if (gravY > 32767) {
		gravY -= 65536;
	}

	return (gravY / 100.0);
}

double BNO055::getGravityZ() {

	int gravZ = 0;

	//Berechne Z Wert
	int gravZLsb = read8(BNO055_GRAVITY_DATA_Z_LSB_ADDR);
	int gravZMsb = read8(BNO055_GRAVITY_DATA_Z_MSB_ADDR);

	gravZ = ((gravZMsb << 8) | gravZLsb) & 0xFFFF;

	if (gravZ > 32767) {
		gravZ -= 65536;
	}

	return (gravZ / 100.0);
}

//Quaternion

double BNO055::getQuaternionX() {

	int quatX = 0;

	//Berechne X Wert
	int quatXLsb = read8(BNO055_QUATERNION_DATA_X_LSB_ADDR);
	int quatXMsb = read8(BNO055_QUATERNION_DATA_X_MSB_ADDR);

	quatX = ((quatXMsb << 8) | quatXLsb) & 0xFFFF;

	if (quatX > 32767) {
		quatX -= 65536;
	}

	int scale = (1.0 / (1 << 14));

	return (quatX * scale);

}

double BNO055::getQuaternionY() {

	int quatY = 0;

	//Berechne Y Wert
	int quatYLsb = read8(BNO055_QUATERNION_DATA_Y_LSB_ADDR);
	int quatYMsb = read8(BNO055_QUATERNION_DATA_Y_MSB_ADDR);

	quatY = ((quatYMsb << 8) | quatYLsb) & 0xFFFF;

	if (quatY > 32767) {
		quatY -= 65536;
	}

	int scale = (1.0 / (1 << 14));

	return (quatY * scale);

}

double BNO055::getQuaternionZ() {

	int quatZ = 0;

	//Berechne Z Wert
	int quatZLsb = read8(BNO055_QUATERNION_DATA_Z_LSB_ADDR);
	int quatZMsb = read8(BNO055_QUATERNION_DATA_Z_MSB_ADDR);

	quatZ = ((quatZMsb << 8) | quatZLsb) & 0xFFFF;

	if (quatZ > 32767) {
		quatZ -= 65536;
	}

	int scale = (1.0 / (1 << 14));

	return (quatZ * scale);

}

double BNO055::getQuaternionW() {

	int quatW = 0;

	//Berechne W Wert
	int quatWLsb = read8(BNO055_QUATERNION_DATA_W_LSB_ADDR);
	int quatWMsb = read8(BNO055_QUATERNION_DATA_W_MSB_ADDR);

	quatW = ((quatWMsb << 8) | quatWLsb) & 0xFFFF;

	if (quatW > 32767) {
		quatW -= 65536;
	}

	int scale = (1.0 / (1 << 14));

	return (quatW * scale);

}

bool BNO055::isMagCalibrated() {
	uint8_t system, gyro, accel, mag;
	getCalibration(&system, &gyro, &accel, &mag);
	if (mag < 3)
		return false;
	return true;
}

long BNO055::_millis() {
	using namespace std::chrono;
	return duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
	).count();
}
