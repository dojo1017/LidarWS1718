#include "../includes/servoController.h"

#include <unistd.h>
#include <stdio.h>

int main() {
	servoController* control = new servoController();

	control->toStartPosition();
	usleep(1000*1000);

	control->moveServo(1, 180);
	// Test moving into restricted area
	while (true) {
		for (int i = 180; i >= 0; i--) {
			control->moveServo(2, i);
			usleep(100*1000);
		}
		for (int i = 0; i <= 180; i++) {
			control->moveServo(2, i);
			usleep(100*1000);
		}
		usleep(1000*1000);
	}

/*	while (true) {
		for (int i = 0; i <= 180; i+=2) {
			control->moveServo(0, i);
			usleep(100*1000);
		}
		printf("wechsel\n");
		usleep(1000*1000);
		for (int i = 179; i >= 0; i-=2) {
			control->moveServo(0, i);
			usleep(100*1000);
		}
		printf("wechsel\n");
		usleep(1000*1000);
	}*/
	return 0;
}
