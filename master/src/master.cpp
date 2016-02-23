#include "../includes/master.h"

#include <unistd.h>

master::master(unsigned int rows, unsigned int columns) {
	this->lidar = new lidarController();
	this->servos = new servoController();
//	this->calculation = new Calculation(0, 0);
//	this->view = new View(0, 0);

	servos->toStartPosition();

	this->rows = rows;
	this->columns = columns;

	usleep(2000*1000);

	servos->toRestPosition();
}

master::~master() {

}

void master::run() {
	//do stuff
}


int main(int argc, char* argv[]) {
	master* m = new master(5, 5);
	m->run();
	delete m;
}
