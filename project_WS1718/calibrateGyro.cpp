//
// Created by Angelo on 07.03.2018.
//

#include "Gyro.h"

int main(int argc, char **argv) {

	Gyro *gyro = new Gyro();
	gyro->calibrate("calibration.dat");

}