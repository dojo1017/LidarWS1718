#include "../includes/servoController.h"
#include "../includes/servo.h"
#include "../includes/PCA9685.h"

#include <vector>
#include <stdio.h>

servoController::servoController() {
	this->pwm = new PCA9685();
	this->pwm->init(I2C_BUS, PWM_I2C_ADR);
	this->pwm->setPWMFreq(PWM_FREQ); //evtl delay einfügen, hmmm //nö, geht ohne

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
	#ifdef DEBUG
		printf("Move Servo %d to %d ", id, value);
	#endif

	if (value > STEPS) value = STEPS;

	servo* current = this->servos[id];

	unsigned int rotation;
	unsigned int min = current->getMin();
	unsigned int max = current->getMax();

	rotation = (((max - min)/STEPS)*value)+min;

	if (rotation > max) rotation = max;
	if (rotation < min) rotation = min;

	#ifdef DEBUG
		printf("calculated rotation: %d\n", rotation);
	#endif

	if (inRestrictedArea(id, rotation)) fprintf(stderr, "Move to restricted area requestet! This incident will be reported!\n");
	else current->moveTo(rotation);
}

void servoController::toStartPosition() {
	for (unsigned int i = 0; i < servos.size(); i++) {
		moveServo(i, (STEPS/2));
	}
}

bool servoController::inRestrictedArea(int servoId, unsigned int value) {
	//MODIFY FOR YOUR SCENARIO!
	if (servoId == 0) return false;

	int rot1 = this->servos[1]->getCurrentRotation(),
	    rot2 = this->servos[2]->getCurrentRotation();

	if (servoId == 2 && rot1 > 330 && value < 300) return true;
	if (servoId == 1 && rot2 < 300 && value > 330) return true;
	if (servoId >= 3) return true;
	return false;
}

unsigned int servoController::getRotation(int id) {
	return this->servos[id]->getCurrentRotation();
}
