//
// Created by simon on 06.03.18.
//

#include <fstream>
#include "Gyro.h"
#include "libs/gyro/BNO055.h"

void int16_to_char(int16_t& from, char* to) {
	to[0] = (char)((from >> 8) & 0xFF);
	to[1] = (char)(from & 0xFF);
}

void char_to_int16(char* from, int16_t& to, uint16_t starting) {
	to = (from[starting] << 8) | from[starting + 1];
}


/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
    */
/**************************************************************************/
void Gyro::displaySensorDetails(void) {
	sensor_t sensor;
	lib.getSensor(&sensor);
	printf((string("------------------------------------\n")
			+ "Sensor: %s\n"
			+ "Driver Ver: %d\n"
			+ "Unique ID: %d\n"
			+ "Max Value: %.6f xxx\n"
			+ "Min Value: %.6f xxx\n"
			+ "Resolution: %.6f xxx\n"
			+ "------------------------------------\n\n")
				   .c_str(),
		   sensor.name,
		   sensor.version,
		   sensor.sensor_id,
		   sensor.max_value,
		   sensor.min_value,
		   sensor.resolution);
	sleep(0.5);
}

/**************************************************************************/
/*
    Display some basic info about the sensor status
    */
/**************************************************************************/
void Gyro::displaySensorStatus(void) {
	/* Get the system status values (mostly for debugging purposes) */
	uint8_t system_status, self_test_results, system_error;
	system_status = self_test_results = system_error = 0;
	lib.getSystemStatus(&system_status, &self_test_results, &system_error);

	/* Display the results in the Serial Monitor */
	printf("\nSystem Status: %#08x\nSelf Test:     %#08x\nSystem Error:  %#08x\n\n",
		   system_status,
		   self_test_results,
		   system_error);
	sleep(0.5);
}

/**************************************************************************/
/*
    Display sensor calibration status
    */
/**************************************************************************/
void Gyro::displayCalStatus(void) {
	/* Get the four calibration values (0..3) */
	/* Any sensor data reporting 0 should be ignored, */
	/* 3 means 'fully calibrated" */
	uint8_t system, gyro, accel, mag;
	system = gyro = accel = mag = 0;
	lib.getCalibration(&system, &gyro, &accel, &mag);

	/* The data should be ignored until the system calibration is > 0 */
	printf("\t");
	if (!system) {
		printf("! ");
	}

	/* Display the individual values */
	printf("System: %d, Gyro: %d, Accel: %d, Mag: %d",
		   system,
		   gyro,
		   accel,
		   mag);
}

/**************************************************************************/
/*
    Display the raw calibration offset and radius data
    */
/**************************************************************************/
void Gyro::displaySensorOffsets(const adafruit_bno055_offsets_t &calibData) {
	printf("Accelerometer: %d %d %d\n",
		   calibData.accel_offset_x,
		   calibData.accel_offset_y,
		   calibData.accel_offset_z);

	printf("Gyro: %d %d %d\n",
		   calibData.gyro_offset_x,
		   calibData.gyro_offset_y,
		   calibData.gyro_offset_z);

	printf("Mag: %d %d %d\n",
		   calibData.mag_offset_x,
		   calibData.mag_offset_y,
		   calibData.mag_offset_z);

	printf("Accel Radius: %d\n",
		   calibData.accel_radius);

	printf("Mag Radius: %d\n",
		   calibData.mag_radius);

}


// I2C_ADDRESS can be 0x28 or 0x29, depending on state of pin PS1.
// if connection issues, consult the documentation
#define I2C_ADDRESS 0x28
#define RESET_PIN 18




Gyro::Gyro() : lib(I2C_ADDRESS, RESET_PIN) {

	cout << "Gyro: Initialisierung" << endl;
	if (!lib.begin()) {
		/* There was a problem detecting the BNO055 ... check your connections */
		printf("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
		while (1);
	}

	cout << "Gyro: Selbsttest()" << endl;
	lib.selfTest();



//    // Anzeige der Euler Daten und Kalibrierungsdaten
//    for(int i = 0; i < 100; i++) {
//        //Eulersche Winkelangaben
//        printf("Heading: %4.2f ", lib.getEulerHeading());
//        printf("Roll: %4.2f ", lib.getEulerRoll());
//        printf("Pitch: %4.2f \t\t", lib.getEulerPitch());
//
//        // Kalibrierungsdaten, sollten mind.2 sein um sinnvolle Daten zu erhalten
//        // Sollte das Magnetometer bei < 2 sein handelt es sich um relative Angaben
//        // erst wenn das Magnetometer den Nordpol gefunden hat sind es absolute Angaben
//        cout << "Sys: " << lib.getKalibrierungSys()
//             << " Gyro: " << lib.getKalibrierungGyro()
//             << " Acc: " << lib.getKalibrierungAcc()
//             << " Mag: " << lib.getKalibrierungMag() << endl;
//
//        sleep(1);
//    }
}

bool Gyro::calibrationDataExists(const char* calibrationDataFile) {
	ifstream calibFile(calibrationDataFile);
	bool exists = calibFile.good();
	calibFile.close();

	return exists;
}

void Gyro::saveCalibrationData(adafruit_bno055_offsets_t calib, const char* calibrationDataFile) {
    ofstream outputFile(calibrationDataFile);
    outputFile.write(reinterpret_cast<const char *>(&calib), sizeof(calib));
}

adafruit_bno055_offsets_t Gyro::loadCalibrationData(const char* calibrationDataFile) {
	adafruit_bno055_offsets_t calibData;

    ifstream inputFile(calibrationDataFile);
    inputFile.read(reinterpret_cast<char *>(&calibData), sizeof(calibData));

	return calibData;
}

void Gyro::calibrate(const char* calibrationDataFile) {
	bool foundCalib = false;

	adafruit_bno055_offsets_t calibrationData;
	sensor_t sensor;
	lib.getSensor(&sensor);

	if (!calibrationDataExists(calibrationDataFile))
	{
		printf("\nNo Calibration Data for this sensor exists\n");
		sleep(0.500);
	}
	else
	{
		printf("\nFound Calibration for this sensor.\n");
		calibrationData = loadCalibrationData(calibrationDataFile);

		displaySensorOffsets(calibrationData);

		printf("\n\nRestoring Calibration data to the BNO055...\n");
		lib.setSensorOffsets(calibrationData);

		printf("\n\nCalibration data loaded into BNO055\n");
		foundCalib = true;
	}

	sleep(1);

	/* Display some basic information on this sensor */
	displaySensorDetails();

	/* Optional: Display current status */
	displaySensorStatus();

	//Crystal must be configured AFTER loading calibration data into BNO055.
	lib.setExtCrystalUse(true);

	sensors_event_t event;
	lib.getEvent(&event);
	if (foundCalib){
		printf("Move sensor slightly to calibrate magnetometers\n");
		while (!lib.isFullyCalibrated())
		{
			lib.getEvent(&event);
			printf("X: %.3f\tY: %.3f\tZ: %.3f",
				   event.orientation.x,
				   event.orientation.y,
				   event.orientation.z);

			/* Optional: Display calibration status */
			displayCalStatus();

			/* New line for the next sample */
			printf("\n");

			sleep(0.001 * BNO055_SAMPLERATE_DELAY_MS);
		}
	}
	else
	{
		printf("Please Calibrate Sensor:\n");
		while (!lib.isFullyCalibrated())
		{
			lib.getEvent(&event);

			printf("X: %.3f\tY: %.3f\tZ: %.3f",
				   event.orientation.x,
				   event.orientation.y,
				   event.orientation.z);

			/* Optional: Display calibration status */
			displayCalStatus();

			/* New line for the next sample */
			printf("\r");

			/* Wait the specified delay before requesting new data */
			sleep(0.001 * BNO055_SAMPLERATE_DELAY_MS);
		}
	}

	printf("\nFully calibrated!\n");
	displayCalStatus();
	printf("\n");
	printf("\n--------------------------------\n");
	printf("\nCalibration Results: \n");
	adafruit_bno055_offsets_t newCalib;
	lib.getSensorOffsets(newCalib);
	displaySensorOffsets(newCalib);

	printf("\n\nStoring calibration data...\n");

	saveCalibrationData(newCalib, calibrationDataFile);
	printf("Data stored.\n");

	printf("\n--------------------------------\n");
	sleep(0.500);

}

double Gyro::getHeading() {
	return lib.getEulerHeading();
}

double Gyro::getPitch() {
	return lib.getEulerPitch();
}

void Gyro::calibrateGyroOnly() {
	sensors_event_t event;
	printf("Please Calibrate Sensor:\n");
	while (!lib.isMagCalibrated())
	{
		lib.getEvent(&event);

		printf("X: %.3f\tY: %.3f\tZ: %.3f",
			   event.orientation.x,
			   event.orientation.y,
			   event.orientation.z);

		/* Optional: Display calibration status */
		displayCalStatus();

		/* New line for the next sample */
		printf("\r");

		/* Wait the specified delay before requesting new data */
		sleep(0.001 * BNO055_SAMPLERATE_DELAY_MS);
	}
}
