#include "../includes/master.h"

#include <unistd.h>

// Delay between servo movements in usec
#define DELAY (100*1000)

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
			for (currentColumn = 0; currentColumn < (this->columns/2); currentColumn++) {
				servos->moveServo(0, currentColumn*servostepcolumn);
				usleep(DELAY);
			}
		} else {
			for (currentColumn = (this->columns/2) ; currentColumn >= 0; currentColumn--) {
				servos->moveServo(0, currentColumn*servostepcolumn);
				usleep(DELAY);
			}
		}
		servos->moveServo(1, currentRow*servosteprow);
		usleep(DELAY);
	} //for rows

}


int main(int argc, char* argv[]) {
	master* m = new master(90, 90);
	m->run();
	delete m;
}
