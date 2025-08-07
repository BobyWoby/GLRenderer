#include "Shader.h"

#include <iostream>

#include "common.h"

unsigned int Shader::compileShader(unsigned int type, std::string source) {
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

Shader::Shader(){
}

Shader::Shader(const std::string &vertexPath, const std::string &fragPath) {
    m_rendererId = glCreateProgram();
    std::string vsrc = fromFile(vertexPath);
    std::string fsrc = fromFile(fragPath);

    unsigned int vs = compileShader(GL_VERTEX_SHADER, vsrc);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fsrc);

    glAttachShader(m_rendererId, vs);
    glAttachShader(m_rendererId, fs);

    glLinkProgram(m_rendererId);
    glValidateProgram(m_rendererId);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

/**
 * @brief Binds the shader program to the OpenGL context
 */
void Shader::Bind() { glUseProgram(m_rendererId); }

/**
 * @brief assigns a single integer uniform to the shader program
 *
 * @param name The specific name inside of the shader program
 * @param val the value to assign to the uniform
 */
void Shader::uniform1i(const std::string &name, int val) {
    glUniform1i(glGetUniformLocation(m_rendererId, name.c_str()), val);
}


void Shader::uniform2i(const std::string &name, int val1, int val2) {
    glUniform2i(glGetUniformLocation(m_rendererId, name.c_str()), val1, val2);
}


void Shader::uniform1ui(const std::string &name, unsigned int val) {
    glUniform1ui(glGetUniformLocation(m_rendererId, name.c_str()), val);
}




void Shader::uniform2f(const std::string &name, float val1, float val2) {
    glUniform2f(glGetUniformLocation(m_rendererId, name.c_str()), val1, val2);
}

void Shader::uniform3f(const std::string &name, float val1, float val2,
                       float val3) {
    glUniform3f(glGetUniformLocation(m_rendererId, name.c_str()), val1, val2,
                val3);
}
