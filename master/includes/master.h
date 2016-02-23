#ifndef _master_h_
#define _master_h_

#include "../../view/includes/view.h"
#include "../../lidar/includes/lidarController.h"
#include "../../servoController/includes/servoController.h"
#include "../../calculation/includes/calculation.h"

#include <vector>
#include <glm/glm.hpp>

class master {
public:
	master(unsigned int rows, unsigned int columns);
	virtual ~master();
	void run();
private:
	void readDistance(int s0, int s1, int s2);
	std::vector<glm::vec3> points, normals;
	lidarController* lidar;
	servoController* servos;
	Calculation* calc;
	View* view;
	unsigned int currentRow, currentColumn;
	unsigned int rows, columns;
};

#endif
