#include "Model.h"

#include <array>
#include <map>
#include <string>

#include "common.h"
#include "iostream"

Model::Model() {}

Model::Model(std::string filepath, std::string vertex_shader,
             std::string frag_shader) {
    parseFile(filepath);
    compileVertices();

    unsigned int buf;
    glGenBuffers(1, &buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    // std::vector<float> pos_verts;
    // for (auto pos : positions) {
    //     pos_verts.push_back(pos[0]);
    //     pos_verts.push_back(pos[1]);
    //     pos_verts.push_back(pos[2]);
    // }
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // each position has 3 components: x, y, and z, and there' no other data
    // stored in each vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    long tex_offset = 3.0 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)tex_offset);

    // std::vector<unsigned int> pos_indices;
    // for (auto index : indices) {
    //     // std::cout << "Position Index " << index[0] << ": "
    //     //           << positions.at(index[0])[0] << ", "
    //     //           << positions.at(index[0])[1] << ", "
    //     //           << positions.at(index[0])[2] << "\n";
    //     pos_indices.push_back(index[0]);
    // }

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 vertex_indices.size() * sizeof(unsigned int),
                 vertex_indices.data(), GL_STATIC_DRAW);

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
            std::array<float, 3> position;

            position[0] = (std::stof(elements.at(1)));
            position[1] = (std::stof(elements.at(2)));
            position[2] = (std::stof(elements.at(3)));
            // std::cout << position[0] << ", " << position[1] << ", "
            //           << position[2] << "\n";

            positions.push_back(position);
        } else if (elements.at(0) == "f") {
            // add to the indices
            for (int i = 1; i < elements.size(); i++) {
                auto object_indices = split(elements.at(i), "/");
                unsigned int pos_index = std::stoi(object_indices.at(0)) - 1;
                unsigned int tex_index = std::stoi(object_indices.at(1)) - 1;
                indices.push_back({pos_index, tex_index});
            }
        } else if (elements.at(0) == "vt") {
            // add to the texture_coords, if the size is only 3, then set w to 0
            std::array<float, 3> tex_coord{std::stof(elements.at(1)), 0.0, 0.0};

            if (elements.size() > 2) {
                tex_coord[1] = std::stof(elements.at(2));
            }
            if (elements.size() > 3) {
                tex_coord[2] = std::stof(elements.at(3));
            }
            texture_coords.push_back(tex_coord);
        }
    }
    // for(auto pos : positions){
    //     std::cout << pos[0] << ", " << pos[1] << ", " << pos[2] << "\n";
    // }
}

void Model::compileVertices() {
    std::map<std::array<float, 6>, int> seenVertices;  // position, tex

    for (auto index : indices) {
        std::array<float, 6> vertex;
        vertex[0] = positions.at(index[0])[0];
        vertex[1] = positions.at(index[0])[1];
        vertex[2] = positions.at(index[0])[2];

        vertex[3] = texture_coords.at(index[1])[0];
        vertex[4] = texture_coords.at(index[1])[1];
        vertex[5] = texture_coords.at(index[1])[2];

        auto it = seenVertices.find(vertex);
        uint pos;
        if (it != seenVertices.end()) {
            // this already exists
            // pos = std::distance(seenVertices.begin(), it);
            pos = seenVertices[vertex];
        } else {
            for (float attribute : vertex) {
                vertices.push_back(attribute);
            }
            pos = seenVertices.size();
            seenVertices.insert({vertex, pos});
        }
        vertex_indices.push_back(pos);
    }
}

unsigned int Model::compileShader(unsigned int type, std::string source) {
    uint id = glCreateShader(type);
    const char *src = source.c_str();
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

    glDrawElements(GL_TRIANGLES, vertex_indices.size(), GL_UNSIGNED_INT, NULL);
}
