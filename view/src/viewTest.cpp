#include <iostream>
#include <vector>
#include "stdio.h"
#include "../includes/view.h"

bool loadObj(const char* path, std::vector<glm::vec3> &points, std::vector<glm::vec3> &faces);

int main(int argc, char const *argv[]) {
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> faces;
    const char* path = "testFiles/monkey.obj";
    loadObj(path, points, faces);

    View* view = new View(points, faces);
    view->startScreen();
    return 0;
}

bool loadObj(const char* path, std::vector<glm::vec3> &points, std::vector<glm::vec3> &faces) {
    FILE * file = fopen(path, "r");
    if ( file == NULL ) {
        printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        getchar();
        return false;
    }
    while ( 1 ) {

        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) {
            break;
        }
        if ( strcmp( lineHeader, "v" ) == 0 ) {
            glm::vec3 tmp_point;
            fscanf(file, "%f %f %f\n", &tmp_point.x, &tmp_point.y, &tmp_point.z );
            points.push_back(tmp_point);
        } else if ( strcmp( lineHeader, "f" ) == 0 ) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9) {
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                return false;
            }
        } else {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    return true;
}
