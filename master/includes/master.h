#ifndef _master_h_
#define _master_h_

#include "../../view/includes/view.h"
#include "../../lidar/includes/lidarController.h"
#include "../../servoController/includes/servoController.h"
#include "../../calculation/includes/calculation.h"

struct point {
    float x, y, z;
};
struct normals {
    int p1, p2, p3;
};
struct servoPosition {
	int s1, s2, s3;
};
class master {
public:
	master(unsigned int rows, unsigned int columns);
	virtual ~master();
	void run();
private:
	void readDistance(int s0, int s1, int s2);

	lidarController* lidar;
	servoController* servos;
	Calculation* calculation;
	View* view;
	unsigned int currentRow, currentColumn;
	unsigned int rows, columns;
};

#endif
