#include "lidarController.h"
#include "lidarLite.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	lidarController *lidar = new lidarController();
	unsigned int result, delay;
	unsigned char status;
	if (argc > 1) delay = atoi(argv[1]);
	else delay = 1000;
	for (int i = 0; i < 400; i++) {
		result =  lidar->getDistance();
		status = lidar->getStatus();

		printf("%d cm\n", result);
		if (status != 0) {
			printf("Status Byte: 0x%x ", status);
			if (status & STAT_BUSY) printf("busy\n");
			if (status & STAT_REF_OVER) printf("reference overflow\n");
			if (status & STAT_SIG_OVER) printf("signal overflow\n");
			if (status & STAT_PIN) printf("mode select pin\n");
			if (status & STAT_SECOND_PEAK) printf("second peak\n");
			if (status & STAT_TIME) printf("active between pairs\n");
			if (status & STAT_INVALID) printf("no signal\n");
			if (status & STAT_EYE) printf("eye safety\n");
		}
		usleep(1000*delay);
	}
}
