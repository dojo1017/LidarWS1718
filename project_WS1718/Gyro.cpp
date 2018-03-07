//
// Created by simon on 06.03.18.
//

#include <fstream>
#include "Gyro.h"
#include "libs/gyro/BNO055.h"

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

bool Gyro::calibrationDataExists() {
	ifstream calibFile("calibration.dat");
	bool exists = calibFile.good();
	calibFile.close();

	return exists;
}

void Gyro::saveCalibrationData(adafruit_bno055_offsets_t calib) {
	int16_t calibData[11];
	calibData[0] = calib.accel_offset_x;
	calibData[1] = calib.accel_offset_y;
	calibData[2] = calib.accel_offset_z;
	calibData[3] = calib.mag_offset_x;
	calibData[4] = calib.mag_offset_y;
	calibData[5] = calib.mag_offset_z;
	calibData[6] = calib.gyro_offset_x;
	calibData[7] = calib.gyro_offset_y;
	calibData[8] = calib.gyro_offset_z;

	calibData[9] = calib.accel_radius;
	calibData[10] = calib.mag_radius;

	ofstream outputFile("calibration.dat");
	outputFile << calibData;
	outputFile.close();
}

void int16_to_char(int16_t& from, char* to) {
	to[0] = (char)((from >> 8) & 0xFF);
	to[1] = (char)(from & 0xFF);
}

void char_to_int16(char* from, int16_t& to, uint16_t starting) {
	to = (from[starting] << 8) | from[starting + 1];
}

adafruit_bno055_offsets_t Gyro::loadCalibrationData() {
	adafruit_bno055_offsets_t calibData;
	char readData[22];

	ifstream inputFile("calibration.dat");
	inputFile.read(readData, 22);
	inputFile.close();

	char_to_int16(readData, calibData.accel_offset_x, 0);
	char_to_int16(readData, calibData.accel_offset_y, 2);
	char_to_int16(readData, calibData.accel_offset_z, 4);
	char_to_int16(readData, calibData.mag_offset_x, 6);
	char_to_int16(readData, calibData.mag_offset_y, 8);
	char_to_int16(readData, calibData.mag_offset_z, 10);
	char_to_int16(readData, calibData.gyro_offset_x, 12);
	char_to_int16(readData, calibData.gyro_offset_y, 14);
	char_to_int16(readData, calibData.gyro_offset_z, 16);
	char_to_int16(readData, calibData.accel_radius, 18);
	char_to_int16(readData, calibData.mag_radius, 20);


	return calibData;
}

void Gyro::accessCalibrationData() {
	bool foundCalib = false;

	adafruit_bno055_offsets_t calibrationData;
	sensor_t sensor;
	lib.getSensor(&sensor);

	if (!calibrationDataExists())
	{
		printf("\nNo Calibration Data for this sensor exists\n");
		sleep(0.500);
	}
	else
	{
		printf("\nFound Calibration for this sensor.\n");
		calibrationData = loadCalibrationData();

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
			printf("X: %d\tY: %d\tZ: %d",
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

			printf("X: %d\tY: %d\tZ: %d",
				   event.orientation.x,
				   event.orientation.y,
				   event.orientation.z);

			/* Optional: Display calibration status */
			displayCalStatus();

			/* New line for the next sample */
			printf("\n");

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

	lib.getSensor(&sensor);
	saveCalibrationData(newCalib);
	printf("Data stored.\n");

	printf("\n--------------------------------\n");
	sleep(0.500);
}

void Gyro::calibrate() {
	accessCalibrationData();
}

double Gyro::getHeading() {
	return lib.getEulerHeading();
}

double Gyro::getPitch() {
	return lib.getEulerPitch();
}
