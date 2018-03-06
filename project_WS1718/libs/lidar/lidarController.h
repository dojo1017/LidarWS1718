#ifndef _lidar_controller_h
#define _lidar_controller_h

class lidarController {
public:
	lidarController();
	~lidarController();

	unsigned int getDistance();
	unsigned char getStatus();
	unsigned char getVersion();

private:
	int fd;
};

#endif
