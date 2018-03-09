#include <cstdio>
#include <sstream>
#include <iomanip>
#include <termios.h>
#include <fcntl.h>
#include <cmath>

#include "Merlin.h"
#include "Utils.h"

using std::string;

// More or less crappy documentation of the Merlin/Orion protocol:
// http://www.papywizard.org/wiki/DevelopGuide
// https://framagit.org/fma38/Papywizard/blob/master/papywizard/hardware/merlinOrionHardware.py

// How to move a motor:
//
// moveMotor(motorHeading, 0, Speed::FAST);
// usleep(5000000);  // wait 5 seconds, during this time the motor drives
// stopMotor(motorHeading);
// waitForStop(motorHeading);


Merlin::Merlin() : gyro() {
    //gyro.calibrate(CALIBRATION_FILENAME);
    init();
}

void Merlin::init(){
    addCommand("F" + motorHeading);
    addCommand("a" + motorHeading);
    addCommand("D" + motorHeading);

    addCommand("F" + motorPitch);
    addCommand("a" + motorPitch);
    addCommand("D" + motorPitch);
}

// Start to move the lower motor (heading) in a full circle.
// This method is non-blocking.
// You have to check regularly if the circle was completed
// by calling Merlin::checkHorizontalCircleFull()
void Merlin::startHorizontalCircle(Direction dir) {
    startHeading = gyro.getHeading();
    horizCircleDir = dir;
    // TODO: find out if direction 0 or 1 is needed to increase the heading
    // Direction::CLOCKWISE increases the heading
    moveMotor(motorHeading, dir, Speed::FAST);
}
/*
bool Merlin::checkHorizontalCircleFull() {
    const double currentHeading = gyro.getHeading();

    // 220
    //

    double deltaHeading;
    if(horizCircleDir == Direction::CLOCKWISE) {
        deltaHeading = currentHeading - startHeading;
    } else {
        deltaHeading = startHeading - currentHeading;
    }
    cout << "deltaHeading: " << deltaHeading << endl;

    // TODO test counterclockwise direction, not sure if correct
    if(deltaHeading < 0.0 && deltaHeading > -maxErrorHeading) {
        cout << "Horizontal circle done, stopping motor" << endl;
        // We reached our starting point
        stopMotor(motorHeading);
        waitForStop(motorHeading);
        return true;
    }
    return false;
}

// This is a blocking method
void Merlin::moveMotorPitch(double degrees, Direction dir) {
    const double startPitch = gyro.getPitch();
    moveMotor(motorPitch, dir, Speed::NORMAL);

    double currPitch;
    do {
        currPitch = gyro.getPitch();
        const double deltaPitch = utils::pitchAngleDelta(currPitch, startPitch);

        cout << "pitch: " << currPitch << " startPitch: " << startPitch
             << " = deltaPitch: " << deltaPitch << endl;
        // Wait for the motor to move the specified amount
        usleep(5000);
    } while(currPitch < degrees);

    // currPitch > degrees, we are done
    stopMotor(motorPitch);
    waitForStop(motorPitch);
}

*/

bool Merlin::checkHorizontalCircleFull() {
    const float currentHeading = gyro.getHeading();
    float deltaHeading;
    float startMaxError; //maxError relativ zum Startpunkt

    if(checkClockwise)
    {
        if(currentHeading > startHeading)
        {
            if(currentHeading > (startHeading + 180))
            {
                clockwise = false;
            }else{
                clockwise = true;
            }
        }

        if(currentHeading < startHeading)
        {
            if(currentHeading < (startHeading - 180))
            {
                clockwise = true;
            }else{
                clockwise = false;
            }
        }

        checkClockwise = false;
    }

    if(clockwise) {

        startMaxError = startHeading + maxErrorHeading;

        if(startMaxError >= 360) //falls der Nullpunkt zwischen start und startMaxError liegt
        {
            startMaxError = startMaxError - 360;

            if(searchBorder && (currentHeading > startMaxError) )
            {
                borderHeading = currentHeading;
            }

            if((!searchBorder) && ((currentHeading > startHeading) || (currentHeading < borderHeading)))
            {
                cout << "Horizontal circle done, stopping motor" << endl;
                // We reached our starting point
                stopMotor(motorHeading);
                waitForStop(motorHeading);
                searchBorder = true;
                checkClockwise = true;
                return true;
            }

        }else{

            if(searchBorder && (currentHeading > startMaxError) )
            {
                borderHeading = currentHeading;
                searchBorder = false;
            }

            if((!searchBorder) && (currentHeading > startHeading) && (currentHeading <= borderHeading))
            {
                cout << "Horizontal circle done, stopping motor" << endl;
                // We reached our starting point
                stopMotor(motorHeading);
                waitForStop(motorHeading);
                searchBorder = true;
                checkClockwise = true;
                return true;
            }

        }


    }else{ //counterclockwise

        startMaxError = startHeading - maxErrorHeading;

        if(startMaxError < 0) //falls der Nullpunkt  zwischen start und startMaxError liegt
        {
            startMaxError = startMaxError + 360;

            if(searchBorder && (currentHeading < startMaxError) )
            {
                borderHeading = currentHeading;
                searchBorder = false;
            }

            if((!searchBorder) && ((currentHeading < startHeading) || (currentHeading > maxErrorHeading)))
            {
                cout << "Horizontal circle done, stopping motor" << endl;
                // We reached our starting point
                stopMotor(motorHeading);
                waitForStop(motorHeading);
                searchBorder = true;
                checkClockwise = true;
                return true;
            }


        } else{

            if(searchBorder && (currentHeading < startMaxError) )
            {
                borderHeading = currentHeading;
                searchBorder = false;
            }

            if((!searchBorder) && (currentHeading < startHeading) && (currentHeading > maxErrorHeading))
            {
                cout << "Horizontal circle done, stopping motor" << endl;
                // We reached our starting point
                stopMotor(motorHeading);
                waitForStop(motorHeading);
                searchBorder = true;
                checkClockwise = true;
                return true;
            }

        }

    }

    return false;

}

//void Merlin::startVerticalCircle(Direction dir) {
//    startPitch = gyro.getPitch();
//    vertCircleDir = dir;
//    moveMotor(motorPitch, dir, Speed::FAST);
//}
//
//bool Merlin::checkVerticalCircleFull() {
//    const double currentPitch = gyro.getPitch();
//
//    double deltaPitch;
//    if(vertCircleDir == Direction::CLOCKWISE) {
//        deltaPitch = currentPitch - startPitch;
//    } else {
//        deltaPitch = startPitch - currentPitch;
//    }
//    cout << "deltaPitch: " << deltaPitch << endl;
//
//    // TODO test counterclockwise direction, not sure if correct
//    if(deltaPitch < 0.0 && deltaPitch > -maxErrorHeading) {
//        cout << "Vertical circle done, stopping motor" << endl;
//        // We reached our starting point
//        stopMotor(motorPitch);
//        waitForStop(motorPitch);
//        return true;
//    }
//    return false;
//}

// This is a blocking method (blocks until the new position is reached)
void Merlin::aimAt(float targetHeading, float targetPitch) {
//    printf("Merlin: aiming at heading %.2f pitch %.2f\n", heading, pitch);
    float deltaHeading;
    float deltaPitch;
    bool headingTargetReached = false;
    bool pitchTargetReached = false;
    bool targetReached = false;
    double currHeading = gyro.getHeading();
    double currPitch = gyro.getPitch();
    printf("Merlin: current heading %.2f pitch %.2f\n", currHeading, currPitch);

    while(!targetReached){
        // TODO
//        const bool headingMoving = waitForStop(motorHeading);
//        const bool pitchMoving = waitForStop(motorPitch);
        bool headingMoving = false;
        bool pitchMoving = false;

        cout << "heading moving: " << headingMoving;
        cout << " pitch moving: " << pitchMoving << endl;

        if(headingMoving || pitchMoving) {
            cout << "heading or pitch moving" << endl;
        } else {
            deltaHeading = targetHeading - currHeading;
            deltaPitch = targetPitch - currPitch;

            cout << "deltaHeading: " << deltaHeading;
            cout << "deltaPitch: " << deltaPitch << endl;


            //Heading-Motor an Zielposition annaehern
            if(fabsf(deltaHeading) > maxErrorHeading)
            {
                cout << "turn right" << endl;
                //Heading-Motor dreht sich solange nach rechts bis er die Zeilposition erreicht
//                startMoving(motorHeading, right_up_direction);
                moveMotor(motorHeading, right_up_direction, Speed::FAST);
                communicate();

//                if(targetHeading > currHeading) //Ziel befindet sich weiter "rechts" im Uhrzeigersinn
//                {
//                    if( fabsf(deltaHeading) > 180)
//                    {
//                        //kuerzerer Weg bei Bewegung nach links
//                        startMoving(motorHeading,left_down_direction); //nach links bewegen
//                        communicate();
//                    }else
//                    {
//                        //kuerzerer Weg bei Bewegung nach rechts
//                        startMoving(motorHeading,right_up_direction); //nach rechts bewegen
//                        communicate();
//                    }
//
//                }else if (targetHeading < currHeading) //Ziel befindet sich weiter "links" im Uhrzeigersinn
//                {
//                    if( fabsf(deltaHeading) > 180)
//                    {
//                        //kuerzerer Weg bei Bewegung nach rechts
//                        startMoving(motorHeading,right_up_direction); //nach rechts bewegen
//                        communicate();
//                    }else
//                    {
//                        //kuerzerer Weg bei Bewegung nach links
//                        startMoving(motorHeading,left_down_direction); //nach links bewegen
//                        communicate();
//                    }
//                }
            } else {
                cout << "target reached (heading)" << endl;
                headingTargetReached = true;
            }

            //Pitch-Motor an Zielposition annaehern
            if(fabsf(deltaPitch) > maxErrorPitch) {
                cout << "move pitch" << endl;
                //Pitch-Motor dreht sich solange nach "oben" bis er die Zeilposition erreicht
                moveMotor(motorPitch, right_up_direction, Speed::FAST);
                communicate();
            } else {
                pitchTargetReached = true;
            }

            if(headingTargetReached && pitchTargetReached) {
                targetReached = true;
            }
        }
    }
}

void Merlin::startMotor(string motor) {
    addCommand("J" + motor);
}

void Merlin::stopMotor(string motor) {
    addCommand("L" + motor);
}

void Merlin::moveHeadingTo(float degrees) {
    // The magic number 8388608 is from the Chronomotion code
    long pos = degrees * stepsHeading / 360.f + 8388608;
    string posString = positionToString(pos);

    stopMotor(motorHeading);
    addCommand("G" + motorHeading + "40");
    addCommand("S" + motorHeading + posString);
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

void Merlin::waitForStop(const string &motor)
{
    cout << "enter waitForStop(" << motor << ")" << endl;

    int i = 0;  // debug counter
    do {
        cout << "waitForStop loop " << i++ << endl;
        // For some reason we need to stop the motor, otherwise we don't get a response to "f"
//        stopMotor(motor);
        addCommand("f" + motor);
        communicate();

        // debug
//        printBuffer(recvBuffer);

        // The end of recvBuffer contains: "X0X" or "X1X" (X are numbers we don't care for)
    } while(recvBuffer[recvBuffer.size() - 2] != '0');
}

void Merlin::moveMotor(std::string motor, int direction, int speed) {
    stopMotor(motor);
    communicate();

    const string directionStr = direction ? "31" : "30";
    addCommand("G" + motor + directionStr);
    communicate();

    addCommand("I" + motor + positionToString(speed));
    communicate();

    startMotor(motor);
    communicate();
}

int Merlin::openUART() {
    int uart0_filestream = -1;
    uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1) {
        cout << "[ERROR] UART0 open() failed" << endl;
        return -1;
    }

    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
    return uart0_filestream;
}

// Sends the queued commands to the Merlin and reads the responses into recvBuffer.
// After calling this method, the command buffer is empty and the recvBuffer
// only contains the response of the last command (without control characters,
// so e.g. the response "=303\r" is actually "303" in the recvBuffer).
//
// Special characters:
//      : begins a command from us
//      = begins a response by the Merlin head
//      \r signals the end of a command or response
//      ! signals an invalid command syntax (! can be in the response from the head)
void Merlin::communicate() {
    int filestream = openUART();
    if(filestream == -1) {
        return;
    }

    // Throw away old responses
    recvBuffer.clear();

    const char *sendBufferPtr = commands.c_str();

    for(int i = 0; i < commands.size(); ++i) {
        const char charToSend = sendBufferPtr[0];
        if(write(filestream, sendBufferPtr++, sizeof(char)) != sizeof(char)) {
            cout << "ERROR during write()" << endl;
        }
        usleep(delay);

        if(charToSend == '\r') {
            // Trial and error result, not sure if needed
            usleep(delay);

            // We finished a command, read the response.
            // First we read an echo of our own command, then the actual response.
            bool gotEchoStart = false;
            bool gotEchoEnd = false;
            bool gotResponseStart = false;
            bool endResponseEnd = false;

            const int maxRecvErrors = 5;
            int recvErrors = 0;

            do {
                // Read one char
                unsigned char tempRecvBuffer[1];
                ssize_t readBytes;
                if((readBytes = read(filestream, tempRecvBuffer, sizeof(char))) != sizeof(char)) {
                    cout << "ERROR during read(): got " << readBytes << " instead of " << sizeof(char) << endl;
                    recvErrors++;
                    // Do not add this char to the receive buffer, it is random
                    if(recvErrors >= maxRecvErrors) {
                        // Prevent an endless loop if we fail to read the '\r'
                        break;
                    } else {
                        continue;
                    }
                }

                char received = tempRecvBuffer[0];

                if(received == ':') {
                    gotEchoStart = true;
                } else if(gotEchoStart && received == '\r') {
                    gotEchoEnd = true;
                }

                if(gotEchoEnd) {
                    if (received == '=') {
                        gotResponseStart = true;
                    } else if (gotResponseStart && received == '\r') {
                        endResponseEnd = true;
                    } else {
                        // Only add all characters that are not control characters to receive buffer
                        recvBuffer.push_back(received);
                    }
                }

                // Debug output
//                const char recvChar = received;
//                string debugOutput(1, recvChar);
//                if(recvChar == '\r') {
//                    debugOutput = "\\r";
//                }
//                cout << "> Recv: " << debugOutput << endl;
            } while(!endResponseEnd);

            // Trial and error result, not sure if needed
            usleep(delay);
        }
    }

    close(filestream);

    // Throw away the commands we sent
    commands.clear();
}


void Merlin::addCommand(string command, bool lineEnd) {
    commands += ":" + command;
    if(lineEnd) {
        commands += "\r";
    }
}

// For debugging
void Merlin::printBuffer(std::string buffer) {
    for(const char c : buffer) {
        if(c == '\r') {
            cout << "\\r\n";
        } else {
            cout << c;
        }
    }
    cout << endl;
}

// Basically a direct port from MerlinHalfSphere
void Merlin::goToDegree(std::string motor, int degree) {
    const int SINGLE_DEGREE = 0xA00;
    const int PIVOT_ANGLE = 0x800000;

    const int relativeAngle = degree * SINGLE_DEGREE;
    const int targetAngle = PIVOT_ANGLE + relativeAngle;
    const string posString = positionToString(targetAngle);

    addCommand("G" + motor + "00");
    addCommand("S" + motor + posString);
}

// Basically a direct port from MerlinHalfSphere
void Merlin::doSequenceStep(int angle, string motor) {
    // Size of one step, in degrees
    const int ANGLE = 5;
    // Größe eines 1° Winkels - Derzeit empierischer Wert, nicht ganz korrekt.
    const int SINGLE_DEGREE = 0xA00;

    stopMotor(motor);
    goToDegree(motor, angle);
    startMotor(motor);
    communicate();

    waitForStop(motor);
}
