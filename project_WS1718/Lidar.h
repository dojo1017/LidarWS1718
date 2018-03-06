//
// Created by simon on 06.03.18.
//

#ifndef PROJECT_WS1718_LIDAR_H
#define PROJECT_WS1718_LIDAR_H


#include <cstdint>  // uint16_t
#include "libs/lidar/lidarController.h"

class Lidar {
public:
    unsigned int measureDistance();

private:
    lidarController lib;
};


#endif //PROJECT_WS1718_LIDAR_H
