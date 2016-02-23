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

// result = rot1(y) * trans1(y) * rot2(z) * trans2(y) * rot3(z) * trans3(y) * trans4(y->distance)
// Servos von unten (1), mitte (2), oben (3)

	float servo1 = 90.0f;
	float servo2 = 90.0f;
	float servo3 = 90.0f;


	glm::vec3 pos3(pos.x, pos.y, pos.z);

	glm::vec4 hVector = glm::vec4(pos3, 1.0f); // create a homogen vector from the compass position


	glm::mat4 idMat = glm::mat4(1.0f);
	glm::mat4 rotMat1 = glm::rotate(idMat, servo1, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotation y axis
	glm::mat4 transMat1	= glm::translate(rotMat1, glm::vec3(0.0f, 8.5f, 0.0f)); // Translation y axis
	glm::mat4 rotMat2 = glm::rotate(transMat1, servo2, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotation z axis
	glm::mat4 transMat2 = glm::translate(rotMat2, glm::vec3(0.0f, 7.5f, 0.0f)); // Translation y axis
	glm::mat4 rotMat3 = glm::rotate(transMat2, servo3, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 transMat3 = glm::translate(rotMat3, glm::vec3(0.0f, 3.5f, 0.0f));

	glm::vec4 hResult = transMat3 * glm::vec4(0.0f, (float)distance, 0.0f, 1.0f);

	printf("\ntransMat3 = %s\n", glm::to_string(transMat3).c_str());


	printf("\nhResult = %s\n", glm::to_string(hResult).c_str());






	glm::vec3 result(hResult);
	this->points->push_back(result);

}

void Calculation::addFace(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3) {
	// faces.push_back(vec3); // Dreidimensionaler Vektor. x = 1. Punkt, y = 2. Punkt, z = 3. Punkt
}