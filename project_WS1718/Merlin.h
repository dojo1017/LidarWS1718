//
// Created by simon on 06.03.18.
//

#ifndef PROJECT_WS1718_MERLIN_H
#define PROJECT_WS1718_MERLIN_H

#include "Gyro.h"

class Merlin {
public:
    Merlin();
    void aimAt(float latitude, float longitude);

private:
    Gyro gyro;
};


#endif //PROJECT_WS1718_MERLIN_H
