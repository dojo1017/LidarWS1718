#ifndef _master_h_
#define _master_h_

#include "../../lidar/includes/lidarController.h"
#include "../../servoController/includes/servoController.h"
//#include "../../calculation/includes/calculation.h"
//#include "../../view/includes/view.h"

struct point {
    float x, y, z;
};
struct face {
    int p1, p2, p3;
};
class master {
public:
	master(unsigned int rows, unsigned int columns);
	virtual ~master();
	void run();
private:
	lidarController* lidar;
	servoController* servos;
//	Calculation* calculation;
//	View* view;
	unsigned int currentRow = 0, currentColumn = 0;
	unsigned int rows, columns;
};

#endif
