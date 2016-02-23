#include "../includes/master.h"

#include "../../lidar/includes/lidarLite.h" //Unbedingt wieder rausnehmen, nur für Test

#include <unistd.h>
#include <stdio.h>

// Delay between servo movements in usec
#define DELAY (25*1000)

master::master(unsigned int rows, unsigned int columns) {
	this->lidar = new lidarController();
	this->servos = new servoController();
//	this->calculation = new Calculation(0, 0);
//	this->view = new View(0, 0);

	this->rows = rows;
	this->columns = columns;

	servos->toStartPosition();
	usleep(2000*1000);

}

master::~master() {

}

void master::run() {
	unsigned int servosteprow = servos->getSteps()/this->rows;
	unsigned int servostepcolumn = servos->getSteps()/this->columns;

	//scan row by row
	for (currentRow = 0; currentRow < this->rows; currentRow++) {
		if ((currentRow % 2) == 0) {
			for (currentColumn = 0; currentColumn < this->columns; currentColumn++) {
				servos->moveServo(0, currentColumn*servostepcolumn);
				usleep(DELAY);
				readDistance();
			}
		} else {
			for (currentColumn = this->columns ; currentColumn > 0; currentColumn--) {
				#ifdef DEBUG
					//fprintf(stderr, "currentColumn: %d columns: %d servostepc: %d\n", currentColumn, columns, servostepcolumn);
				#endif
				servos->moveServo(0, currentColumn*servostepcolumn);
				usleep(DELAY);
				readDistance();
			}
		}
		servos->moveServo(1, currentRow*servosteprow);
		usleep(1000*1000);
	} //for rows

	//all done, back to rest
	servos->toRestPosition();

}

void master::readDistance() {
	unsigned int dis = this->lidar->getDistance();
	printf("Distanz: %d\n", dis);
	unsigned int status = this->lidar->getStatus();
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
}

int main(int argc, char* argv[]) {
	master* m = new master(90, 90);
	m->run();
	delete m;
}
