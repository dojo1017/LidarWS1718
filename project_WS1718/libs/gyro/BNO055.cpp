#include "BNO055.h"


BNO055::BNO055(int address, int reset = 0) {

    //Standardadresse lautet 0x28
    //Wird PS1 gesetzt ist die Adresse 0x29
    wiringPiSetup () ;
    fd = wiringPiI2CSetup (address) ; /*Use i2cdetect command to find your respective device address*/

    resetPin = reset;

    if (fd == -1){
        cout << "Keine Verbindung zum BNO055 möglich! Verkabelung korrekt?" << endl;
    }

}

BNO055::~BNO055() {

}


void BNO055::setMode(int mode){
    wiringPiI2CWriteReg8 (fd, BNO055_OPR_MODE_ADDR, mode & 0xFF);

    sleep(0.03);
}

void BNO055::begin(){

    //Pruefe Status vom BNO055
    wiringPiI2CWriteReg8 (fd, BNO055_PAGE_ID_ADDR, 0);

    //Config Modus setzen
    setMode(OPERATION_MODE_CONFIG);
    wiringPiI2CWriteReg8 (fd, BNO055_PAGE_ID_ADDR, 0);

    //Chip ID ueberpruefen
    int bnoId = wiringPiI2CReadReg8 (fd, BNO055_CHIP_ID_ADDR);

    if(bnoId != BNO055_ID){
        //Sollte die ID nicht stimmen wird ein Reset durchgefuehrt
        doReset();
    }

    //Warte 650ms wie im Datenblatt beschrieben
    sleep(0.65);

    //Setze normalen Power Modus
    wiringPiI2CWriteReg8 (fd, BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);

    //Default auf internen Oscillator
    wiringPiI2CWriteReg8 (fd, BNO055_SYS_TRIGGER_ADDR, 0x0);

    //Normaler Operationsmodus
    setMode(OPERATION_MODE_NDOF);
}

void BNO055::selfTest(){

    cout << "Starte Selbsttest" << endl;

    //In Config Modus wechseln
    setMode(OPERATION_MODE_CONFIG);

    //Selbsttest durchführen
    int sysTrigger = wiringPiI2CReadReg8 (fd, BNO055_SYS_TRIGGER_ADDR);
    wiringPiI2CWriteReg8 (fd, BNO055_SYS_TRIGGER_ADDR, sysTrigger | 0x1);

    //Auf Test warten
    sleep(1);

    //Testergebnis lesen
    int selfTest = wiringPiI2CReadReg8 (fd, BNO055_SELFTEST_RESULT_ADDR);

    //Ersten 4Bits sind reserviert und werden ignoriert
    selfTest = selfTest & 0x0F;

    //In Operationsmodus zurückkehren
    setMode(OPERATION_MODE_NDOF);

    //Status und Error Register lesen
    int status = wiringPiI2CReadReg8 (fd, BNO055_SYS_STAT_ADDR);
    int error = wiringPiI2CReadReg8 (fd, BNO055_SYS_ERR_ADDR);

    //Ausgabe Selbsttest
    cout << "Systemstatus: " <<  status << endl;
    cout << "Systemerror: " <<  error << endl;
    printf("Selbst Test: 0x0%X (0x0F ist normal)\n", selfTest);
    //cout << "Selbst Test: " << std::hex << selfTest << endl;

    cout << "Ende Selbsttest" << endl;


}

void BNO055::doReset(){
    //Führt einen Reset des BNO055 mittels des Hardware Reset Pin
    //Da es ein optionale Parameter ist, pruefen ob resetPin gesetzt

    if(resetPin != 0){
        //Fuehre Reset aus
        // Wichtig: Hier wird das WiringPi Layout verwendet
        // Daher zuerst ermitteln welcher Pin es ist

        int wpPin = translatePinWiringPi(resetPin);

        pinMode(wpPin, OUTPUT);

        //Setze Low an
        digitalWrite(wpPin, 0);

        //Warte
        sleep(0.2);

        //Setze High an
        digitalWrite(wpPin, 1);


    }

}

int BNO055::translatePinWiringPi(int pin){
    //Übersetze welcher Pin für WiringPi benötigt wird
    //Aktuelle Informationen unter http://raspberrypiguide.de/howtos/raspberry-pi-gpio-how-to/

    int wpPin = 0;

    switch(pin){
        case 2: wpPin = 8;
        case 3: wpPin = 9;
        case 4: wpPin = 7;
        case 7: wpPin = 11;
        case 8: wpPin = 10;
        case 9: wpPin = 13;
        case 10: wpPin = 12;
        case 11: wpPin = 14;
        case 14: wpPin = 15;
        case 15: wpPin = 16;
        case 17: wpPin = 0;
        case 18: wpPin = 1;
        case 22: wpPin = 3;
        case 23: wpPin = 4;
        case 24: wpPin = 5;
        case 25: wpPin = 6;
        case 27: wpPin = 2;
        default: wpPin = 0;

    }

    return wpPin;

}

int BNO055::getTemperature(){
    //Temperaturangaben in Grad Celsius
    int temp = wiringPiI2CReadReg8 (fd, BNO055_TEMP_ADDR);

    return temp;
}

int BNO055::getSoftwareRevision(){
    int swLsb = wiringPiI2CReadReg8 (fd, BNO055_SW_REV_ID_LSB_ADDR);
    int swMsb = wiringPiI2CReadReg8 (fd, BNO055_SW_REV_ID_MSB_ADDR);

    int sw = ((swMsb << 8) | swLsb ) & 0xFFFF;

    return sw;
}

int BNO055::getBootloaderRevision(){

    return wiringPiI2CReadReg8 (fd, BNO055_BL_REV_ID_ADDR);
}

int BNO055::getAccelerometerID(){

    return wiringPiI2CReadReg8 (fd, BNO055_ACCEL_REV_ID_ADDR);
}

int BNO055::getMagnetometerID(){

    return wiringPiI2CReadReg8 (fd, BNO055_MAG_REV_ID_ADDR);
}

int BNO055::getGyroID(){

    return wiringPiI2CReadReg8 (fd, BNO055_GYRO_REV_ID_ADDR);
}

//Kalibrierungswerte der einzelnen Einheiten

/*  - System, 3=fully calibrated, 0=not calibrated
    - Gyroscope, 3=fully calibrated, 0=not calibrated
    - Accelerometer, 3=fully calibrated, 0=not calibrated
    - Magnetometer, 3=fully calibrated, 0=not calibrated
*/

int BNO055::getKalibrierungSys(){

    int calStatus = wiringPiI2CReadReg8 (fd, BNO055_CALIB_STAT_ADDR);

    return (calStatus >> 6) & 0x03;

}

int BNO055::getKalibrierungGyro(){

    int calStatus = wiringPiI2CReadReg8 (fd, BNO055_CALIB_STAT_ADDR);

    return (calStatus >> 4) & 0x03;
}

int BNO055::getKalibrierungAcc(){

    int calStatus = wiringPiI2CReadReg8 (fd, BNO055_CALIB_STAT_ADDR);

    return (calStatus >> 2) & 0x03;
}

int BNO055::getKalibrierungMag(){

    int calStatus = wiringPiI2CReadReg8 (fd, BNO055_CALIB_STAT_ADDR);

    return calStatus & 0x03;
}

//Eulersche Winkelangaben
//Winkelangaben in Grad

//Heading Bereich von 0 - 360 (0 = Nord)
double BNO055::getEulerHeading(){

    int heading = 0;

    //Berechne Heading
    do {
        int headingLsb = wiringPiI2CReadReg8 (fd, BNO055_EULER_H_LSB_ADDR);
        int headingMsb = wiringPiI2CReadReg8 (fd, BNO055_EULER_H_MSB_ADDR);

        heading = ((headingMsb << 8) | headingLsb) & 0xFFFF;

        if(heading > 32767){
            heading -= 65536;
        }


    } while((heading /16.0) < -1500 ); //Wichtig bzg dem I2C Bug, es koennen teilweise unmögliche Werte gelsen werden

    return (heading / 16.0);

}

//Roll Bereich von -90 bis 90
double BNO055::getEulerRoll(){

    int roll = 0;

    //Berechne Roll
    do {
        int rollLsb = wiringPiI2CReadReg8 (fd, BNO055_EULER_R_LSB_ADDR);
        int rollMsb = wiringPiI2CReadReg8 (fd, BNO055_EULER_R_MSB_ADDR);

        roll = ((rollMsb << 8) | rollLsb) & 0xFFFF;

        if(roll > 32767){
            roll -= 65536;
        }


    } while((roll /16.0) < -1500 ); //Wichtig bzg dem I2C Bug, es koennen teilweise unmögliche Werte gelsen werden

    return (roll / 16.0);
}

//Pitch Bereich von -180 bis 180
double BNO055::getEulerPitch(){

    int pitch = 0;

    //Berechne Pitch
    do {
        int pitchLsb = wiringPiI2CReadReg8 (fd, BNO055_EULER_P_LSB_ADDR);
        int pitchMsb = wiringPiI2CReadReg8 (fd, BNO055_EULER_P_MSB_ADDR);

        pitch = ((pitchMsb << 8) | pitchLsb) & 0xFFFF;

        if(pitch > 32767){
            pitch -= 65536;
        }


    } while((pitch /16.0) < -1500 ); //Wichtig bzg dem I2C Bug, es koennen teilweise unmögliche Werte gelsen werden

    return (pitch / 16.0);
}

//Gyroskop Werte
//Die Gyroskopwerte sind Winkelgeschwindigkeiten in Grad pro Sekunde
double BNO055::getGyroX(){

    int gyroX = 0;

    //Berechne X Wert
        int gyroXLsb = wiringPiI2CReadReg8 (fd, BNO055_GYRO_DATA_X_LSB_ADDR);
        int gyroXMsb = wiringPiI2CReadReg8 (fd, BNO055_GYRO_DATA_X_MSB_ADDR);

        gyroX = ((gyroXMsb << 8) | gyroXLsb) & 0xFFFF;

        if(gyroX > 32767){
            gyroX -= 65536;
        }


    return (gyroX / 900.0);

}

double BNO055::getGyroY(){

    int gyroY = 0;

    //Berechne Y Wert
    int gyroYLsb = wiringPiI2CReadReg8 (fd, BNO055_GYRO_DATA_Y_LSB_ADDR);
    int gyroYMsb = wiringPiI2CReadReg8 (fd, BNO055_GYRO_DATA_Y_MSB_ADDR);

    gyroY = ((gyroYMsb << 8) | gyroYLsb) & 0xFFFF;

    if(gyroY > 32767){
        gyroY -= 65536;
    }


    return (gyroY / 900.0);

}

double BNO055::getGyroZ(){

    int gyroZ = 0;

    //Berechne Z Wert
    int gyroZLsb = wiringPiI2CReadReg8 (fd, BNO055_GYRO_DATA_Z_LSB_ADDR);
    int gyroZMsb = wiringPiI2CReadReg8 (fd, BNO055_GYRO_DATA_Z_MSB_ADDR);

    gyroZ = ((gyroZMsb << 8) | gyroZLsb) & 0xFFFF;

    if(gyroZ > 32767){
        gyroZ -= 65536;
    }


    return (gyroZ / 900.0);

}

//Magnetometer
//Die Magnetometer Angaben sind in micro Tesla
double BNO055::getMagnetometerX(){

    int magX = 0;

    //Berechne X Wert
    int magXLsb = wiringPiI2CReadReg8 (fd, BNO055_MAG_DATA_X_LSB_ADDR);
    int magXMsb = wiringPiI2CReadReg8 (fd, BNO055_MAG_DATA_X_MSB_ADDR);

    magX = ((magXMsb << 8) | magXLsb) & 0xFFFF;

    if(magX > 32767){
        magX -= 65536;
    }


    return (magX / 16.0);
}

double BNO055::getMagnetometerY(){

    int magY = 0;

    //Berechne Y Wert
    int magYLsb = wiringPiI2CReadReg8 (fd, BNO055_MAG_DATA_Y_LSB_ADDR);
    int magYMsb = wiringPiI2CReadReg8 (fd, BNO055_MAG_DATA_Y_MSB_ADDR);

    magY = ((magYMsb << 8) | magYLsb) & 0xFFFF;

    if(magY > 32767){
        magY -= 65536;
    }


    return (magY / 16.0);
}

double BNO055::getMagnetometerZ(){

    int magZ = 0;

    //Berechne Z Wert
    int magZLsb = wiringPiI2CReadReg8 (fd, BNO055_MAG_DATA_Z_LSB_ADDR);
    int magZMsb = wiringPiI2CReadReg8 (fd, BNO055_MAG_DATA_Z_MSB_ADDR);

    magZ = ((magZMsb << 8) | magZLsb) & 0xFFFF;

    if(magZ > 32767){
        magZ -= 65536;
    }


    return (magZ / 16.0);
}

// Accelerometer
//Die Beschleunigungsdaten sind in Meter / Sekunde ^ 2
double BNO055::getAccelerometerX(){

    int accX = 0;

    //Berechne X Wert
    int accXLsb = wiringPiI2CReadReg8 (fd, BNO055_ACCEL_DATA_X_LSB_ADDR);
    int accXMsb = wiringPiI2CReadReg8 (fd, BNO055_ACCEL_DATA_X_MSB_ADDR);

    accX = ((accXMsb << 8) | accXLsb) & 0xFFFF;

    if(accX > 32767){
        accX -= 65536;
    }

    return (accX / 100.0);
}

double BNO055::getAccelerometerY(){

    int accY = 0;

    //Berechne Y Wert
    int accYLsb = wiringPiI2CReadReg8 (fd, BNO055_ACCEL_DATA_Y_LSB_ADDR);
    int accYMsb = wiringPiI2CReadReg8 (fd, BNO055_ACCEL_DATA_Y_MSB_ADDR);

    accY = ((accYMsb << 8) | accYLsb) & 0xFFFF;

    if(accY > 32767){
        accY -= 65536;
    }

    return (accY / 100.0);
}

double BNO055::getAccelerometerZ(){

    int accZ = 0;

    //Berechne Z Wert
    int accZLsb = wiringPiI2CReadReg8 (fd, BNO055_ACCEL_DATA_Z_LSB_ADDR);
    int accZMsb = wiringPiI2CReadReg8 (fd, BNO055_ACCEL_DATA_Z_MSB_ADDR);

    accZ = ((accZMsb << 8) | accZLsb) & 0xFFFF;

    if(accZ > 32767){
        accZ -= 65536;
    }

    return (accZ / 100.0);
}

//Linear Acceleration
//Die Beschleunigungsdaten sind in Meter / Sekunde ^ 2
double BNO055::getLinearAccelerationX(){

    int accX = 0;

    //Berechne X Wert
    int accXLsb = wiringPiI2CReadReg8 (fd, BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR);
    int accXMsb = wiringPiI2CReadReg8 (fd, BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR);

    accX = ((accXMsb << 8) | accXLsb) & 0xFFFF;

    if(accX > 32767){
        accX -= 65536;
    }

    return (accX / 100.0);
}

double BNO055::getLinearAccelerationY(){

    int accY = 0;

    //Berechne Y Wert
    int accYLsb = wiringPiI2CReadReg8 (fd, BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR);
    int accYMsb = wiringPiI2CReadReg8 (fd, BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR);

    accY = ((accYMsb << 8) | accYLsb) & 0xFFFF;

    if(accY > 32767){
        accY -= 65536;
    }

    return (accY / 100.0);
}

double BNO055::getLinearAccelerationZ(){

    int accZ = 0;

    //Berechne Z Wert
    int accZLsb = wiringPiI2CReadReg8 (fd, BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR);
    int accZMsb = wiringPiI2CReadReg8 (fd, BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR);

    accZ = ((accZMsb << 8) | accZLsb) & 0xFFFF;

    if(accZ > 32767){
        accZ -= 65536;
    }

    return (accZ / 100.0);
}

//Gravity
//Die Gravitationsdaten sind in Meter / Sekunde ^ 2

double BNO055::getGravityX(){

    int gravX = 0;

    //Berechne X Wert
    int gravXLsb = wiringPiI2CReadReg8 (fd, BNO055_GRAVITY_DATA_X_LSB_ADDR);
    int gravXMsb = wiringPiI2CReadReg8 (fd, BNO055_GRAVITY_DATA_X_MSB_ADDR);

    gravX = ((gravXMsb << 8) | gravXLsb) & 0xFFFF;

    if(gravX > 32767){
        gravX -= 65536;
    }

    return (gravX / 100.0);
}

double BNO055::getGravityY(){

    int gravY = 0;

    //Berechne Y Wert
    int gravYLsb = wiringPiI2CReadReg8 (fd, BNO055_GRAVITY_DATA_Y_LSB_ADDR);
    int gravYMsb = wiringPiI2CReadReg8 (fd, BNO055_GRAVITY_DATA_Y_MSB_ADDR);

    gravY = ((gravYMsb << 8) | gravYLsb) & 0xFFFF;

    if(gravY > 32767){
        gravY -= 65536;
    }

    return (gravY / 100.0);
}

double BNO055::getGravityZ(){

    int gravZ = 0;

    //Berechne Z Wert
    int gravZLsb = wiringPiI2CReadReg8 (fd, BNO055_GRAVITY_DATA_Z_LSB_ADDR);
    int gravZMsb = wiringPiI2CReadReg8 (fd, BNO055_GRAVITY_DATA_Z_MSB_ADDR);

    gravZ = ((gravZMsb << 8) | gravZLsb) & 0xFFFF;

    if(gravZ > 32767){
        gravZ -= 65536;
    }

    return (gravZ / 100.0);
}

//Quaternion

double BNO055::getQuaternionX(){

    int quatX = 0;

    //Berechne X Wert
    int quatXLsb = wiringPiI2CReadReg8 (fd, BNO055_QUATERNION_DATA_X_LSB_ADDR);
    int quatXMsb = wiringPiI2CReadReg8 (fd, BNO055_QUATERNION_DATA_X_MSB_ADDR);

    quatX = ((quatXMsb << 8) | quatXLsb) & 0xFFFF;

    if(quatX > 32767){
        quatX -= 65536;
    }

    int scale = (1.0 / (1<<14));

    return (quatX * scale);

}

double BNO055::getQuaternionY(){

    int quatY = 0;

    //Berechne Y Wert
    int quatYLsb = wiringPiI2CReadReg8 (fd, BNO055_QUATERNION_DATA_Y_LSB_ADDR);
    int quatYMsb = wiringPiI2CReadReg8 (fd, BNO055_QUATERNION_DATA_Y_MSB_ADDR);

    quatY = ((quatYMsb << 8) | quatYLsb) & 0xFFFF;

    if(quatY > 32767){
        quatY -= 65536;
    }

    int scale = (1.0 / (1<<14));

    return (quatY * scale);

}

double BNO055::getQuaternionZ(){

    int quatZ = 0;

    //Berechne Z Wert
    int quatZLsb = wiringPiI2CReadReg8 (fd, BNO055_QUATERNION_DATA_Z_LSB_ADDR);
    int quatZMsb = wiringPiI2CReadReg8 (fd, BNO055_QUATERNION_DATA_Z_MSB_ADDR);

    quatZ = ((quatZMsb << 8) | quatZLsb) & 0xFFFF;

    if(quatZ > 32767){
        quatZ -= 65536;
    }

    int scale = (1.0 / (1<<14));

    return (quatZ * scale);

}

double BNO055::getQuaternionW(){

    int quatW = 0;

    //Berechne W Wert
    int quatWLsb = wiringPiI2CReadReg8 (fd, BNO055_QUATERNION_DATA_W_LSB_ADDR);
    int quatWMsb = wiringPiI2CReadReg8 (fd, BNO055_QUATERNION_DATA_W_MSB_ADDR);

    quatW = ((quatWMsb << 8) | quatWLsb) & 0xFFFF;

    if(quatW > 32767){
        quatW -= 65536;
    }

    int scale = (1.0 / (1<<14));

    return (quatW * scale);

}