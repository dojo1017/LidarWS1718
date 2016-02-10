#include <iostream>
#include <stdio.h>

#include "../includes/calculation.h"
void test1(Calculation* calculation, std::vector<glm::vec3>* points, std::vector<glm::vec3>* faces);

void one();

int main(int argc, char const *argv[]) {

	std::vector<glm::vec3>* points = new std::vector<glm::vec3>();
	std::vector<glm::vec3>* faces = new std::vector<glm::vec3>();

	Calculation* calculation = new Calculation(points, faces);

	test1(calculation, points, faces);
	


	printf("\n");
	printf("\n");

	return 0;
}

void test1(Calculation* calculation, std::vector<glm::vec3>* points, std::vector<glm::vec3>* faces) {
	
	position pos0; pos0.x = 0; pos0.y = 0; pos0.z = 90; // in degrees 0
	position pos1; pos1.x = 0; pos1.y = 0; pos1.z = 45; // in degrees 45
	position pos2; pos2.x = 0; pos2.y = 0; pos2.z = 0; // in degrees 90
	position pos3; pos3.x = 0; pos3.y = 0; pos3.z = 315; // in degrees 135
	position pos4; pos4.x = 0; pos4.y = 0; pos4.z = 270; // in degrees 180

	position pos01; pos01.x = 90; pos01.y = 0; pos01.z = 0; // in degrees
	position pos11; pos11.x = 45; pos11.y = 0; pos11.z = 0; // in degrees
	position pos21; pos21.x = 0; pos21.y = 0; pos21.z = 0; // in degrees
	position pos31; pos31.x = 315; pos31.y = 0; pos31.z = 0; // in degrees
	position pos41; pos41.x = 270; pos41.y = 0; pos41.z = 0; // in degrees

	position pos02; pos02.x = 0; pos02.y = 90; pos02.z = 0; // in degrees
	position pos12; pos12.x = 0; pos12.y = 45; pos12.z = 0; // in degrees
	position pos22; pos22.x = 0; pos22.y = 0; pos22.z = 0; // in degrees
	position pos32; pos32.x = 0; pos32.y = 315; pos32.z = 0; // in degrees
	position pos42; pos42.x = 0; pos42.y = 270; pos42.z = 0; // in degrees


	unsigned int distance = 10;



	calculation->addPoint(pos0, distance);
	calculation->addPoint(pos1, distance);
	calculation->addPoint(pos2, distance);
	calculation->addPoint(pos3, distance);
	calculation->addPoint(pos4, distance);

	calculation->addPoint(pos01, distance);
	calculation->addPoint(pos11, distance);
	calculation->addPoint(pos21, distance);
	calculation->addPoint(pos31, distance);
	calculation->addPoint(pos41, distance);

	calculation->addPoint(pos02, distance);
	calculation->addPoint(pos12, distance);
	calculation->addPoint(pos22, distance);
	calculation->addPoint(pos32, distance);
	calculation->addPoint(pos42, distance);

	printf("\n");
	printf("\n");

	printf("Point Array: (Size: %i)\n", points->size());
	for (unsigned int i = 0; i < points->size(); ++i)
	{
		printf("Point %i: %.2f, %.2f, %.2f \n", i, points->at(i).x, points->at(i).y, points->at(i).z);
		if (((i+1) % 5) == 0)
		{
			printf("\n");
		}
	}
	printf("\n");

	printf("Face Array: (Size: %i)\n", faces->size());
	for (unsigned int i = 0; i < faces->size(); ++i)
	{
		printf("Face %i: %.2f, %.2f, %.2f \n", i, faces->at(i).x, faces->at(i).y, faces->at(i).z);
	}
}


void one() {
	glm::mat4 trans;
	trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 result = trans * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	printf("%f, %f, %f\n", result.x, result.y, result.z);
}

