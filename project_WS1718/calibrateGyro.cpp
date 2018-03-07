//
// Created by Angelo on 07.03.2018.
//

#include "Gyro.h"

int main(int argc, char **argv) {
	if(argc > 1 && argv[1] == "--reset") {

	}

	Gyro *gyro = new Gyro();
	gyro->calibrate();

	// TODO: load calibration data

	// TODO: Show current calibration
	// TODO: X needs calibration, calibrate now? (Y/n)
	// TODO: Please do ... for up to 30 seconds to calibrate
	// TODO: calibrate next

	// TODO: save calibration data

}