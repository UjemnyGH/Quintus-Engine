#pragma once
#ifndef __C_ENGINE_FUNCTIONS_
#define __C_ENGINE_FUNCTIONS_

#include "../../vendor/glad/glad.h"
#include <iostream>
#include <fstream>

namespace qe
{
    typedef const char* shader_t;

    enum ShaderType {
        vertex = GL_VERTEX_SHADER,
        fragment = GL_FRAGMENT_SHADER,
        geometry = GL_GEOMETRY_SHADER,
        compute = GL_COMPUTE_SHADER,
        tess_eva = GL_TESS_EVALUATION_SHADER,
        tess_ctrl = GL_TESS_CONTROL_SHADER,
    };

    void qe_term(std::string msg) {
        std::cerr << msg << std::endl;

        exit(msg.size());
    }

    void qe_warn(std::string msg) {
        std::cerr << msg << std::endl;
    }

    uint32_t LoadShaderFromPath(std::string path, ShaderType shader_type) {
        std::ifstream fin(path, std::ios::binary | std::ios::ate);

        if(fin.bad()) {
            qe_warn("Bad file " + path);
        }

        if(!fin.is_open()) {
            qe_warn("File not open: " + path);
        }

        uint32_t lenght = static_cast<uint32_t>(fin.tellg());
        fin.seekg(std::ios::beg);

        char* src_buffer = new char[(lenght + 1) * sizeof(char)];
        fin.read(src_buffer, lenght);
        src_buffer[lenght] = '\0';
        fin.close();

        uint32_t shader = glCreateShader(shader_type);

        glShaderSource(shader, 1, &src_buffer, nullptr);

        delete[] src_buffer;

        glCompileShader(shader);

        return shader;
    }

    uint32_t LoadShader(std::string path, ShaderType shader_type) {
        return LoadShaderFromPath(path, shader_type);
    }

    uint32_t LoadShader(shader_t shader, ShaderType shader_type) {
        uint32_t sh = glCreateShader(shader_type);

        glShaderSource(sh, 1, &shader, nullptr);

        glCompileShader(sh);

        return sh;
    }
}

#endif