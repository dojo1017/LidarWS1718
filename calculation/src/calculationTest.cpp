#include <iostream>
#include <stdio.h>

#include "../includes/calculation.h"
void test1(Calculation* calculation, std::vector<glm::vec3>* points, std::vector<glm::vec3>* normals, int rows, int columns);
void test2(Calculation* calculation, std::vector<glm::vec3>* points, std::vector<glm::vec3>* normals);
void test3(Calculation* calculation, std::vector<glm::vec3>* points, std::vector<glm::vec3>* normals, int rows, int columns);
void printAll(std::vector<glm::vec3>* points, std::vector<glm::vec3>* normals);

int main(int argc, char const *argv[]) {

	std::vector<glm::vec3>* points = new std::vector<glm::vec3>();
	std::vector<glm::vec3>* normals = new std::vector<glm::vec3>();
	int rows = 1;
	int columns = 180;

	Calculation* calculation = new Calculation(points, normals, rows, columns);

	test3(calculation, points, normals, rows, columns);
	calculation->addPoints();
	printAll(points, normals);


	// float a = 0;
	// float b = 90;
	// float c = 180;

	// printf("%s\n", glm::to_string(glm::radians(a-90)).c_str());
	// printf("%s\n", glm::to_string(glm::radians(b-90)).c_str());
	// printf("%s\n", glm::to_string(glm::radians(c-90)).c_str());

	printf("\n");
	printf("\n");

	return 0;
}

void test3(Calculation* calculation, std::vector<glm::vec3>* points, std::vector<glm::vec3>* normals, int rows, int columns) {
		int degrees = 270;
	// for (int row = 0; row < rows; ++row)
	// {
		for (int column = 0; column < columns; ++column)
		{
			servoPosition pos; pos.s1 = column; pos.s2 = degrees; pos.s3 = 0; // in degrees 0
			unsigned int distance = 10;
			calculation->addPoint(pos, distance);
		}
	// }
}


void test2(Calculation* calculation, std::vector<glm::vec3>* points, std::vector<glm::vec3>* normals) {

	servoPosition pos0; pos0.s1 = 0; pos0.s2 = 0; pos0.s3 = 90; // in degrees 0
	servoPosition pos1; pos1.s1 = 0; pos1.s2 = 0; pos1.s3 = 45; // in degrees 45
	servoPosition pos2; pos2.s1 = 0; pos2.s2 = 0; pos2.s3 = 0; // in degrees 90
	servoPosition pos3; pos3.s1 = 0; pos3.s2 = 0; pos3.s3 = 315; // in degrees 135
	servoPosition pos4; pos4.s1 = 0; pos4.s2 = 0; pos4.s3 = 270; // in degrees 180

	servoPosition pos01; pos01.s1 = 90; pos01.s2 = 0; pos01.s3 = 0; // in degrees
	servoPosition pos11; pos11.s1 = 45; pos11.s2 = 0; pos11.s3 = 0; // in degrees
	servoPosition pos21; pos21.s1 = 0; pos21.s2 = 0; pos21.s3 = 0; // in degrees
	servoPosition pos31; pos31.s1 = 315; pos31.s2 = 0; pos31.s3 = 0; // in degrees
	servoPosition pos41; pos41.s1 = 270; pos41.s2 = 0; pos41.s3 = 0; // in degrees

	servoPosition pos02; pos02.s1 = 0; pos02.s2 = 90; pos02.s3 = 0; // in degrees
	servoPosition pos12; pos12.s1 = 0; pos12.s2 = 45; pos12.s3 = 0; // in degrees
	servoPosition pos22; pos22.s1 = 0; pos22.s2 = 0; pos22.s3 = 0; // in degrees
	servoPosition pos32; pos32.s1 = 0; pos32.s2 = 315; pos32.s3 = 0; // in degrees
	servoPosition pos42; pos42.s1 = 0; pos42.s2 = 270; pos42.s3 = 0; // in degrees


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
}

void test1(Calculation* calculation, std::vector<glm::vec3>* points, std::vector<glm::vec3>* normals, int rows, int columns) {
	int i = 1;
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			glm::vec3 point(i, 0.0f, 0.0f);
			calculation->addPoint2(point);
			i++;
		}
	}
}

void printAll(std::vector<glm::vec3>* points, std::vector<glm::vec3>* normals) {
	printf("Point Array: (Size: %i)\n", points->size());
	for (unsigned int i = 0; i < points->size(); ++i)
	{
		printf("Point %i: %.2f, %.2f, %.2f \n", i, points->at(i).x, points->at(i).y, points->at(i).z);
	}
	printf("\n");

	printf("Face Array: (Size: %i)\n", normals->size());
	for (unsigned int i = 0; i < normals->size(); ++i)
	{
		printf("Face %i: %.2f, %.2f, %.2f \n", i, normals->at(i).x, normals->at(i).y, normals->at(i).z);
	}
}

