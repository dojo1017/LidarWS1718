//
// Created by simon on 06.03.18.
//

#ifndef PROJECT_WS1718_GYRO_H
#define PROJECT_WS1718_GYRO_H

#include "libs/gyro/BNO055.h"

class Gyro {
public:
    Gyro();
    double getHeading();
    double getPitch();

private:
    BNO055 lib;
};


#endif //PROJECT_WS1718_GYRO_H
