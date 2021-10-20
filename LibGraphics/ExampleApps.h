#include "GFXWindow.h"
#include "GFXObject.h"
#include "GFXShader.h"

#pragma once

typedef struct {
    GLfloat x;
    GLfloat y;
} GLfloatXY;

static void projectMotionVectorCalc(GLfloatXY* pCoord, GLfloat vInit, GLfloat time, GLfloat g, GLfloat radTheta) {
    pCoord->y = vInit * sinf(radTheta) - (g * time);
    pCoord->x = vInit * cosf(radTheta); // redundant, does not need to be calculated each time
}

static void appProjectileMotion() {
    GFXWindow window(1024, 512, "Physics");

    window.makeContextCurrent();
    window.setInputMode(GLFW_STICKY_KEYS, GL_TRUE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GFXShader shader(
        R"(
#version 330 core

layout (location = 0) in vec3 inPos;

out vec4 vertexOutColor;

void main() {
    gl_Position = vec4(inPos, 1);
    vertexOutColor = vec4(.5, 0, 0, 1);
})",
R"(
#version 330 core

out vec3 outColor;

uniform vec3 inColor;

void main() {
    outColor = inColor;
    //outColor = vec3(1, 1, 1);
}
)");

    shader.use();

    GFXObject<2> vectorAll = {
        { 0 , 0 },
        { 0 , 0 }
    };
    GFXObject<2> vectorX = {
        { 0 , 0 },
        { 0 , 0 }
    };
    GFXObject<2> vectorY = {
        { 0 , 0 },
        { 0 , 0 }
    };

    GLfloat graphX0 = -.9f;
    GLfloat graphY0 = -.9f;

    GFXObject<2> graphXY = {
        { graphX0, -1.f },
        { graphX0, 1.f },
        { -1.f, graphY0 },
        { 1.f, graphY0 }
    };

    GFXObject<2> graphPlot;

    GLuint inColorLocation = shader.getUniform("inColor");

    GLfloat velocityInit = 100;
    GLfloat g = 9.81;
    GLfloat theta = glm::radians((GLfloat)70.f);

    GLfloatXY velocityVector = {
       velocityInit * cosf(theta),
       velocityInit * sinf(theta)
    };

    GLfloat maxY = (velocityInit * sinf(theta)) * (velocityInit * sinf(theta)) / (2.f * g);
    GLfloat maxX = velocityInit * velocityInit * sinf(2.f * theta) / g;

    GLfloat graphXScale = 1.9 / maxX;
    GLfloat vectorComponentScale = 150.f;

    GLfloat dTime = 2.f * velocityInit * sinf(theta) / g, time = 0.f, timeStep = .005f;

    for (
        ;
        time < dTime;
        time += timeStep
        ) {
        graphPlot.addVertex({
            ((velocityInit * cosf(theta) * time) * graphXScale) + graphX0,
            (((velocityInit * sinf(theta) * time) - (.5f * g * time * time)) * graphXScale) + graphY0
            });
    }

    time = 0.f;

    do {
        glClear(GL_COLOR_BUFFER_BIT);

        // render graph XY lines
        glUniform3f(inColorLocation, 1.f, 1.f, 1.f);
        graphXY.render(GL_LINES);

        // render graph plot
        graphPlot.render(GL_POINTS);

        // set vector vertices
        if (window.isKeyPressed(GLFW_KEY_RIGHT)) {
            vectorX.getVertex(0) = {
                ((velocityInit * cosf(theta) * time) * graphXScale) + graphX0,
                (((velocityInit * sinf(theta) * time) - (.5f * g * time * time)) * graphXScale) + graphY0
            };

            vectorX.getVertex(1) = {
                vectorX.getVertex(0).at(0) + (velocityVector.x / vectorComponentScale),
                vectorX.getVertex(0).at(1)
            };

            vectorY.getVertex(0) = vectorX.getVertex(0);

            vectorY.getVertex(1) = {
                 vectorY.getVertex(0).at(0),
                 vectorY.getVertex(0).at(1) + (velocityVector.y / vectorComponentScale),
            };

            vectorAll.getVertex(0) = vectorX.getVertex(0);

            vectorAll.getVertex(1) = {
                vectorX.getVertex(1).at(0),
                vectorY.getVertex(1).at(1)
            };

            projectMotionVectorCalc(&velocityVector, velocityInit, time, g, theta);

            time += timeStep;
        }

        // render vector
        glUniform3f(inColorLocation, 0.f, 1.f, 0.f);
        vectorAll.render(GL_LINES);

        // render vector y component
        glUniform3f(inColorLocation, 1.f, 0.f, 0.f);
        vectorY.render(GL_LINES);

        // render vector x component
        glUniform3f(inColorLocation, 0.f, 0.f, 1.f);
        vectorX.render(GL_LINES);


        window.swapBuffers();
        glfwPollEvents();
    } while (!window.shouldClose());
}



