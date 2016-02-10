#include <iostream>
#include <stdio.h>

#include "../includes/calculation.h"

void one() {
	glm::mat4 trans;
	trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 result = trans * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	printf("%f, %f, %f\n", result.x, result.y, result.z);
}

int main(int argc, char const *argv[]) {

	std::vector<glm::vec3>* points = new std::vector<glm::vec3>();
	std::vector<glm::vec3>* faces = new std::vector<glm::vec3>();

	Calculation* calculation = new Calculation(points, faces);

	position pos0; pos0.x = 0; pos0.y = 0; pos0.z = 0; // in degrees
	position pos1; pos1.x = 0; pos1.y = 0; pos1.z = 45; // in degrees
	position pos2; pos2.x = 0; pos2.y = 0; pos2.z = 90; // in degrees
	position pos3; pos3.x = 0; pos3.y = 0; pos3.z = 135; // in degrees
	position pos4; pos4.x = 0; pos4.y = 0; pos4.z = 180; // in degrees

	position pos01; pos01.x = 0; pos01.y = 0; pos01.z = 0; // in degrees
	position pos11; pos11.x = 45; pos11.y = 0; pos11.z = 0; // in degrees
	position pos21; pos21.x = 90; pos21.y = 0; pos21.z = 0; // in degrees
	position pos31; pos31.x = 135; pos31.y = 0; pos31.z = 0; // in degrees
	position pos41; pos41.x = 180; pos41.y = 0; pos41.z = 0; // in degrees

	unsigned int distance = 10;



	// calculation->addPoint(pos0, distance);
	// calculation->addPoint(pos1, distance);
	// calculation->addPoint(pos2, distance);
	// calculation->addPoint(pos3, distance);
	// calculation->addPoint(pos4, distance);

	calculation->addPoint(pos01, distance);
	calculation->addPoint(pos11, distance);
	calculation->addPoint(pos21, distance);
	calculation->addPoint(pos31, distance);
	calculation->addPoint(pos41, distance);


	printf("\n");
	printf("\n");

	printf("Point Array: (Size: %i)\n", points->size());
	for (unsigned int i = 0; i < points->size(); ++i)
	{
		printf("Point %i: %f, %f, %f \n", i, points->at(i).x, points->at(i).y, points->at(i).z);
	}
	printf("\n");

	printf("Face Array: (Size: %i)\n", faces->size());
	for (unsigned int i = 0; i < faces->size(); ++i)
	{
		printf("Face %i: %f, %f, %f \n", i, faces->at(i).x, faces->at(i).y, faces->at(i).z);
	}


	printf("\n");
	printf("\n");













	return 0;
}
