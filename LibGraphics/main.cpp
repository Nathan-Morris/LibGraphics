#include "GFXWindow.h"
#include "GFXObject.h"
#include "GFXCamera.h"
#include "GFXMaterial.h"
#include "GFXShaderProgram.h"

/*
[[Possible Class Name GFXMaterial]]
VAO abstraction
 - stores layout of vertex data
 - stores list of objects

GFXShader
 - uniform
 - layouts
 - fragment passing support
*/

const char* vertexShader = R"(
#version 330 core
layout (location = 0) in vec3 inPos;
uniform mat4 MVP;
out vec3 fragmentColor;
void main() {
    gl_Position = MVP * vec4(inPos, 1.0);
}
)";

const char* fragmentShader = R"(
#version 330 core
out vec3 outColor;
void main() {
    outColor = vec3(1,0,0);
}
)";

using namespace glm;

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    fprintf(stderr, "GL: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

static void transLate(GFXVertex<3>& vertex) {
    vertex.at(1) += .5;
}


static inline GLfloat& cirInc(GLfloat& in, GLfloat inc, GLfloat lower, GLfloat upper) {
    in += inc;
    if (in > upper)
        in = lower;
    return in;
}

int main() {
    int WIDTH = 1024, HEIGHT = 512;

    GFXWindow window(WIDTH, HEIGHT, "Physics");

    window.makeContextCurrent();
    window.setInputMode(GLFW_STICKY_KEYS, GL_TRUE);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    //
    //
    //

    GFXShaderProgram shader;

    shader.addShader(
        GFXShader(GL_VERTEX_SHADER).setCode(vertexShader)
    ).addShader(
        GFXShader(GL_FRAGMENT_SHADER).setCode(fragmentShader)
    );

    GFXMaterial<3> cube(shader);
    cube.putVertexBuffer(0, {
        {-1.0f,-1.0f,-1.0f,}, {-1.0f,-1.0f, 1.0f,}, {-1.0f, 1.0f, 1.0f,}, {1.0f, 1.0f,-1.0f, },
        {-1.0f,-1.0f,-1.0f,}, {-1.0f, 1.0f,-1.0f,}, {1.0f,-1.0f, 1.0f, }, {-1.0f,-1.0f,-1.0f,},
        {1.0f,-1.0f,-1.0f, }, {1.0f, 1.0f,-1.0f, }, {1.0f,-1.0f,-1.0f, }, {-1.0f,-1.0f,-1.0f,},
        {-1.0f,-1.0f,-1.0f,}, {-1.0f, 1.0f, 1.0f,}, {-1.0f, 1.0f,-1.0f,}, {1.0f,-1.0f, 1.0f, },
        {-1.0f,-1.0f, 1.0f,}, {-1.0f,-1.0f,-1.0f,}, {-1.0f, 1.0f, 1.0f,}, {-1.0f,-1.0f, 1.0f,},
        {1.0f,-1.0f, 1.0f, }, {1.0f, 1.0f, 1.0f, }, {1.0f,-1.0f,-1.0f, }, {1.0f, 1.0f,-1.0f, },
        {1.0f,-1.0f,-1.0f, }, {1.0f, 1.0f, 1.0f, }, {1.0f,-1.0f, 1.0f, }, {1.0f, 1.0f, 1.0f, },
        {1.0f, 1.0f,-1.0f, }, {-1.0f, 1.0f,-1.0f,}, {1.0f, 1.0f, 1.0f, }, {-1.0f, 1.0f,-1.0f,},
        {-1.0f, 1.0f, 1.0f,}, {1.0f, 1.0f, 1.0f, }, {-1.0f, 1.0f, 1.0f,}, {1.0f,-1.0f, 1.0f  }
    });
    cube.shader().compile();
    cube.shader().link();



    GFXMaterial<3> pyramid(
        GFXShaderProgram().addShader(
            GFXShader(GL_VERTEX_SHADER).setCode(
                "\n#version 330 core\nlayout(location = 0) in vec3 inPos;\nuniform mat4 MVPPyramid;\nout vec3 fragmentColor;\nvoid main() {\ngl_Position = MVPPyramid * vec4(inPos, 1.0);\n}"
            )
        ).addShader(
            GFXShader(GL_FRAGMENT_SHADER).setCode(
                "#version 330 core\nout vec3 outColor;\nvoid main() {\noutColor = vec3(0,1,0);\n}"
            )
        )
    );
    pyramid.putVertexBuffer(0, {
        { 0, 1, 0 }, {.5, 0, -.5}, {.5, 0, .5},
        { 0, 1, 0 }, {.5, 0, .5}, {-.5, 0, .5},
        { 0, 1, 0 }, {-.5, 0, -.5}, {-.5, 0, .5},
        { 0, 1, 0 }, {-.5, 0, -.5}, {.5, 0, -.5},
        { -.5, 0, .5 }, {-.5, 0, -.5}, {.5, 0, -.5},
        { -.5, 0, .5 }, {.5, 0, .5}, {.5, 0, -.5},
    });
    pyramid.shader().compile();
    pyramid.shader().link();
    pyramid.getVertexBuffer(0).dataTranslate(transLate);

    //

    GFXCamera camera(
        vec3(4, 4, 4),
        vec3(0,0,0),
        WIDTH,
        HEIGHT,
        45
    );
    
    GLfloat cameraTheta = 0;

    //glEnable(GL_DEPTH_TEST);

    bool which = false;

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pyramid.drawArrays(GL_TRIANGLES);
        cube.drawArrays(GL_TRIANGLES);

        // this works, but good god...
       /* if (which) {
            pyramid.drawArrays(GL_TRIANGLES);
            cube.drawArrays(GL_TRIANGLES);
        }
        else {
            cube.drawArrays(GL_TRIANGLES);
            pyramid.drawArrays(GL_TRIANGLES);
        }
        which = !which;*/

        camera.view() = lookAt(
            vec3(cosf(cameraTheta) * 10.f, 2.f, sinf(cameraTheta) * 10.f),
            vec3(0, 0, 0),
            vec3(0, 1, 0)
        );

        cirInc(cameraTheta, .01f, 0, radians(360.f));

        camera.loadModelViewProjection(cube.shader().uniformLocation("MVP"));
        camera.loadModelViewProjection(pyramid.shader().uniformLocation("MVPPyramid"));

        window.swapBuffers();
        glfwPollEvents();
    } while (!window.shouldClose());
}