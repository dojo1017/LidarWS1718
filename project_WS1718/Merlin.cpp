//
// Created by simon on 06.03.18.
//

#include <stdio.h>
#include <math.h>
#include <sstream
#include <iomanip>
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

void Merlin::addCommand(string command, bool lineEnd) {
    commands += command;
    if(lineEnd) {
        commands += "\r";
    }
}

void Merlin::startMotor(string motor) {
    addCommand(":J" + motor);
}

void Merlin::stopMotor(string motor) {
    addCommand(":L" + motor);
}

void Merlin::moveHeadingTo(float degrees) {
    // The magic number 8388608 is from the Chronomotion code
    long pos = degrees * stepsHeading / 360.f + 8388608;
    string posString = positionToString(pos);

    stopMotor(motorHeading);
    addCommand(":G" + motorHeading + "40");
    addCommand(":S" + motorHeading + posString);
    startMotor(motorHeading);
}

string Merlin::positionToString(int pos) {
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(6)
           << std::uppercase
           << std::hex << pos;
    string temp(stream.str());

    string result = temp;
    result[0] = temp[4];
    result[1] = temp[5];
    result[2] = temp[2];
    result[3] = temp[3];
    result[4] = temp[0];
    result[5] = temp[1];

    return result;
}

//private String TranslatePosition(int pos) {
//    String temp;
//    temp = Integer.toHexString(pos);
//    StringBuilder dest = new StringBuilder(temp);
//    for (int i = temp.length(); i < 6; i++) {
//        dest.append("0");
//    }
//
//    StringBuilder dest2 = new StringBuilder("");
//    dest2.append(dest.charAt(4));
//    dest2.append(dest.charAt(5));
//
//    dest2.append(dest.charAt(2));
//    dest2.append(dest.charAt(3));
//
//    dest2.append(dest.charAt(0));
//    dest2.append(dest.charAt(1));
//
//    return dest2.toString().toUpperCase();
//}