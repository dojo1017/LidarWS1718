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

// Servos von unten (1), mitte (2), oben (3)

	float servo1 = pos.x;
	float servo2 = pos.y; // radians
	float servo3 = pos.z;

	printf("Servo 1: %f\n", servo1);
	printf("Servo 2: %f\n", servo2);
	printf("Servo 3: %f\n", servo3);

	// glm::vec3 pos3(pos.x, pos.y, pos.z);
	glm::vec3 pos3(1.0f, 1.0f, 1.0f);

	glm::vec4 hVector = glm::vec4(pos3, 1.0f); // create a homogen vector from the compass position


	// result = rot1(y) * trans1(y) * rot2(z) * trans2(y) * rot3(z) * trans3(y) * trans4(y->distance)
	glm::mat4 idMat = glm::mat4(1.0f);
	glm::mat4 rotMat1 = glm::rotate(idMat, glm::radians(servo1), glm::vec3(0.0f, -1.0f, 0.0f)); // Rotation y axis
	glm::mat4 transMat1	= glm::translate(rotMat1, glm::vec3(0.0f, 8.5f, 0.0f)); // Translation y axis
	glm::mat4 rotMat2 = glm::rotate(transMat1, glm::radians(servo2), glm::vec3(0.0f, 0.0f, -1.0f)); // Rotation z axis
	glm::mat4 transMat2 = glm::translate(rotMat2, glm::vec3(0.0f, 7.5f, 0.0f)); // Translation y axis
	glm::mat4 rotMat3 = glm::rotate(transMat2, glm::radians(servo3), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 transMat3 = glm::translate(rotMat3, glm::vec3(0.0f, 3.5f, 0.0f));
	glm::mat4 transMat4 = glm::translate(transMat3, glm::vec3(0.0f, distance, 0.0f));

	glm::vec4 hResult = transMat4 * hVector;



	// printf("\nrotMat1 = %s\n", glm::to_string(rotMat1).c_str());

	// printf("\ntransMat1 = %s\n", glm::to_string(transMat1).c_str());

	// printf("\nrotMat2 = %s\n", glm::to_string(rotMat2).c_str());

	// printf("\ntransMat2 = %s\n", glm::to_string(transMat2).c_str());

	// printf("\nrotMat3 = %s\n", glm::to_string(rotMat3).c_str());

	// printf("\ntransMat3 = %s\n", glm::to_string(transMat3).c_str());


	printf("\nhResult = %s\n", glm::to_string(hResult).c_str());






	glm::vec3 result(hResult);
	this->points->push_back(result);

}

void Calculation::addFace(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3) {
	// faces.push_back(vec3); // Dreidimensionaler Vektor. x = 1. Punkt, y = 2. Punkt, z = 3. Punkt
}