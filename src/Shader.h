#pragma once
#include "common.h"
#include "glad/gl.h"

class Shader {
   private:
    std::string m_src;
    unsigned int compileShader(unsigned int type, std::string source);

   public:
    unsigned int m_rendererId;
    Shader(const std::string &vertexPath, const std::string &fragPath);
    Shader();

    void Bind();

    void uniform1i(const std::string &name, int val);
    void uniform2i(const std::string &name, int val1, int val2);

    void uniform2f(const std::string &name, float val1, float val2);
    void uniform3f(const std::string &name, float val1, float val2, float val3);

    void uniform1ui(const std::string &name, unsigned int val);
};
