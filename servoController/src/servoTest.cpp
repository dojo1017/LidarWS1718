#include "../includes/servoController.h"

#include <unistd.h>
#include <stdio.h>

int main() {
	servoController* control = new servoController();

	while (true) {
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
	}
	return 0;
}
