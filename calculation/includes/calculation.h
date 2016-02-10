#ifndef Calculation_H_
#define Calculation_H_

#include "../../master/includes/structs.h"
#include <vector>
#include <GLES2/gl2.h>
#include <stdlib.h>
#include <stdio.h>


/* BEGIN GL MATH INCLUDES */
#define GLM_FORCE_RADIANS
// #include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <glm/gtx/string_cast.hpp>
/* END GL MATH INCLUDES */

class Calculation {
	/* Array of vectors for the view */
	// The view expects the points to be ordered for triangles.
	// The first three points make one triangle the second three points make another etc.
	std::vector<glm::vec3>* points;

	/* Array of normals for the view */
	// The view expects the normals to be ordered for triangles (exactly the same way as the points).
	std::vector<glm::vec3>* normals;

	// Two dimensional array with all measured points of the roomscanner (servo+lidar)
	glm::vec3** allPoints;

	// Counters
	int indexRow;
	int rows;
	int indexColumn;
	int columns;
	float maxDistance;

	// Checks the points and if invalid returns false.
	bool checkAnyEmptyPoint(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

	// This method receives the three points, that form a triangle and calculates the normals.
	// These normals are then passed to the normal array for the view.
	void addNormal(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);

	// Log file
	FILE* file;

public:
	Calculation(std::vector<glm::vec3>* _points, std::vector<glm::vec3>* _normals, int rows, int columns);
	~Calculation();

	// This method is called by the master and receives the positions of each servo, the measured distance of the Lidar Lite module, 
	// as well as the current row and column from the measurement unit.
	void addPoint(servoPosition servos, unsigned int distance, unsigned int currentRow, unsigned int currentColumn);

	// Once all the points have been calculated, the master calls this method to duplicate and reorder all points for the view.
	void addPoints();
};

#endif /* Calculation_H_ */
