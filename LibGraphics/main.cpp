#include "GFXWindow.h"
#include "GFXObject.h"
#include "GFXShader.h"

int main() {
    GFXWindow window(740, 740, "Hello, World!");

    window.makeContextCurrent();
    window.setInputMode(GLFW_STICKY_KEYS, GL_TRUE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    GFXShader shader(
        R"(#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})",
        R"(#version 330 core
uniform vec3 inColor;
out vec3 FragColor;
void main()
{
    FragColor = inColor;
} )"
    );

    shader.use();

    GFXObjectHints hints;
    hints.staticVertices = 1;

    GFXObject<2> vertical(hints);
    GFXObject<2> horizontal(hints);
    GFXObject<2> whole(hints);

    vertical.addVertex({ 0, 0 });
    vertical.addVertex({ 0, .7 });

    horizontal.addVertex({ 0, 0 });
    horizontal.addVertex({ .7, 0 });

    whole.addVertex({ 0, 0 });
    whole.addVertex({ .7, .7 });

    GLuint uniformLocation = shader.getUniform("inColor");

    do {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUniform3f(uniformLocation, 0, 1, 1);
        horizontal.render(GL_LINES);

        glUniform3f(uniformLocation, 1, 0, 0);
        vertical.render(GL_LINES);
        
        glUniform3f(uniformLocation, 1, 1, 1);
        whole.render(GL_LINES);

        window.swapBuffers();
        glfwPollEvents();
    } while (!window.shouldClose());
}