//
// Created by simon on 06.03.18.
//

#include <stdio.h>
#include "Lidar.h"
#include "libs/lidar/lidarLite.h"  // For the status codes

// The returned distance is in cm
unsigned int Lidar::measureDistance() {
    unsigned int distance = lib.getDistance();
    unsigned char status = lib.getStatus();

    if(status) {
        printf("Status Byte: 0x%x\n", status);
        if (status & STAT_BUSY) printf("busy\n");
        if (status & STAT_REF_OVER) printf("reference overflow\n");
        if (status & STAT_SIG_OVER) printf("signal overflow\n");
        if (status & STAT_PIN) printf("mode select pin\n");
        if (status & STAT_SECOND_PEAK) printf("second peak\n");
        if (status & STAT_TIME) printf("active between pairs\n");
        if (status & STAT_INVALID) printf("no signal\n");
        if (status & STAT_EYE) printf("eye safety\n");
    }

    return distance;
}