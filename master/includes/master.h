#ifndef _master_h_
#define _master_h_

#include "../../lidar/includes/lidarController.h"
#include "../../servoController/includes/servoController.h"
#include "../../calculation/includes/calculation.h"
#include "../../view/includes/view.h"

struct point {
    float x, y, z;
};
struct face {
    int p1, p2, p3;
};
class master {
public:
    master();
    virtual ~master();
private:
	lidarController* lidar;
	servoController* servos;
	Calculation* calculation;
	View* view;
};

#endif
