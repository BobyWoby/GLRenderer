#pragma once

#include <glad/gl.h>
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define ASSERT(x, msg) assert((void(msg), x));

inline void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id,
                                     GLenum severity, GLsizei length,
                                     const GLchar* message,
                                     const void* userParam) {
    std::string _source;
    std::string _type;
    std::string _severity;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_OTHER:
            _source = "OTHER";
        default:
            _source = "UNKNOWN";
    }
    switch (type) {
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UNDEFINED BEHAVIOR";
        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMACE";
        case GL_DEBUG_TYPE_POP_GROUP:
            _type = "POP GROUP";
        case GL_DEBUG_TYPE_PUSH_GROUP:
            _type = "PUSH GROUP";
        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
        default:
            _type = "UNKNOWN";
    }
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION";
        default:
            _severity = "UNKNOWN";
    }

    printf("%d: %s of %s severity, raised from %s: %s\n", id, _type.c_str(),
           _severity.c_str(), _source.c_str(), message);
}
