#ifndef Calculation_H_
#define Calculation_H_

#include "../../master/includes/master.h"
#include "../../compass/includes/compassController.h"
#include <vector>
#include <GLES2/gl2.h>


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
	std::vector<glm::vec3>* points;
	std::vector<glm::vec3>* faces;
public:
	Calculation(std::vector<glm::vec3>* _points, std::vector<glm::vec3>* _faces);
	~Calculation();

	void addPoint(position pos, unsigned int distance);
	void addFace(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
};

#endif /* Calculation_H_ */