#include "../includes/PCA9685.h"
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <iostream>

int main (int argc, char* argv[]) {
	int channel = 0;
	if (argc > 1) channel = atoi(argv[1]);
	PCA9685 pwm;
	pwm.init(1,0x40);
	sleep(1);
	printf("PWM Init done.\n");
	pwm.setPWMFreq (50);
	sleep(1);
	printf("PWM freq set to 50 Hz.\n");
	printf("Any value you type now will be set as on-time PWM channel %d\n", channel);

	bool running = true;
	std::string input = "";
	int current = 0;
	while (running) {
		std::getline(std::cin, input);
		current = atoi(input.c_str());
		if (current < 0) return 0;
		printf("Setting PWM(%d) to %d on-time\n", channel, current);
		pwm.setPWM(channel, current);
		usleep(1000);
	}
	return 0;
}
