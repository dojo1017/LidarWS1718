//
// Created by simon on 06.03.18.
//

#ifndef PROJECT_WS1718_MERLIN_H
#define PROJECT_WS1718_MERLIN_H

#include <string>
#include "Gyro.h"

class Merlin {
public:
    Merlin();
    void init();
    void aimAt(float targetHeading, float targetPitch);

private:
    Gyro gyro;
    const float maxErrorHeading = 1.f;
    const float maxErrorPitch = 1.f;
    const std::string motorHeading = "1";
    const std::string motorPitch = "2";
    const int right_up_direction = 0;
    const int left_down_direction = 1;
    const float stepsHeading = 1.f; // TODO
    const float stepsPitch = 1.f; // TODO
    // Delay between characters when sending commands, in nanoseconds
    const unsigned int delay = 10000;
    string commands;
    string recvBuffer;

    void addCommand(std::string command, bool lineEnd=true);
    void startMotor(std::string motor);
    void stopMotor(std::string motor);
    void moveHeadingTo(float degrees);
    bool isHeadingMoving();
    bool isPitchMoving();
    std::string positionToString(int pos);
    int openUART();
    void communicate();
    void moveMotor(std::string motor, int direction);
    void printBuffer(std::string buffer);
};


#endif //PROJECT_WS1718_MERLIN_H
