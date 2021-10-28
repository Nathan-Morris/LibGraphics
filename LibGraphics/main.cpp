#include "GFXWindow.h"
#include "GFXObject.h"
#include "GFXShader.h"

#include "ExampleApps.h"

const char* vertexShader = R"(
#version 330 core

layout (location = 0) in vec3 inPos;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(inPos, 1.0);
    //gl_Position = vec4(inPos, 1.0);
}
)";

const char* fragmentShader = R"(
#version 330 core

out vec3 outColor;

uniform vec3 inColor;

void main() {
    //outColor = inColor;
    outColor = vec3(1, 0, 0);
}
)";

using namespace glm;

int main() {
    int WIDTH = 1024, HEIGHT = 512;

    GFXWindow window(WIDTH, HEIGHT, "Physics");

    window.makeContextCurrent();
    window.setInputMode(GLFW_STICKY_KEYS, GL_TRUE);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GFXShader shader(
        vertexShader,
        fragmentShader
    );

    shader.use();

    GFXObject<3> triangle = {
        { -1.0f, -1.0f, 0.0f },
        {1.0f, -1.0f, 0.0f },
        {0.0f,  1.0f, 0.0f }
    };

    //
    
    vec3 up(0.f, 1.f, 0.f);

    vec3 cameraTarget(0.f, 0.f, 0.f);

    mat4 view = lookAt(
        vec3(3.f, 3.f, 3.f),
        cameraTarget,
        up
    );

    mat4 project = perspective(radians(45.f), (float)WIDTH / HEIGHT, .1f, 100.f);

    mat4 model(1.f);

    mat4 mvp = project * view * model;

    //

    GLuint MVPMatrixLoc = shader.getUniform("MVP");

    glUniformMatrix4fv(MVPMatrixLoc, 1, GL_FALSE, value_ptr(mvp));

    //
    //
    //

    static const float triangleBuffer[] = {
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        0.0f,  1.0f, 1.0f,
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(triangleBuffer),
        triangleBuffer,
        GL_STATIC_DRAW
    );


    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //triangle.render(GL_TRIANGLES);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE, 0, NULL
        );

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        window.swapBuffers();
        glfwPollEvents();
    } while (!window.shouldClose());
}