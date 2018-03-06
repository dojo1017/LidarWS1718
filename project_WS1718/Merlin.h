//
// Created by simon on 06.03.18.
//

#ifndef PROJECT_WS1718_MERLIN_H
#define PROJECT_WS1718_MERLIN_H

#include "Gyro.h"

class Merlin {
public:
    Merlin();
    void aimAt(float targetHeading, float targetPitch);

private:
    Gyro gyro;
    const float maxError = 1.f;
};


#endif //PROJECT_WS1718_MERLIN_H
