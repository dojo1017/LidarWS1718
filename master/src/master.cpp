#include "../includes/master.h"

#include <unistd.h>
#include <stdio.h>

master::master(unsigned int rows, unsigned int columns, int delay) {
	this->lidar = new lidarController();
	this->servos = new servoController();
	this->calc = new Calculation(&points, &normals, rows, columns);
	this->rows = rows;
	this->columns = columns;
	this->delay = delay * 1000;

#ifdef OUTPUT
	this->outputfd = fopen("measure.log", "w");
	fprintf(this->outputfd, "servo1 servo2 servo3 distance row column\n");
#endif

	servos->toStartPosition();
	usleep(2000 * 1000); //Wait for debounce on start position

}

master::~master() {
	delete lidar;
	delete servos;
	delete calc;
	delete view;
}

void master::run() {
	float servosteprow = (float)servos->getSteps() / (float)this->rows;
	float servostepcolumn = (float)servos->getSteps() / (float)this->columns;
	unsigned int spos0, spos1, spos2 = 90;

	//scan row by row
	for (currentRow = 0; (unsigned int)currentRow < this->rows; currentRow++) {
		spos1 = (unsigned int)((float)currentRow * servosteprow);

		servos->moveServo(1, spos1);
		usleep(1000 * 1000); //next row, wait for debounce
		//printf("r.moved servo 1 to %d (calculated by row %d * step %f\n", spos1, currentRow, servosteprow);

		if ((currentRow % 2) == 0) { //Even row, scan from left to right
			for (currentColumn = 0; (unsigned int)currentColumn < this->columns; currentColumn++) {
				spos0 = (unsigned int)((float)currentColumn * servostepcolumn);
				servos->moveServo(0, spos0);
				usleep(this->delay);
				readDistance(spos0, spos1, spos2, currentRow, currentColumn);
				//printf("e.moved servo 0 to %d (calculated by row %d * step %f\n", spos0, currentColumn, servostepcolumn);
			}
		} else { //odd row, scan from right to left
			for (currentColumn--; currentColumn >= 0; currentColumn--) {
				spos0 = (unsigned int)((float)currentColumn * servostepcolumn);
				servos->moveServo(0, spos0);
				usleep(this->delay);
				readDistance(spos0, spos1, spos2, currentRow, currentColumn);
				//printf("o.moved servo 0 to %d (calculated by row %d * step %f\n", spos0, currentColumn, servostepcolumn);
			}
			currentColumn++;
		}
	} //for rows

	//all done, back to rest
	servos->toRestPosition();
#ifdef OUTPUT
	fclose(this->outputfd);
#endif
	this->calc->addPoints();
	this->view = new View(points, normals);
	this->view->startScreen();

}

void master::readDistance(int s1, int s2, int s3, int row, int column) {
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
	this->calc->addPoint(pos, dis, row, column);
#ifdef OUTPUT
	fprintf(this->outputfd, "%d %d %d %d %d %d\n", pos.s1, pos.s2, pos.s3, dis, row, column);
#endif
//	fprintf(stderr, "%d %d %d %d\n", pos.s1, pos.s2, pos.s3, dis);
}

int main(int argc, char* argv[]) {
	int row = 40, column = 80, delayMil = 50;
	if (argc == 2) {
		printf("2 oder 3 Paramter möglich, wobei der erste die Zeilenanzahl, der zweite die Spaltenanzahl und der dritte optionale das Delay ist\n");
		exit(0);
	}
	if (argc == 3) {
		row = atoi(argv[1]);
		column = atoi(argv[2]);
	}
	if (argc == 4) {
		row = atoi(argv[1]);
		column = atoi(argv[2]);
		delayMil = atoi(argv[3]);
	}
	if (row > 180 || row < 0 || column > 180 || column < 0 || delayMil < 0) {
		printf("Zeile bzw. Spalte dürfen nur zwischen 0 bis 180 liegen.\n");
	} else  {
		master* m = new master(row, column, delayMil);
		m->run();
		delete m;
	}
}
