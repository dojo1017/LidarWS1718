//
// Created by simon on 06.03.18.
//

#ifndef PROJECT_WS1718_GYRO_H
#define PROJECT_WS1718_GYRO_H

#include "libs/gyro/BNO055.h"

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

class Gyro {
public:
    Gyro();
    double getHeading();
    double getPitch();

	void calibrate(const char* calibrationDataFile);

	void calibrateGyroOnly();

private:
    BNO055 lib;

	static const int NOT_CALIBRATED = 0;
	static const int FULLY_CALIBRATED = 3;

	void displaySensorDetails(void);

	void displaySensorStatus(void);

	void displayCalStatus(void);

	void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData);

	bool calibrationDataExists(const char* calibrationDataFile);

	adafruit_bno055_offsets_t loadCalibrationData(const char* calibrationDataFile);

	void saveCalibrationData(adafruit_bno055_offsets_t calib, const char* calibrationDataFile);
};


#endif //PROJECT_WS1718_GYRO_H
