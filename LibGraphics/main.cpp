#include "GFXWindow.h"
#include "GFXObject.h"
#include "GFXShader.h"
#include "GFXCamera.h"

#include "ExampleApps.h"

/*
[[Possible Class Name GFXMaterial]]
VAO abstraction
 - stores layout of vertex data
 - stores list of objects
*/

const char* vertexShader = R"(
#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 vertexColor;

uniform mat4 MVP;

out vec3 fragmentColor;

void main() {
    gl_Position = MVP * vec4(inPos, 1.0);
    fragmentColor = vertexColor;
}
)";

const char* fragmentShader = R"(
#version 330 core

in vec3 inColor;
out vec3 outColor;

void main() {
    //outColor = inColor;
    outColor = vec3(1,0,0);
}
)";

using namespace glm;

void GLAPIENTRY
MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
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

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GFXShader shader(
        vertexShader,
        fragmentShader
    );

    shader.use();

    GFXObject<3> triangle = {
        {-1.0f,-1.0f,-1.0f,},
        {-1.0f,-1.0f, 1.0f,},
        {-1.0f, 1.0f, 1.0f,},
        {1.0f, 1.0f,-1.0f, },
        {-1.0f,-1.0f,-1.0f,},
        {-1.0f, 1.0f,-1.0f,},
        {1.0f,-1.0f, 1.0f, },
        {-1.0f,-1.0f,-1.0f,},
        {1.0f,-1.0f,-1.0f, },
        {1.0f, 1.0f,-1.0f, },
        {1.0f,-1.0f,-1.0f, },
        {-1.0f,-1.0f,-1.0f,},
        {-1.0f,-1.0f,-1.0f,},
        {-1.0f, 1.0f, 1.0f,},
        {-1.0f, 1.0f,-1.0f,},
        {1.0f,-1.0f, 1.0f, },
        {-1.0f,-1.0f, 1.0f,},
        {-1.0f,-1.0f,-1.0f,},
        {-1.0f, 1.0f, 1.0f,},
        {-1.0f,-1.0f, 1.0f,},
        {1.0f,-1.0f, 1.0f, },
        {1.0f, 1.0f, 1.0f, },
        {1.0f,-1.0f,-1.0f, },
        {1.0f, 1.0f,-1.0f, },
        {1.0f,-1.0f,-1.0f, },
        {1.0f, 1.0f, 1.0f, },
        {1.0f,-1.0f, 1.0f, },
        {1.0f, 1.0f, 1.0f, },
        {1.0f, 1.0f,-1.0f, },
        {-1.0f, 1.0f,-1.0f,},
        {1.0f, 1.0f, 1.0f, },
        {-1.0f, 1.0f,-1.0f,},
        {-1.0f, 1.0f, 1.0f,},
        {1.0f, 1.0f, 1.0f, },
        {-1.0f, 1.0f, 1.0f,},
        {1.0f,-1.0f, 1.0f  }
    };

    GFXDataBuffer<3> vertexColorData = {
        {0.583f,  0.771f,  0.014f},
        {0.609f,  0.115f,  0.436f},
        {0.327f,  0.483f,  0.844f},
        {0.822f,  0.569f,  0.201f},
        {0.435f,  0.602f,  0.223f},
        {0.310f,  0.747f,  0.185f},
        {0.597f,  0.770f,  0.761f},
        {0.559f,  0.436f,  0.730f},
        {0.359f,  0.583f,  0.152f},
        {0.483f,  0.596f,  0.789f},
        {0.559f,  0.861f,  0.639f},
        {0.195f,  0.548f,  0.859f},
        {0.014f,  0.184f,  0.576f},
        {0.771f,  0.328f,  0.970f},
        {0.406f,  0.615f,  0.116f},
        {0.676f,  0.977f,  0.133f},
        {0.971f,  0.572f,  0.833f},
        {0.140f,  0.616f,  0.489f},
        {0.997f,  0.513f,  0.064f},
        {0.945f,  0.719f,  0.592f},
        {0.543f,  0.021f,  0.978f},
        {0.279f,  0.317f,  0.505f},
        {0.167f,  0.620f,  0.077f},
        {0.347f,  0.857f,  0.137f},
        {0.055f,  0.953f,  0.042f},
        {0.714f,  0.505f,  0.345f},
        {0.783f,  0.290f,  0.734f},
        {0.722f,  0.645f,  0.174f},
        {0.302f,  0.455f,  0.848f},
        {0.225f,  0.587f,  0.040f},
        {0.517f,  0.713f,  0.338f},
        {0.053f,  0.959f,  0.120f},
        {0.393f,  0.621f,  0.362f},
        {0.673f,  0.211f,  0.457f},
        {0.820f,  0.883f,  0.371f},
        {0.982f,  0.099f,  0.879f}
    };

    //

    GFXCamera camera(
        vec3(3, 3, 3),
        vec3(0,0,0),
        WIDTH,
        HEIGHT,
        45
    );
    
    GLuint MVPMatrixLoc = shader.getUniform("MVP");
    
    GLfloat cameraTheta = 0;

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        triangle.render(GL_TRIANGLES);
        
        camera.view() = lookAt(
            vec3(cosf(cameraTheta) * 10.f, 2.f, sinf(cameraTheta) * 10.f),
            vec3(0, 0, 0),
            vec3(0, 1, 0)
        );

        cirInc(cameraTheta, .001f, 0, radians(360.f));

        camera.loadModelViewProjection(MVPMatrixLoc);

        window.swapBuffers();
        glfwPollEvents();
    } while (!window.shouldClose());
}