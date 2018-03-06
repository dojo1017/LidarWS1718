#include "lidarController.h"
#include "lidarLite.h"

#include <unistd.h>

lidarController::lidarController() {
	this->fd = lidar_init(false); //debug off
	if (fd < 0) fprintf(stderr, "%s", "Could not initialize I2C-filedescriptor");
}

lidarController::~lidarController() {
	close(this->fd);
}

unsigned int lidarController::getDistance() {
	return lidar_read(this->fd);
}

unsigned char lidarController::getStatus() {
	return lidar_status(this->fd);
}

unsigned char lidarController::getVersion() {
	return lidar_version(this->fd);
}
