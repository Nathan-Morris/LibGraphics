#include "GFXWindow.h"
#include "GFXObject.h"
#include "GFXShader.h"

const char* vertexShader = R"(
#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

out vec3 vertexColor;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelTranslate;

void main() {
    gl_Position = uProjectionMatrix * uModelTranslate * vec4(inPos.xyz, 1.0);
    vertexColor = inColor;
}
)";

const char* fragmentShader = R"(
#version 330 core

out vec4 fragColor;

in vec4 vertexColor;

void main() {
    fragColor = vertexColor;
}
)";

int main() {
    GFXWindow window(740, 740, "Hello, World!");

    window.makeContextCurrent();
    window.setInputMode(GLFW_STICKY_KEYS, GL_TRUE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    GFXShader shader(
        vertexShader,
        fragmentShader
    );

    shader.use();

    GFXObjectHints hints = { 0 };
    hints.staticVertices = 1;

    GFXObject<3> cube(hints);

    cube.addVertex({ 0, 1, 0 });
    cube.addVertex({ -1, 0, -1 });
    cube.addVertex({ 1, 0, -1 });

    cube.addVertex({ 0, 1, 0 });
    cube.addVertex({ -1, 0, -1 }); 
    cube.addVertex({ -1, 0, 1 });

    cube.addVertex({ 0, 1, 0 });
    cube.addVertex({ 1, 0, 1 });
    cube.addVertex({ -1, 0, 1 });

    cube.addVertex({ 0, 1, 0 });
    cube.addVertex({ 1, 0, 1 });
    cube.addVertex({ 1, 0, -1 });

    cube.addVertex({ 0, 1, 0 });
    cube.addVertex({ 1, 0, 1 });
    cube.addVertex({ 1, 0, -1 });

    cube.addVertex({ -1, 0, 1 });
    cube.addVertex({ 1, 0, 1 });
    cube.addVertex({ -1, 0, -1 });

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.f), glm::vec3(1.0f, 1.0f, 0.0f));
    glm::mat4 modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
    glm::mat4 view = glm::lookAt(glm::vec3(1, 0, -3), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelview = view * modelTransform * rotation;
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.f), (float)740 / (float)740, .1f, 10.0f);

    GLuint matTransformLocation = shader.getUniform("uModelTranslate");
    GLuint matProjectionLocation = shader.getUniform("uProjectionMatrix");

    glEnable(GL_DEPTH_TEST);

    do {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUniformMatrix4fv(matTransformLocation, 1, GL_FALSE, glm::value_ptr(modelview));
        glUniformMatrix4fv(matProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


        cube.render(GL_TRIANGLES);

        window.swapBuffers();
        glfwPollEvents();
    } while (!window.shouldClose());
}