//
// Created by simon on 06.03.18.
//

#include <stdio.h>
#include <math.h>
#include "Merlin.h"
using std::string;

Merlin::Merlin() {

}

// This is a blocking method (blocks until the new position is reached)
void Merlin::aimAt(float heading, float pitch) {
//    printf("Merlin: aiming at heading %.2f pitch %.2f\n", heading, pitch);
    double currHeading = gyro.getHeading();
    double currPitch = gyro.getPitch();
    printf("Merlin: current heading %.2f pitch %.2f\n", currHeading, currPitch);


    // TODO



//    while(true) {
//        // Check where we are currently
//        float heading = gyro.getHeading();
//        float pitch = gyro.getPitch();
//
//        float deltaHeading = targetHeading - heading;
//        float deltaPitch = targetPitch - pitch;
//
//        if(fabsf(deltaHeading) < maxError && fabsf(deltaPitch) < maxError) {
//            // Target reached
//            break;
//        }
//
//        if(deltaHeading > 0.f) {
//            // Move so heading gets smaller
//        } else {
//            // Move so heading gets larger
//        }
//
//        if(deltaPitch > 0.f) {
//            // Move so heading gets smaller
//        } else {
//            // Move so heading gets larger
//        }
//    }
}

void Merlin::addCommand(string command) {
    commands += command;
}

void Merlin::startMotor(string motor) {
    addCommand(":G" + motor + "");
}

void Merlin::stopMotor(string motor) {

}