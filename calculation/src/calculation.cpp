#include "../includes/calculation.h"


Calculation::Calculation(std::vector<glm::vec3>* _points, std::vector<glm::vec3>* _faces) {
	this->points = _points;
	this->faces = _faces;

}

Calculation::~Calculation() {

}

void Calculation::addPoint(position pos, unsigned int distance) {

// struct position { int x,y,z; }; // Kompasswerte
// unsigned int distance // Lidar Distanz

	glm::vec3 pos3(pos.x, pos.y, pos.z);

	glm::vec4 hVector = glm::vec4(pos3, 1.0); // create a homogen vector from the compass position

	// printf("addPoint() = %s\n", glm::to_string(homogeneVector).c_str());

	// Einheitsmatrix 4x4
	glm::mat4 idMat4 = glm::mat4(1.0f);
	
	/* Translation */
	// Translationsmatrix auf der y-Achse in +8.5 cm
	glm::mat4 transMat4 = glm::translate(0.0f, 8.5f, 0.0f);
	

	/* Rotation */
	// Rotationsachse ist die y-Achse
	glm::vec3 rotAxis(0,1,0);
	// Rotation glm::rotate( angle_in_degrees , myRotationAxis )
	glm::rotate(90, rotAxis)

	// printf("addPoint() = %s\n", glm::to_string(myMatrix).c_str());
	glm::vec4 transformedVector = myMatrix * hVector;






	glm::vec3 result(hVector);
	this->points->push_back(result);

}

void Calculation::addFace(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3) {
	// faces.push_back(vec3); // Dreidimensionaler Vektor. x = 1. Punkt, y = 2. Punkt, z = 3. Punkt
}