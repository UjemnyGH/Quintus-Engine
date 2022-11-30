#pragma once
#ifndef __C_ENGINE_FUNCTIONS_
#define __C_ENGINE_FUNCTIONS_

#include "../../vendor/glad/glad.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <bit>

typedef unsigned char byte_t;

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

    /**
     * @brief Terminate program with message (message length is a exit code!)
     * 
     * @param msg 
     */
    void qe_term(std::string msg) {
        std::cerr << "Termination: " << msg << std::endl;

        exit(msg.size());
    }

    /**
     * @brief Show warn
     * 
     * @param msg 
     */
    void qe_warn(std::string msg) {
        std::cerr << "Warning: " << msg << std::endl;
    }

    /**
     * @brief Load binary file and convert values in it to floats in little endian 
     * 
     * @param filename 
     * @return std::vector<float> 
     */
    std::vector<float> load_bin_file_to_vector_float(std::string const filename) {
        std::vector<float> result;
        std::vector<byte_t> bytes;

        FILE *file = fopen(filename.c_str(), "rb");

        fseek(file, 0, SEEK_END);

        bytes.resize(ftell(file));

        fseek(file, 0, SEEK_SET);

        fread(bytes.data(), sizeof(byte_t), bytes.size(), file);

        fclose(file);

        byte_t byte_section[4];
        uint32_t byte_counter = 0;

        for(byte_t byte : bytes) {
            byte_section[3 - byte_counter] = byte;

            byte_counter++;

            if(byte_counter > 3) {
                byte_counter = 0;

                uint32_t flt_to_convert = (uint32_t)(byte_section[0] << 24) + (uint32_t)(byte_section[1] << 16) + (uint32_t)(byte_section[2] << 8) + (uint32_t)(byte_section[3]);

                result.push_back(std::bit_cast<float>(flt_to_convert));
            }
        }

        return result;
    }

    /**
     * @brief Load vector of binary values and convert it into floats little endian
     * 
     * @param bytes 
     * @return std::vector<float> 
     */
    std::vector<float> load_bin_vector_byte_t_to_vector_float(std::vector<byte_t> const bytes) {
        std::vector<float> result;

        byte_t byte_section[4];
        uint32_t byte_counter = 0;

        for(byte_t byte : bytes) {
            byte_section[3 - byte_counter] = byte;

            byte_counter++;

            if(byte_counter > 3) {
                byte_counter = 0;

                uint32_t flt_to_convert = (uint32_t)(byte_section[0] << 24) + (uint32_t)(byte_section[1] << 16) + (uint32_t)(byte_section[2] << 8) + (uint32_t)(byte_section[3]);

                result.push_back(std::bit_cast<float>(flt_to_convert));
            }
        }

        return result;
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

    /**
     * @brief Load shader from path
     * 
     * @param path 
     * @param shader_type 
     * @return uint32_t 
     */
    uint32_t LoadShader(std::string path, ShaderType shader_type) {
        return LoadShaderFromPath(path, shader_type);
    }

    /**
     * @brief Load shader from code
     * 
     * @param shader 
     * @param shader_type 
     * @return uint32_t 
     */
    uint32_t LoadShader(shader_t shader, ShaderType shader_type) {
        uint32_t sh = glCreateShader(shader_type);

        glShaderSource(sh, 1, &shader, nullptr);

        glCompileShader(sh);

        return sh;
    }
}

#endif