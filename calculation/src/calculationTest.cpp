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

	position pos1; pos1.x = 0; pos1.y = 0; pos1.z = 0;
	position pos2; pos2.x = 0; pos2.y = 1; pos2.z = 0;
	position pos3; pos3.x = 1; pos3.y = 0; pos3.z = 0;
	position pos4; pos4.x = 1; pos4.y = 1; pos4.z = 0;

	unsigned int distance = 100;



	calculation->addPoint(pos1, distance);
	calculation->addPoint(pos2, distance);
	calculation->addPoint(pos3, distance);
	calculation->addPoint(pos4, distance);


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
