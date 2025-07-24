#include <glad/gl.h>

#include <cstring>
#include <fstream>
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Model.h"
#include "logging.h"

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

float rotation[3] = {0, 0, 0};

void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mod) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if ((key == GLFW_KEY_A && action == GLFW_PRESS) ||
        (key == GLFW_KEY_A && action == GLFW_REPEAT)) {
        rotation[1] -= 0.1;
    } else if ((key == GLFW_KEY_D && action == GLFW_PRESS) ||
               (key == GLFW_KEY_D && action == GLFW_REPEAT)) {
        rotation[1] += 0.1;
    } else if ((key == GLFW_KEY_S && action == GLFW_PRESS) ||
               (key == GLFW_KEY_S && action == GLFW_REPEAT)) {
        rotation[0] -= 0.1;
    } else if ((key == GLFW_KEY_W && action == GLFW_PRESS) ||
               (key == GLFW_KEY_W && action == GLFW_REPEAT)) {
        rotation[0] += 0.1;
    }
}
int main(int argc, char* argv[]) {
    glfwSetErrorCallback(error_callback);
    if (glfwInit() == GLFW_FALSE) {
        std::cerr << "Failed to init GLFW!\n";
        exit(1);
    }
    GLFWwindow* window = glfwCreateWindow(1280, 1280, "Renderer", NULL, NULL);

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    Model cube("../res/models/diablo3.obj", "../res/shaders/Basic.vert",
               "../res/shaders/Basic.frag");

    memcpy(rotation, cube.rotation, 3 * sizeof(float));

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugCallback, NULL);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        memcpy(cube.rotation, rotation, sizeof(rotation));

        glfwPollEvents();
        cube.render();
        glfwSwapBuffers(window);
    }

    return 0;
}
