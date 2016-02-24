#include "../includes/view.h"
void startScreen();
GLuint loadSchader(const char* path, GLenum type);

View::View(std::vector<glm::vec3> &_points, std::vector<glm::vec3> &_faces) {
    this->points = _points;
    this->faces = _faces;
    initScreen();
    // Create and compile our GLSL program from the shaders
    //TODO fix path!!!!
    GLuint vertexShaderId =  loadSchader("src/TransformVertexShader.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShaderId =  loadSchader("src/TextureFragmentShader.glsl", GL_FRAGMENT_SHADER);
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderId);
    glAttachShader(programID, fragmentShaderId);
    glLinkProgram(programID);
    glUseProgram(programID);
    // programID = LoadShaders( "src/TransformVertexShader.glsl", "src/TextureFragmentShader.glsl" );

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    ViewMatrixID = glGetUniformLocation(programID, "V");
    ModelMatrixID = glGetUniformLocation(programID, "M");


    // Get a handle for our buffers
    vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    ViewLookAt = glm::lookAt(
                     glm::vec3(0, 0.5f, 0), // Camera is at origin in World Space
                     // glm::vec3(4, 1, 3), // Camera is at (4,1,3), in World Space
                     glm::vec3(1, 0.5f, 0), // and looks at direction x
                     glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
                 );
    // Model matrix : an identity matrix (model will be at the origin)
    Model = glm::mat4(1.0f);

    // Transform for Model
    position = glm::vec3(0, 0, 0);
    rotation = glm::vec3(0, 0, 0);
    scale = glm::vec3(1, 1, 1);

    // Load it into a VBO
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, faces.size() * sizeof(glm::vec3), &faces[0], GL_STATIC_DRAW);

    glUseProgram(programID);
    LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
}

View::~View() {

}

void View::startScreen() {
    printf("%d\n", points.size());
    printf("%d\n", faces.size());
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        // Rebuild the Model matrix
        rotation.y += 0.01f;
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix    = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
        glm::mat4 scalingMatrix     = glm::scale(glm::mat4(1.0f), scale);

        Model = translationMatrix * rotationMatrix * scalingMatrix;
        glm::mat4 MVP = Projection * ViewLookAt * Model; // Remember, matrix multiplication is the other way around

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewLookAt[0][0]);

        glm::vec3 lightPos = glm::vec3(0, 0.5f, 0);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        // 1rst attribute buffer : points
        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            vertexPosition_modelspaceID,  // The attribute we want to configure
            3,                            // size
            GL_FLOAT,                     // type
            GL_FALSE,                     // normalized?
            0,                            // stride
            (void*)0                      // array buffer offset
        );

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(vertexNormal_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(
            vertexNormal_modelspaceID, // The attribute we want to configure
            3, // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            (void*)0 // array buffer offset
        );

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, points.size() );

        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexUVID);
        glDisableVertexAttribArray(vertexNormal_modelspaceID);

        updateScreen();

    } while (1);
}

void View::updateScreen() {
    eglSwapBuffers(this->GDisplay, GSurface);
}

void View::setViewport() {
    glViewport ( 0, 0, GScreenWidth, GScreenHeight );
}

void View::initScreen() {
    bcm_host_init();
    EGLint num_config;

    static EGL_DISPMANX_WINDOW_T nativewindow;

    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    static const EGLint attribute_list[] =
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 16,   // You need this line for depth buffering to work
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    static const EGLint context_attributes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    EGLConfig config;

    this->GDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(this->GDisplay, NULL, NULL);
    eglChooseConfig(this->GDisplay, attribute_list, &config, 1, &num_config);
    eglBindAPI(EGL_OPENGL_ES_API);
    GContext = eglCreateContext(this->GDisplay, config, EGL_NO_CONTEXT, context_attributes);
    graphics_get_display_size(0 /* LCD */, &GScreenWidth, &GScreenHeight);
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = GScreenWidth;
    dst_rect.height = GScreenHeight;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = GScreenWidth << 16;
    src_rect.height = GScreenHeight << 16;

    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );

    dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
                      0/*layer*/, &dst_rect, 0/*src*/,
                      &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);
    nativewindow.element = dispman_element;
    nativewindow.width = GScreenWidth;
    nativewindow.height = GScreenHeight;
    vc_dispmanx_update_submit_sync( dispman_update );

    GSurface = eglCreateWindowSurface( this->GDisplay, config, &nativewindow, NULL );
    eglMakeCurrent(this->GDisplay, GSurface, GSurface, GContext);

    // Set background color and clear buffers
    glClearColor(0.15f, 0.25f, 0.35f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );

    glViewport ( 0, 0, GScreenWidth, GScreenHeight );

    printf("Screen started\n");

    // Dark blue background
    // glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
}

GLuint loadSchader(const char* path, GLenum type) {
    FILE* f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    int sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    GLchar* Src = new GLchar[sz + 1];
    fread(Src, 1, sz, f);
    Src[sz] = 0; //null terminate it!
    fclose(f);

    GLuint result = glCreateShader(type);
    glShaderSource(result, 1, (const GLchar**)&Src, 0);
    glCompileShader(result);

    return result;
}
