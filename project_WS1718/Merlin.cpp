//
// Created by simon on 06.03.18.
//

#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <termios.h>
#include <fcntl.h>
#include <math.h>
#include "Merlin.h"
using std::string;

//#include "libs/merlin/MerlinHalfSqhere.h"

Merlin::Merlin() : gyro() {
    // Just a test
    init();
    communicate();

    for(int i = 0; i < 20; ++i) {
        startMotor(motorHeading);
        communicate();
        waitForStop(motorHeading);
    }

    usleep(100000);

    for(int i = 0; i < 20; ++i) {
        startMotor(motorPitch);
        communicate();
        waitForStop(motorPitch);
    }

//    aimAt(0, 0);  // does not work

//    moveHeadingTo(30.f);  // does not work
//    communicate();

    // Debug: print receive buffer
    printBuffer(recvBuffer);
}

void Merlin::init(){
    addCommand("F" + motorHeading);
    addCommand("a" + motorHeading);
    addCommand("D" + motorHeading);

    addCommand("F" + motorPitch);
    addCommand("a" + motorPitch);
    addCommand("D" + motorPitch);
}

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
        const bool headingMoving = waitForStop(motorHeading);
        const bool pitchMoving = waitForStop(motorPitch);

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
                moveMotor(motorHeading, right_up_direction);
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
                moveMotor(motorPitch, right_up_direction);
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

void Merlin::addCommand(string command, bool lineEnd) {
    commands += ":" + command;
    if(lineEnd) {
        commands += "\r";
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

// Sends the queued commands to the Merlin and reads the responses into recvBuffer
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
            bool gotEchoStart = false;
            bool gotEchoEnd = false;
            bool gotResponseStart = false;
            bool endResponseEnd = false;

            usleep(delay);

            const int maxRecvErrors = 5;
            int recvErrors = 0;

            do {
                // Read one char
                unsigned char tempRecvBuffer[1];
                if(read(filestream, tempRecvBuffer, sizeof(char)) != sizeof(char)) {
                    cout << "ERROR during read()" << endl;
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

            usleep(delay);
        }
    }

    close(filestream);

    // Terminate receive buffer
    recvBuffer.push_back('\0');

    // Throw away the commands we sent
    commands.clear();
}

void Merlin::moveMotor(std::string motor, int direction) {
    // From the documentation at http://www.papywizard.org/wiki/DevelopGuide
    const string speedDivider = "220000";
    const string directionStr = direction ? "1" : "0";

    addCommand("L" + motor);
    addCommand("G" + motor + "3" + directionStr);
    addCommand("I" + motor + speedDivider);
    addCommand("J" + motor);
}

bool Merlin::waitForStop(const string &motor)
{
    cout << "enter waitForStop(" << motor << ")" << endl;

    // For some reason we need to stop the motor, otherwise we don't get a response to "f"
    stopMotor(motor);
    addCommand("f" + motor);
    communicate();

    // debug
    printBuffer(recvBuffer);

    return recvBuffer[recvBuffer.size() - 4] != '0';
}

// For debugging
void Merlin::printBuffer(std::string buffer) {
    for(int i = 0; i < buffer.size(); ++i) {
        if(buffer[i] == '\r') {
            cout << "\\r\n";
        } else {
            cout << buffer[i];
        }
    }
    cout << endl;
}
