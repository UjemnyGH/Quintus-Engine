#pragma once
#ifndef __C_BUFFERS_
#define __C_BUFFERS_

#define GLEW_STATIC
#include <GL/glew.h>
#include <cstdint>
#include <vector>
#include <fstream>

namespace qe {
    struct QE_Vao {
        uint32_t m_id;
        bool created = false;

        /**
         * @brief Create vertex array object (called in Bind())
         * 
         */
        void Init() {
            if(!created) {
                created = true;
                
                glGenVertexArrays(1, &m_id);
            }
        }

        /**
         * @brief Bind vertex array object and also create if not created
         * 
         */
        void Bind() {
            Init();

            glBindVertexArray(m_id);
        }

        /**
         * @brief Unbind vertex array object
         * 
         */
        void Unbind() {
            glBindVertexArray(0);
        }

        /**
         * @brief Destroy the vao object
         * 
         */
        ~QE_Vao() {
            created = false;
            glDeleteVertexArrays(1, &m_id);
        }
    };

    struct QE_Vbo {
        uint32_t m_id;
        bool created = false;

        /**
         * @brief Create buffer (called in Bind())
         * 
         */
        void Init() {
            if(!created) {
                created = true;

                glGenBuffers(1, &m_id);
            }
        }

        /**
         * @brief Bind buffer to GL_ARRAY_BUFFER
         * 
         */
        void Bind() {
            Init();

            glBindBuffer(GL_ARRAY_BUFFER, m_id);
        }

        /**
         * @brief Bind and set new values to buffer
         * 
         * @param data data in float
         * @param index index inf shader
         * @param dimmension size (1 = float, 2 = vec2, 3 = vec3, 4 = vec4) of data
         */
        void Bind(std::vector<float> data, uint32_t index, uint32_t dimmension = 3) {
            Init();

            glBindBuffer(GL_ARRAY_BUFFER, m_id);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_DYNAMIC_DRAW);

            glVertexAttribPointer(index, dimmension, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(index);
        }

        /**
         * @brief Destroy the vbo object
         * 
         */
        ~QE_Vbo() {
            created = false;
            glDeleteBuffers(1, &m_id);
        }
    };

    struct QE_Ebo {
        uint32_t m_id;
        bool created = false;

        /**
         * @brief Create indices buffer (called in Bind())
         * 
         */
        void Init() {
            if(!created) {
                created = true;

                glGenBuffers(1, &m_id);
            }
        }

        /**
         * @brief Bind indices buffer to GL_ARRAY_BUFFER
         * 
         */
        void Bind() {
            Init();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        }

        /**
         * @brief Bind and set new values to indices buffer
         * 
         * @param data data in unsigned int
         */
        void Bind(std::vector<uint32_t> data) {
            Init();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_DYNAMIC_DRAW);
        }

        /**
         * @brief Destroy the ebo object
         * 
         */
        ~QE_Ebo() {
            created = false;
            glDeleteBuffers(1, &m_id);
        }
    };

    struct QE_Shader {
        uint32_t m_id;
        bool created = false;

        /**
         * @brief Create shader program
         * 
         */
        void Init() {
            if(!created) {
                created = true;

                m_id = glCreateProgram();
            }
        }

        /**
         * @brief Attach shader to program
         * 
         * @param shader_id 
         */
        void AttachShader(uint32_t shader_id) {
            Init();
            glAttachShader(m_id, shader_id);
        }

        /**
         * @brief Link program
         * 
         */
        void LinkProgram() {
            Init();
            glLinkProgram(m_id);
        }

        /**
         * @brief Use program as current shader program
         * 
         */
        void Bind() {
            Init();

            glUseProgram(m_id);
        }

        /**
         * @brief Unuse program
         * 
         */
        void Unbind() {
            glUseProgram(0);
        }

        /**
         * @brief Destroy the shader object
         * 
         */
        ~QE_Shader() {
            created = false;
            glDeleteProgram(m_id);
        }
    };

    typedef QE_Vao Vao;
    typedef QE_Vbo Vbo;
    typedef QE_Ebo Ebo;
    typedef QE_Shader Shader;
}

/*<?php
session_start();

$database = mysqli_connect("localhost", "root", "", "samochody");

$mk = $_POST['marka'];
$md = $_POST['model'];
$rk = $_POST['rocznik'];
$pj = $_POST['pojemnosc'];

if($mk != NULL && $md != NULL && $rk != NULL && $pj != NULL) {
    $result = mysqli_query($database, "INSERT INTO `spis`(`id`, `marka`, `model`, `rocznik`, `pojemnosc`) VALUES (NULL, '$mk', '$md', '$rk', '$pj');");
}
else {

}

mysqli_close($database);

session_abort();
?>*/

/*<?php
$database = mysqli_connect("localhost", "root", "", "samochody");

$result = mysqli_query($database, "SELECT * FROM spis;");

while($row = $result->fetch_row()) {
    printf("<tr><th>%s</th><th>%s</th><th>%s</th><th>%s</th><th>%s</th></tr>", $row[0], $row[1], $row[3], $row[3], $row[4]);
}

mysqli_close($database);
?>*/

#endif