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
	// point* points;
	std::vector<glm::vec3>* points; // Reihenfolge bestimmt Flaechen 1.2.3
	glm::vec3** allPoints;
	int indexRow;
	int rows;
	int indexColumn;
	int columns;

	std::vector<glm::vec3>* normals; // Normalen

public:
	Calculation(std::vector<glm::vec3>* _points, std::vector<glm::vec3>* _normals, int rows, int columns);
	~Calculation();

	void addPoint(servoPosition servos, unsigned int distance);
	void addPoint2(glm::vec3 point);
	void addPoints();
	void addNormal(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);

private:
	FILE* file;
};

#endif /* Calculation_H_ */
