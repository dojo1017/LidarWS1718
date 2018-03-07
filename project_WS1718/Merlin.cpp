//
// Created by simon on 06.03.18.
//

#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <termios.h>
#include <fcntl.h>

#include "Merlin.h"
using std::string;

Merlin::Merlin() {
    // Just a test
    init();
    moveHeadingTo(30.f);
    cout << "start communicate()" << endl;
    communicate();
    cout << "communicate() done" << endl;
}

void Merlin::init(){
    string command;
    //Motor 1
    command = "F" + motorHeading;
    addCommand(command);
    command = "a" + motorHeading;
    addCommand(command);
    command = "D" + motorHeading;
    addCommand(command);

    //Motor 2
    command = "F" + motorPitch;
    addCommand(command);
    command = "a" + motorPitch;
    addCommand(command);
    command = "D" + motorPitch;
    addCommand(command);
}

// This is a blocking method (blocks until the new position is reached)
void Merlin::aimAt(float heading, float pitch) {
//    printf("Merlin: aiming at heading %.2f pitch %.2f\n", heading, pitch);
    double currHeading = gyro.getHeading();
    double currPitch = gyro.getPitch();
    printf("Merlin: current heading %.2f pitch %.2f\n", currHeading, currPitch);
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

void Merlin::communicate() {
    int filestream = openUART();
    if(filestream == -1) {
        return;
    }

    string recvBuffer;
//    unsigned char recvBuffer[1024];
//    unsigned char *recvBufferPtr = recvBuffer;
    const char *sendBufferPtr = commands.c_str();

    for(int i = 0; i < commands.size(); ++i) {
        const char charToSend = sendBufferPtr[0];
        // Debug output
        string debugOutput(1, charToSend);
        if(charToSend == '\r') {
            debugOutput = "\\r";
        }
        cout << "Send: " << debugOutput << endl;

        if(write(filestream, sendBufferPtr++, sizeof(char)) != sizeof(char)) {
            cout << "ERROR during write()" << endl;
        }
        usleep(delay);

        // TODO: extra delay after '\r'?
        if(charToSend == '\r') {
            usleep(delay);

            do {
                // Read one char
                unsigned char tempRecvBuffer[1];
                if(read(filestream, tempRecvBuffer, sizeof(char)) != sizeof(char)) {
                    cout << "ERROR during read()" << endl;
                    // Do not add this char to the receive buffer, it is random
                    // TODO: if the '\r' fails, we have an endless loop...
                    continue;
                }

                recvBuffer.push_back(tempRecvBuffer[0]);

                // Debug output
                const char recvChar = recvBuffer.back();
                string debugOutput(1, recvChar);
                if(recvChar == '\r') {
                    debugOutput = "\\r";
                }
                cout << "> Recv: " << debugOutput << endl;
                // TODO: sleep after each char?
            } while(recvBuffer.back() != '\r');
        }
    }

    close(filestream);
}