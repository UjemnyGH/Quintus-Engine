#pragma once
#ifndef __C_BUFFERS_
#define __C_BUFFERS_

#include "../../vendor/glad/glad.h"
#include <cstdint>
#include <vector>
#include <fstream>
#include "../../vendor/stb/stb_image.h"

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

    struct QE_Texture {
        uint32_t m_id;
        bool created = false;

		int width, height, nrChannels;
        unsigned char* m_data;

		int samplers[32] = {
		0,      1,      2,      3,      4,      5,      6,      7,      8,      9,      10,     11,     12,     13,     14,     15,
		16,     17,     18,     19,     20,     21,     22,     23,     24,     25,     26,     27,     28,     29,     30,     31
		};

        void Init() {
            if(!created) {
                created = true;
                glGenTextures(1, &m_id);
            }
        }

		/**
		 * @brief Bind texture
		 * 
		 */
		void Bind() {
            Init();

			glBindTexture(GL_TEXTURE_2D, m_id);
		}

		/**
		 * @brief Bind texture by ID(0-31)
		 * 
		 * @param _id 
		 */
		void Bind(const unsigned int _id) {
            Init();

			glBindTextureUnit(_id, m_id);
		}

		/**
		 * @brief Bind texture and set data
		 * 
		 * @param name name of .png file(only that is handled right now)
		 * @param wrapping GL_REPEAT, GL_CLAMP_TO_EDGE...
		 * @param pixelized true = pixelized art(good for pixel arts)
		 */
		void Bind(const std::string& name, const unsigned int& wrapping, bool pixelized = false) {
            Init();

			glBindTexture(GL_TEXTURE_2D, m_id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
			if (!pixelized)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else if (pixelized)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}

			stbi_set_flip_vertically_on_load(1);

			m_data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
			if (m_data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				printf("Error loading image: %s\n", name.c_str());
			}

			stbi_image_free(m_data);
		}

		/**
		 * @brief Sets shader uniform by name
		 * 
		 * @param program Shader program
		 * @param place how uniform in shader has named
		 */
		void Uniform(const unsigned int program, const std::string place) {
			glUseProgram(program);

			glUniform1iv(glGetUniformLocation(program, place.c_str()), 32, samplers);
			
			glUseProgram(0);
		}

		void Uniform(const unsigned int program, const std::string place, unsigned int number) {
			glUseProgram(program);

			glUniform1i(glGetUniformLocation(program, place.c_str()), number);

			glUseProgram(0);
		}

		~QE_Texture() {
			glDeleteTextures(1, &m_id);
		}
    };

    typedef QE_Vao Vao;
    typedef QE_Vbo Vbo;
    typedef QE_Ebo Ebo;
    typedef QE_Shader Shader;
    typedef QE_Texture Texture;
}

#endif