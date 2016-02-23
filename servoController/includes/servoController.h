#ifndef _servocontroller_h_
#define _servocontroller_h_

#include "servo.h"
#include "PCA9685.h"

#include <vector>

#define I2C_BUS 1

// PWM-Platine hört auf I2C Adresse 0x40
#define PWM_I2C_ADR 0x40
#define PWM_FREQ 50 /* 50 Hz */

#define STEPS 180

class servoController {
public:
	servoController();
	virtual ~servoController();
	void moveServo(int id, unsigned int value);
	unsigned int getRotation(int id);
	void toStartPosition();
	void toRestPosition();
	unsigned int getSteps() { return (STEPS); };
private:
	std::vector<servo*> servos;
	PCA9685* pwm;
	bool inRestrictedArea(int servoId, unsigned int value);
};

#endif
