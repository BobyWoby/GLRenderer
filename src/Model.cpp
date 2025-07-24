#include "Model.h"

#include <string>

#include "common.h"
#include "iostream"

Model::Model() {}

Model::Model(std::string filepath, std::string vertex_shader,
             std::string frag_shader) {
    parseFile(filepath);

    unsigned int buf;
    glGenBuffers(1, &buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);

    // each position has 3 components: x, y, and z, and there' no other data
    // stored in each vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);
    
    std::string vShader = fromFile(vertex_shader);
    std::string fShader = fromFile(frag_shader);


    program = glCreateProgram();

    unsigned int vs = compileShader(GL_VERTEX_SHADER, vShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);


    u_rotation = glGetUniformLocation(program, "u_Rot");
}

Model::~Model() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &ibo);
}

void Model::parseFile(std::string filepath) {
    std::ifstream file(filepath);
    std::string line, start;
    std::vector<std::string> elements;

    while (std::getline(file, line)) {
        if (line.length() < 1 || line.at(0) == '#') continue;
        elements = split(line, " ");
        if (elements.at(0) == "v") {
            // add to the vertices
            vertices.push_back(std::stof(elements.at(1)) * 0.5);
            vertices.push_back(std::stof(elements.at(2)) * 0.5);
            vertices.push_back(std::stof(elements.at(3)) * 0.5);

        } else if (elements.at(0) == "f") {
            // add to the indices
            for (int i = 1; i < elements.size(); i++) {
                auto object_indices = split(elements.at(i), "/");
                indices.push_back(std::stoi(object_indices.at(0)) - 1);
            }
        }
    }
}


unsigned int Model::compileShader(unsigned int type, std::string source) {
    uint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int compile_status;

    glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);

    if (compile_status == GL_FALSE) {
        // failed to compile
        int log_length = 0;
        char msg[1024];
        glGetShaderInfoLog(id, 1024, &log_length, msg);
        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!\n";
        std::cout << msg << "\n";
        glDeleteShader(id);
        return 0;
    }

    return id;
}

void Model::render() {
    // glUseProgram(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    glUseProgram(program);

    glUniform3f(u_rotation, rotation[0], rotation[1], rotation[2]);

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
}

