//
// Created by simon on 06.03.18.
//

#include "Gyro.h"

#define I2C_ADDRESS 0x28
#define RESET_PIN 18

Gyro::Gyro()
    : lib(I2C_ADDRESS, RESET_PIN)
{
    cout << "Gyro: BNO055 begin()" << endl;
    lib.begin();
    cout << "Gyro: BNO055 selfTest()" << endl;
    lib.selfTest();

    // TODO: this was copied from the BNO055/main.cpp file
    // TODO: We should probably calibrate until these values
    // reach at least 2 (0 = not calibrated at all, 3 = fully calibrated)

    int calibrationGyro = -1;
    int calibrationMag = -1;
    const int fullyCalibrated = 3;

    while(calibrationGyro < fullyCalibrated && calibrationMag < fullyCalibrated) {
        calibrationGyro = lib.getKalibrierungGyro();
        calibrationMag = lib.getKalibrierungMag();

        cout << "Calibration:" << endl
             << "  Gyro: " << calibrationGyro << "/" << fullyCalibrated << endl
             << "  Mag: " << calibrationMag << "/" << fullyCalibrated << endl;

        sleep(1);
    }

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

double Gyro::getHeading() {
    return lib.getEulerHeading();
}

double Gyro::getPitch() {
    return lib.getEulerPitch();
}