#include <iostream>
#include <vector>
#include "stdio.h"
#include "../includes/view.h"

bool loadObj(const char* path, std::vector<glm::vec3> &points, std::vector<glm::vec3> &faces);
glm::vec3 calculateNormal( glm::vec3 coord1, glm::vec3 coord2, glm::vec3 coord3 );
int main(int argc, char const *argv[]) {
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> faces;
    const char* path = "testFiles/realData.log";
    loadObj(path, points, faces);
    printf("points : %d normales: %d\n", points.size(), faces.size());
    View* view = new View(points, faces);
    view->startScreen();
    return 0;
}

bool loadObj(const char* path, std::vector<glm::vec3> &points, std::vector<glm::vec3> &faces) {
    FILE * file = fopen(path, "r");
    if ( file == NULL ) {
        printf("Impossible to open the file !\n");
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
            float normalCoord[3];
            int matches = fscanf(file, "%f %f %f\n", &normalCoord[0], &normalCoord[1], &normalCoord[2] );
            if (matches != 3) {
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                return false;
            }
            glm::vec3 normal = glm::vec3(normalCoord[0], normalCoord[1], normalCoord[2]);
            faces.push_back(normal);
        } else {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }
    return true;
}