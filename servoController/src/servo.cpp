#include "../includes/servo.h"

servo::servo(int id, unsigned int min, unsigned int max, PCA9685* pwm, int channel) {
	this->id = id;
	this->channel = channel;
	this->min = min;
	this->max = max;
	this-> pwm = pwm;
}

servo::~servo() {

}

void servo::moveTo(unsigned int move) {
	if (move < this->min) move = this->min;
	if (move > this->max) move = this->max;

	this->currentRotation = move;
	this->pwm->setPWM(this->channel, move);
}
