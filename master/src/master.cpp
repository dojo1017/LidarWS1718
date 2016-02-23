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
	unsigned int spos0, spos1, spos2 = 90;

	//scan row by row
	for (currentRow = 0; currentRow < this->rows; currentRow++) {
		spos1 = currentRow * servosteprow;
		if ((currentRow % 2) == 0) {
			for (currentColumn = 0; currentColumn < this->columns; currentColumn++) {
				spos0 = currentColumn * servostepcolumn;
				servos->moveServo(0, spos0);
				usleep(DELAY);
				readDistance(spos0, spos1, spos2);
			}
		} else {
			for (currentColumn = this->columns ; currentColumn > 0; currentColumn--) {
				spos0 = currentColumn * servostepcolumn;
				servos->moveServo(0, spos0);
				usleep(DELAY);
				readDistance(spos0, spos1, spos2);
			}
		}
		servos->moveServo(1, spos1);
		usleep(1000*1000);
	} //for rows

	//all done, back to rest
	servos->toRestPosition();

}

void master::readDistance(int s1, int s2, int s3) {
	unsigned int dis = this->lidar->getDistance();
/*	unsigned int status = this->lidar->getStatus();
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
                }*/
	struct servoPosition pos = {s1, s2, s3};
	//call addPoint(pos, dis);
	printf("S0: %d S1: %d S2: %d Distanz: %d\n", pos.s1, pos.s2, pos.s3, dis);
}

int main(int argc, char* argv[]) {
	master* m = new master(45, 45);
	m->run();
	delete m;
}
