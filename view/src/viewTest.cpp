#include <iostream>
#include <vector>
#include "stdio.h"
#include "../includes/view.h"

bool loadObj(const char* path, std::vector<glm::vec3> &points, std::vector<glm::vec3> &faces);
glm::vec3 calculateNormal( glm::vec3 coord1, glm::vec3 coord2, glm::vec3 coord3 );
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
            glm::vec3 coord1 = points.at(normalIndex[0] - 1);
            glm::vec3 coord2 = points.at(normalIndex[1] - 1);
            glm::vec3 coord3 = points.at(normalIndex[2] - 1);

            glm::vec3 normal = calculateNormal(coord1, coord2, coord3);
            faces.push_back(normal);
        } else {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }
    return true;
}

void printVec3(const char * name, glm::vec3 vec) {
    printf("%s x: %f y: %f z: %f\n", name, vec.x, vec.y, vec.z);
}

// float* calculateNormal( float *coord1, float *coord2, float *coord3 )
glm::vec3 calculateNormal( glm::vec3 coord1, glm::vec3 coord2, glm::vec3 coord3 ) {
    /* calculate Vector1 and Vector2 */
    glm::vec3 va, vb, vr;
    float val;
    va.x = coord1.x - coord2.x;
    va.y = coord1.y - coord2.y;
    va.z = coord1.z - coord2.z;
    // printVec3("va", va);

    vb.x = coord1.x - coord3.x;
    vb.y = coord1.y - coord3.y;
    vb.z = coord1.z - coord3.z;
    // printVec3("vb", vb);


    /* cross product */
    vr.x = va.y * vb.z - vb.y * va.z;
    vr.y = vb.x * va.z - va.x * vb.z;
    vr.z = va.x * vb.y - vb.x * va.y;
    // printVec3("vr", vr);


    /* normalization factor */
    val = sqrt( vr.x * vr.x + vr.y * vr.y + vr.z * vr.z );

    // printf("%f\n", val);

    glm::vec3 norm;
    norm.x = vr.x / val;
    norm.y = vr.y / val;
    norm.z = vr.z / val;
    printVec3("norm", norm);


    return norm;
}
