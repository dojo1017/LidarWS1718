//
// Created by simon on 06.03.18.
//

#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <math.h>
#include "Merlin.h"
using std::string;

Merlin::Merlin() {

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

        if((!isHeadingMoving()) && (!isPitchMoving()))
        {
            deltaHeading = targetHeading - currHeading;
            deltaPitch = targetPitch - currPitch;


            //Heading-Motor an Zielposition annaehern
            if(fabsf(deltaHeading) > maxErrorHeading)
            {
                //Heading-Motor dreht sich solange nach rechts bis er die Zeilposition erreicht
                //startMoving(motorHeading, right_up_direction);
                ////TODO Kommandos senden
                //resetCommands();

                if(targetHeading > currHeading) //Ziel befindet sich weiter "rechts" im Uhrzeigersinn
                {
                    if( fabsf(deltaHeading) > 180)
                    {
                        //kuerzerer Weg bei Bewegung nach links
                        startMoving(motorHeading,left_down_direction); //nach links bewegen
                        //TODO Kommandos senden
                        resetCommands();
                    }else
                    {
                        //kuerzerer Weg bei Bewegung nach rechts
                        startMoving(motorHeading,right_up_direction); //nach rechts bewegen
                        //TODO Kommandos senden
                        resetCommands();
                    }

                }else if (targetHeading < currHeading) //Ziel befindet sich weiter "links" im Uhrzeigersinn
                {
                    if( fabsf(deltaHeading) > 180)
                    {
                        //kuerzerer Weg bei Bewegung nach rechts
                        startMoving(motorHeading,right_up_direction); //nach rechts bewegen
                        //TODO Kommandos senden
                        resetCommands();
                    }else
                    {
                        //kuerzerer Weg bei Bewegung nach links
                        startMoving(motorHeading,left_down_direction); //nach links bewegen
                        //TODO Kommandos senden
                        resetCommands();
                    }
                }

            }
            else
            {
                headingTargetReached = true;
            }

            //Pitch-Motor an Zielposition annaehern
            if(fabsf(deltaPitch) > maxErrorPitch)
            {
                //Pitch-Motor dreht sich solange nach oben bis er die Zeilposition erreicht
                startMoving(motorPitch, right_up_direction);
                //TODO Kommandos senden
                resetCommands();
            }
            else
            {
                pitchTargetReached = true;
            }

            if(headingTargetReached && pitchTargetReached)
            {
                targetReached = true;
            }
        }


    }

}

void Merlin::resetCommands(){
    commands = "";
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

void Merlin::startMoving(string motor, string direction)
{
    stopMotor(motor); //":L<motor>\r"
    addCommand("G" + motor + "3" + direction);
    addCommand("I" + motor + "220000");
    startMotor(motor); //":J<motor>\r"
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

bool Merlin::isHeadingMoving()
{
    string receivedMessage;

    resetCommands();
    addCommand("f" + motorHeading);
    //TODO senden
    //TODO empfangen (receivedMessage setzen)

    //if(receivedMessage[1].("0"))

}

bool Merlin::isPitchMoving()
{
    resetCommands();
    addCommand("f" + motorPitch);
    //TODO senden
    //TODO empfangen(receivedMessage setzen)



}
