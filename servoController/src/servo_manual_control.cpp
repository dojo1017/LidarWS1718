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
	printf("1\n");
	pwm.setPWMFreq (50);
	sleep(1);
	printf("2\n");

	bool running = true;
	std::string input = "";
	int current = 0;
	while (running) {
		std::getline(std::cin, input);
		const char *pt = input.c_str();
		current = atoi(pt);
		if (current < 0) return 0;
		printf("Setting PWM to %d on-time\n", current);
		pwm.setPWM(channel, current);
		usleep(1000);
	}
	return 0;
}
