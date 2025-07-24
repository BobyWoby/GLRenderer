#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <vector>

#include "logging.h"

class Model {
   private:
    void parseFile(std::string filepath);
    unsigned int compileShader(unsigned int type, std::string source);

    unsigned int vao, ibo;  // the vertex array object and index buffer object
    unsigned int program;
    int u_rotation;

   public:
    float rotation[3] = {0,0,0};
    std::vector<float> vertices;
    std::vector<unsigned int> indices;  // face indices

    Model();
    ~Model();
    Model(std::string filepath, std::string vertex_shader,
          std::string frag_shader);  // parse .obj file

    void render();
};
