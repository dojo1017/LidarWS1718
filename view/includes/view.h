
#ifndef View_H_
#define View_H_

#define GLM_FORCE_RADIANS

#include <vector>
#include "stdio.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include "bcm_host.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

class View {
private:
    uint32_t GScreenWidth;
    uint32_t GScreenHeight;
    EGLDisplay GDisplay;
    EGLSurface GSurface;
    EGLContext GContext;

    GLuint programID;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 Model;
    glm::mat4 Projection;
    GLuint MatrixID;
    GLuint vertexPosition_modelspaceID;
    glm::mat4 ViewLookAt;
    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint LightID;
    GLuint vertexNormal_modelspaceID;


    std::vector<glm::vec3> points;
    std::vector<glm::vec3> faces;
    GLuint vertexUVID;


    void setViewport();
    void initScreen();
    void drawScreen();
public:
    void startScreen();
    void updateScreen();
    View(std::vector<glm::vec3> &points, std::vector<glm::vec3> &faces);
    ~View();
};

#endif /* View_H_ */
