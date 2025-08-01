#pragma once

#include <glad/gl.h>
#include <tuple>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <vector>

#include "logging.h"

struct Vertex{
    float position[3];
    float tex_coords[2];

    void appendVertex(std::vector<float> vec){
        vec.push_back(position[0]);
        vec.push_back(position[1]);
        vec.push_back(position[2]);

        vec.push_back(tex_coords[0]);
        vec.push_back(tex_coords[1]);
    }

};


class Model {
   private:
    void parseFile(std::string filepath);
    void compileVertices();
    unsigned int compileShader(unsigned int type, std::string source);

    unsigned int vao, ibo;  // the vertex array object and index buffer object
    unsigned int program;
    int u_rotation;

   public:
    float rotation[3] = {0,0,0};
    float color[3] = {0,0,0};
    std::vector<float> vertices;
    std::vector<std::array<float, 3>> positions;
    std::vector<std::array<float, 3>> texture_coords;
    std::vector<std::array<unsigned int, 2>> indices;  // face/texture indices
    std::vector<uint> vertex_indices;

    Model();
    ~Model();
    Model(std::string filepath, std::string vertex_shader,
          std::string frag_shader);  // parse .obj file

    void render();
};
