#include "../includes/servoController.h"
#include "../includes/servo.h"
#include "../includes/PCA9685.h"

#include <vector>
#include <stdio.h>

servoController::servoController() {
	this->pwm = new PCA9685();
	this->pwm->init(I2C_BUS, PWM_I2C_ADR);
	this->pwm->setPWMFreq(PWM_FREQ); //evtl delay einfügen, hmmm

	servo* servo1 = new servo(0, 115, 510, this->pwm, 0);
	servo* servo2 = new servo(1, 120, 515, this->pwm, 1);
	servo* servo3 = new servo(2, 120, 510, this->pwm, 2);

	this->servos.push_back(servo1);
	this->servos.push_back(servo2);
	this->servos.push_back(servo3);
}

servoController::~servoController() {
	delete this->pwm;
	this->servos.clear();
//	delete this->servos;
}

void servoController::moveServo(int id, unsigned int value) {
//	if (value < 0) value = 0; //d'oh... unsigned < 0
	printf("Move Servo %d to %d ", id, value);
	if (value > STEPS) value = STEPS;

	servo* current = this->servos[id];

	unsigned int rotation;
	unsigned int min = current->getMin();
	unsigned int max = current->getMax();

	rotation = (((max - min)/STEPS)*value)+min;

	if (rotation > max) rotation = max;
	if (rotation < min) rotation = min;

	printf("calculated rotation: %d\n", rotation);

	current->moveTo(rotation);
}

unsigned int servoController::getRotation(int id) {
	return this->servos[id]->getCurrentRotation();
}
