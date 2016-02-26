#include "../includes/calculation.h"

#include <stdio.h>

// Constructor
Calculation::Calculation(std::vector<glm::vec3>* _points, std::vector<glm::vec3>* _normals, int rows, int columns) {
	this->points = _points;
	this->normals = _normals;

	this->indexRow = 0;
	this->indexColumn = 0;

	this->rows = rows;
	this->columns = columns;

	this->maxDistance = 0;

	// Create the array, where all measured data will be stored
	this->allPoints = (glm::vec3**)malloc(rows * sizeof(glm::vec3*));
	for (int i = 0; i < rows; i++)
	{
		allPoints[i] = (glm::vec3*) malloc(columns * sizeof(glm::vec3));
	}

	// Open log file
	this->file = fopen("points.log", "w");
}

// Destructor
Calculation::~Calculation() {
	delete[] this->allPoints;
}


void Calculation::addPoint(servoPosition servos, unsigned int distance, unsigned int currentRow, unsigned int currentColumn) {

	if (distance < 1000 && distance > 0) {

		float servo1 = servos.s1; // Servo von unten (1)
		float servo2 = servos.s2; // Servo von mitte (2)
		float servo3 = servos.s3; // Servo von oben (3)

		glm::vec3 servos3(0.0f, 0.0f, 0.0f);

		glm::vec4 hVector = glm::vec4(servos3, 1.0f); // create a homogen vector from the compass position


		// Result: rot1(y) * trans1(y) * rot2(z) * trans2(y) * rot3(z) * trans3(y) * trans4(y->distance) * base
		glm::mat4 idMat = glm::mat4(1.0f);
		// rot1(y)
		glm::mat4 rotMat1 = glm::rotate(idMat, glm::radians(servo1 - 90), glm::vec3(0.0f, -1.0f, 0.0f)); // Rotation y axis
		// trans1(y)
		glm::mat4 transMat1	= glm::translate(rotMat1, glm::vec3(0.0f, 8.5f, 0.0f)); // Translation y axis
		// rot2(z)
		glm::mat4 rotMat2 = glm::rotate(transMat1, glm::radians(servo2 - 90), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotation z axis
		// trans2(y)
		glm::mat4 transMat2 = glm::translate(rotMat2, glm::vec3(0.0f, 7.5f, 0.0f)); // Translation y axis
		// rot3(z)
		glm::mat4 rotMat3 = glm::rotate(transMat2, glm::radians(servo3 - 90), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotation z axis (2)
		// trans3(y)
		glm::mat4 transMat3 = glm::translate(rotMat3, glm::vec3(0.0f, 3.5f, 0.0f)); // Translation y axis (2)
		// trans4(y->distance)
		glm::mat4 transMat4 = glm::translate(transMat3, glm::vec3(0.0f, distance, 0.0f)); // Translation y axis (distance)
		// base
		glm::vec4 hResult = transMat4 * hVector; // Mulitply with Normal (1, 1, 1, 1)

		// homogene to three dimensional vector
		glm::vec3 result(hResult);
		// add point to array
		this->allPoints[currentRow][currentColumn] = result;

		// find the value of the farthest coordinate, it will later be used to normalize all vectors
		if (this->maxDistance < abs(result.x)) { this->maxDistance = abs(result.x);	}
		if (this->maxDistance < abs(result.y)) { this->maxDistance = abs(result.y); }
		if (this->maxDistance < abs(result.z)) { this->maxDistance = abs(result.z); }
	}
}

void Calculation::addPoints() {

	printf("Rows: %i\n", this->rows);
	printf("Columns: %i\n", this->columns);

	for (int row = 1; row < this->rows - 1; row++)
	{
		for (int column = this->columns - 1; column >= 0; column--)
		{
			if (column > 0)
			{
				glm::vec3 point1(this->allPoints[row][column]);
				glm::vec3 point2(this->allPoints[row - 1][column - 1]);
				glm::vec3 point3(this->allPoints[row - 1][column]);

				// normalize all vectors
				point1 = point1 * (1 / maxDistance);
				point2 = point2 * (1 / maxDistance);
				point3 = point3 * (1 / maxDistance);

				if (checkAnyEmptyPoint(point1, point2, point3)) {
					printf("Missing point at row %d column %d\n", row, column);
				} else {
					// push points in correct order to the points array for the view
					this->points->push_back(point1);
					this->points->push_back(point2);
					this->points->push_back(point3);

					// add the normals for these three points
					addNormal(point1, point2, point3);

					// printf("FOO Entry: (%i,%i): %s\n", row, column, glm::to_string(point1).c_str());
					// printf("FOO Entry: (%i,%i): %s\n", row - 1, column - 1, glm::to_string(point2).c_str());
					// printf("FOO Entry: (%i,%i): %s\n", row - 1, column, glm::to_string(point3).c_str());

					// log
					fprintf(this->file, "v %f %f %f\n", point1.x, point1.y, point1.z);
					fprintf(this->file, "v %f %f %f\n", point2.x, point2.y, point2.z);
					fprintf(this->file, "v %f %f %f\n", point3.x, point3.y, point3.z);
					int indexPoints = points->size();
					fprintf(this->file, "t %d %d %d\n", indexPoints - 2, indexPoints - 1, indexPoints);
				}
			}
			if (column < (this->columns - 1))
			{
				glm::vec3 point4(this->allPoints[row][column]);
				glm::vec3 point5(this->allPoints[row - 1][column]);
				glm::vec3 point6(this->allPoints[row][column + 1]);

				// normalize all vectors
				point4 = point4 * (1 / maxDistance);
				point5 = point5 * (1 / maxDistance);
				point6 = point6 * (1 / maxDistance);

				if (checkAnyEmptyPoint(point4, point5, point6)) {
					printf("Missing point at row %d column %d\n", row, column);
				} else {
					// push points in correct order to the points array for the view
					this->points->push_back(point4);
					this->points->push_back(point5);
					this->points->push_back(point6);

					// add the normals for these three points
					addNormal(point4, point5, point6);

					// printf("BAA Entry: (%i,%i): %s\n", row, column, glm::to_string(point4).c_str());
					// printf("BAA Entry: (%i,%i): %s\n", row - 1, column, glm::to_string(point5).c_str());
					// printf("BAA Entry: (%i,%i): %s\n", row, column + 1, glm::to_string(point6).c_str());

					// log
					fprintf(this->file, "v %f %f %f\n", point4.x, point4.y, point4.z);
					fprintf(this->file, "v %f %f %f\n", point5.x, point5.y, point5.z);
					fprintf(this->file, "v %f %f %f\n", point6.x, point6.y, point6.z);
					int indexPoints = points->size();
					fprintf(this->file, "t %d %d %d\n", indexPoints - 2, indexPoints - 1, indexPoints);
				}
			}
		}
	}
	// close log file
	fclose(this->file);
}

bool Calculation::checkAnyEmptyPoint(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	bool result = false;
	result |= p1.x == 0.0f && p1.y == 0.0f && p1.z == 0.0f;
	result |= p2.x == 0.0f && p2.y == 0.0f && p2.z == 0.0f;
	result |= p3.x == 0.0f && p3.y == 0.0f && p3.z == 0.0f;
	return result;
}

// faces.push_back(vec3); // Dreidimensionaler Vektor. x = 1. Punkt, y = 2. Punkt, z = 3. Punkt
void Calculation::addNormal(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3) {

	/* calculate Vector1 and Vector2 */
	glm::vec3 va, vb, vr;
	float val;
	va.x = point1.x - point2.x;
	va.y = point1.y - point2.y;
	va.z = point1.z - point2.z;

	vb.x = point1.x - point3.x;
	vb.y = point1.y - point3.y;
	vb.z = point1.z - point3.z;

	/* cross product */
	vr.x = va.y * vb.z - vb.y * va.z;
	vr.y = vb.x * va.z - va.x * vb.z;
	vr.z = va.x * vb.y - vb.x * va.y;

	/* normalization factor */
	val = sqrt( vr.x * vr.x + vr.y * vr.y + vr.z * vr.z );

	glm::vec3 normal;
	normal.x = vr.x / val;
	normal.y = vr.y / val;
	normal.z = vr.z / val;

	// push_back three times, because it is needed for all three points of the triangle
	this->normals->push_back(normal);
	this->normals->push_back(normal);
	this->normals->push_back(normal);

	// log
	fprintf(this->file, "f %f %f %f\n", normal.x, normal.y, normal.z);
	fprintf(this->file, "f %f %f %f\n", normal.x, normal.y, normal.z);
	fprintf(this->file, "f %f %f %f\n", normal.x, normal.y, normal.z);

}













