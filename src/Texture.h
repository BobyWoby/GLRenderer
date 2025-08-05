#pragma once
#include <glad/gl.h>
#include <string>

class Texture{
private:
    unsigned int rendererID;
    std::string m_filpath;
    unsigned char* m_localBuffer;
    int m_width, m_height, m_bpp;

public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(uint slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const {return m_width;}
    inline int GetHeight() const {return m_height;}
};
