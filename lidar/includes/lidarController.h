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
