//
// Created by simon on 06.03.18.
//

#ifndef PROJECT_WS1718_MERLIN_H
#define PROJECT_WS1718_MERLIN_H

#include <string>
#include "Gyro.h"

#define CALIBRATION_FILENAME "calibration.dat"

class Merlin {
public:
    typedef enum {
        FAST = 17,
        NORMAL = 34,
        ALTERNATE = 80,  // No idea what this means
        SLOW = 170,
    } Speed;

    // direction of motor rotation as seen from above (motorHeading) or ?? (motorPitch)
    typedef enum {
        CLOCKWISE = 0,
        COUNTERCLOCKWISE = 1,
    } Direction;

    Merlin();
    void aimAt(float targetHeading, float targetPitch);  // TODO - do we need this?

    // public motor control methods
    void startHorizontalCircle(Direction dir);
    bool checkHorizontalCircleFull();
//    void startVerticalCircle(Direction dir);
//    bool checkVerticalCircleFull();
    void moveMotorPitch(double degrees, Direction dir);

    void stopMotor(std::string motor);
    void waitForStop(const std::string &motor);

    Gyro gyro;

private:
    // For the circle
    double startHeading;
    Direction horizCircleDir;
    double startPitch;
    Direction vertCircleDir;

    // Maximum allowed error for the circle, in degrees
    // Smaller values mean more precision but require
    const float maxErrorHeading = 2.f;
    const float maxErrorPitch = 2.f;

    const std::string motorHeading = "1";
    const std::string motorPitch = "2";
    const int right_up_direction = 0;
    const int left_down_direction = 1;
    const float stepsHeading = 1.f; // TODO
    const float stepsPitch = 1.f; // TODO
    // Delay between characters when sending commands, in nanoseconds
    const unsigned int delay = 25000;
    string commands;
    string recvBuffer;

    // private motor control methods
    void init();
    void startMotor(std::string motor);
    void moveMotor(std::string motor, int direction, int speed);
    void moveHeadingTo(float degrees);  // TODO - do we need this?

    // UART communication methods
    void addCommand(std::string command, bool lineEnd=true);
    std::string positionToString(int pos);
    int openUART();
    void communicate();
    void printBuffer(std::string buffer);
};


#endif //PROJECT_WS1718_MERLIN_H
