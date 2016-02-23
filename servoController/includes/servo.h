#ifndef _servo_h_
#define _servo_h_

#include "PCA9685.h"

class servo {
public:
	servo(int id, unsigned int min, unsigned int max, PCA9685* pwm, int channel);
	~servo();
	void moveTo(unsigned int move);
	unsigned int getMin() { return this->min; };
	unsigned int getMax() { return this->max; };
	unsigned int getCurrentRotation() { return this->currentRotation; };
private:
	PCA9685* pwm;
	int id, channel;
	unsigned int min, max;
	unsigned int currentRotation;
};

#endif

