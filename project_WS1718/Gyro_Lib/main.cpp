#include <iostream>

#include "BNO055.h"

#include <wiringPi.h>
#include <wiringPiI2C.h>


using namespace std;

int main() {

    BNO055 *bno;

    cout << "Starte Test" << endl;

    cout << "Verbinde BNO055" << endl;
    //Uebergebe die I2C Adresse an die Bibliothek sowie optional der Reset Pin (GPIO Nummer)
    //Standard ist 0x28, sollte Pin PS1 gesetzt sein wird die Adresse 0x29
    bno = new BNO055(0x28, 18);

    cout << "Initialisiere BNO055" << endl;
    //Grundfunktion zum initialisieren des BNO055
    bno->begin();

    //Gebe Revisionsdaten aus
    cout << "SW Revision: " << bno->getSoftwareRevision() << endl;
    cout << "BL Revision: " << bno->getBootloaderRevision() << endl;
    cout << "Mag ID: " << bno->getMagnetometerID() << endl;
    cout << "Gyro ID: " << bno->getGyroID() << endl;
    cout << "Acc ID: " << bno->getAccelerometerID() << endl;
    cout << endl;

    //Falls nötig einen Selbsttest durchführen
    //Es hat sich als bewährt erwiesen diesen nach der Initialisation immer asuzufuehren
    cout << "Fuehre Selbsttest aus" << endl;
    bno->selfTest();


    //Anzeige der Euler Daten und Kalibrierungsdaten
    for(int i = 0; i <100; i++){
        //Eulersche Winkelangaben
        printf("Heading: %4.2f ", bno->getEulerHeading());
        printf("Roll: %4.2f ", bno->getEulerRoll());
        printf("Pitch: %4.2f \t\t", bno->getEulerPitch());

        //Kalibrierungsdaten, sollten mind.2 sein um sinnvole Daten zuerhalten
        //Sollte das Magnetometer bei < 2 sein handelt es sich um relative Angaben
        //erst wenn das Magnetometer den Nordpol gefunden hat sind es absolute Angaben
        cout << "Sys: " << bno->getKalibrierungSys() << " Gyro: " << bno->getKalibrierungGyro() << " Acc: " << bno->getKalibrierungAcc() << " Mag: " << bno->getKalibrierungMag() << endl;

        sleep(1);
    }

    cout << "Ende Test" << endl;
    return 0;
}

