#include "../includes/master.h"

master::master() {
	this->lidar = new lidarController();
	this->servos = new servoController();
	this->calculation = new Calculation(0, 0);
	this->view = new View(0, 0);
}

master::~master() {

}


int main(int argc, char* argv[]) {
	master* master = new master();
//	master->run();
	delete master;
}
