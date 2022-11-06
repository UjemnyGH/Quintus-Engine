#pragma once
#ifndef __R_RENDERER_
#define __R_RENDERER_

#include "../Core/c_layers.hpp"
#include "../Core/c_buffers.hpp"
#include "r_renderer_helpers.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <array>

namespace qe
{
    glm::mat4 m_projection = glm::mat4(1.0f);
    glm::mat4 m_view = glm::mat4(1.0f);

    class QE_Renderer : public Layer {
    private:
        Vao m_vao;
        std::vector<Vbo> m_vbos;
        std::array<Texture, 32> m_textures;
        Shader m_sh;
        Ebo m_ebo;
        std::vector<uint32_t> m_shaders;
        uint32_t m_textures_amount = 0;
        std::vector<uint32_t> m_textures_priority_queue;

        Rendered m_rendered;
        AxisHelper m_renderer_axis_helper;

        glm::mat4 m_model = glm::mat4(1.0f);

    public:
        bool m_render = true;
        bool m_with_indices = false;
        bool m_triangles = true;

        QE_Renderer() { m_auto_start = true; m_layer_debug_name = "Renderer"; }

        /**
         * @brief Restart (reattach) all shaders 
         * 
         */
        void ReattachShaders() {
            m_sh.Unbind();
            m_sh.~QE_Shader();

            for(uint32_t shader : m_shaders) {
                m_sh.AttachShader(shader);
            }

            m_sh.LinkProgram();
        }

        /**
         * @brief Add shader id to shaders heap
         * 
         * @param shader_id 
         */
        void AddShader(uint32_t shader_id) {
            m_shaders.push_back(shader_id);

            ReattachShaders();
        }

        /**
         * @brief Clear all shaders from heap
         * 
         */
        void ClearShaders() {
            m_shaders.clear();

            ReattachShaders();
        }

        /**
         * @brief Called on attaching layer
         * 
         */
        virtual void Start() override {
            ClearShaders();

            for(auto tex : m_textures) {
                tex.Init();
            }

            m_vao.Bind();

            m_sh.Bind();

            m_textures[0].Uniform(m_sh.m_id, "Textures");

            m_vbos.resize(5);
            
            m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
            m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
            m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
            m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
            m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

            m_vao.Unbind();
            m_sh.Unbind();
        }

        /**
         * @brief Add texture to textures heap
         * 
         * @param name 
         */
        void AddTexture(const std::string name) {
            if(m_textures_amount < 32 && m_textures_priority_queue.size() == 0) {
                m_textures[m_textures_amount].Bind(name, GL_REPEAT);
                m_textures_amount++;
            }
            else if(m_textures_priority_queue.size() > 0) {
                m_textures[m_textures_priority_queue[0]].Bind(name, GL_REPEAT);

                m_textures_priority_queue.erase(m_textures_priority_queue.begin());
                m_textures_amount++;
            }
            else {
                m_textures[31].Bind(name, GL_REPEAT);
            }
        }

        /**
         * @brief Remove texture from texture heap by id
         * 
         * @param id 
         */
        void RemoveTexture(uint32_t id) {
            m_textures_priority_queue.push_back(id);
            m_textures[id].~QE_Texture();
            m_textures[id].Init();
            m_textures_amount--;
        }

        /**
         * @brief Update (render) renderer stuff
         * 
         */
        virtual void Update() override {
            if(m_render) {
                m_sh.Bind();
                m_vao.Bind();

                m_textures[0].Bind(0);
                m_textures[1].Bind(1);
                m_textures[2].Bind(2);
                m_textures[3].Bind(3);
                m_textures[4].Bind(4);
                m_textures[5].Bind(5);
                m_textures[6].Bind(6);
                m_textures[7].Bind(7);
                m_textures[8].Bind(8);
                m_textures[9].Bind(9);
                m_textures[10].Bind(10);
                m_textures[11].Bind(11);
                m_textures[12].Bind(12);
                m_textures[13].Bind(13);
                m_textures[14].Bind(14);
                m_textures[15].Bind(15);
                m_textures[16].Bind(16);
                m_textures[17].Bind(17);
                m_textures[18].Bind(18);
                m_textures[19].Bind(19);
                m_textures[20].Bind(20);
                m_textures[21].Bind(21);
                m_textures[22].Bind(22);
                m_textures[23].Bind(23);
                m_textures[24].Bind(24);
                m_textures[25].Bind(25);
                m_textures[26].Bind(26);
                m_textures[27].Bind(27);
                m_textures[28].Bind(28);
                m_textures[29].Bind(29);
                m_textures[30].Bind(30);
                m_textures[31].Bind(31);

                glUniformMatrix4fv(glGetUniformLocation(m_sh.m_id, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
                glUniformMatrix4fv(glGetUniformLocation(m_sh.m_id, "model"), 1, GL_FALSE, glm::value_ptr(m_model));
                glUniformMatrix4fv(glGetUniformLocation(m_sh.m_id, "view"), 1, GL_FALSE, glm::value_ptr(m_view));

                if(!m_with_indices & m_triangles) {
                    glDrawArrays(GL_TRIANGLES, 0, m_rendered.m_joined_data.m_vertices.size() * 2);
                }
                else if(m_with_indices & m_triangles) {
                    glDrawElements(GL_TRIANGLES, m_rendered.m_joined_data.m_vertices.size() * 2, GL_UNSIGNED_INT, nullptr);
                }
                else if(!m_with_indices & !m_triangles) {
                    glDrawArrays(GL_LINES, 0, m_rendered.m_joined_data.m_vertices.size() * 2);
                }
                else if(m_with_indices & !m_triangles) {
                    glDrawElements(GL_LINES, m_rendered.m_joined_data.m_vertices.size() * 2, GL_UNSIGNED_INT, nullptr);
                }
            
                m_vao.Unbind();
                m_sh.Unbind();
            }
        }

        /**
         * @brief Set the Position object
         * 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetPosition(float x, float y, float z) {
            m_renderer_axis_helper.position[0] = x;
            m_renderer_axis_helper.position[1] = y;
            m_renderer_axis_helper.position[2] = z;

            m_model = glm::translate(glm::mat4(1.0f), glm::vec3(m_renderer_axis_helper.position[0], m_renderer_axis_helper.position[1], m_renderer_axis_helper.position[2]));
            m_model = glm::rotate(m_model, glm::radians(m_renderer_axis_helper.rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(m_renderer_axis_helper.rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(m_renderer_axis_helper.rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
            m_model = glm::scale(m_model, glm::vec3(m_renderer_axis_helper.scale[0], m_renderer_axis_helper.scale[1], m_renderer_axis_helper.scale[2]));
        }

        /**
         * @brief Set the Scale object
         * 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetScale(float x, float y, float z) {
            m_renderer_axis_helper.scale[0] = x;
            m_renderer_axis_helper.scale[1] = y;
            m_renderer_axis_helper.scale[2] = z;

            m_model = glm::translate(glm::mat4(1.0f), glm::vec3(m_renderer_axis_helper.position[0], m_renderer_axis_helper.position[1], m_renderer_axis_helper.position[2]));
            m_model = glm::rotate(m_model, glm::radians(m_renderer_axis_helper.rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(m_renderer_axis_helper.rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(m_renderer_axis_helper.rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
            m_model = glm::scale(m_model, glm::vec3(m_renderer_axis_helper.scale[0], m_renderer_axis_helper.scale[1], m_renderer_axis_helper.scale[2]));
        }

        /**
         * @brief Set the Rotation object
         * 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetRotation(float x, float y, float z) {
            m_renderer_axis_helper.rotation[0] = x;
            m_renderer_axis_helper.rotation[1] = y;
            m_renderer_axis_helper.rotation[2] = z;

            m_model = glm::translate(glm::mat4(1.0f), glm::vec3(m_renderer_axis_helper.position[0], m_renderer_axis_helper.position[1], m_renderer_axis_helper.position[2]));
            m_model = glm::rotate(m_model, glm::radians(m_renderer_axis_helper.rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(m_renderer_axis_helper.rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(m_renderer_axis_helper.rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
            m_model = glm::scale(m_model, glm::vec3(m_renderer_axis_helper.scale[0], m_renderer_axis_helper.scale[1], m_renderer_axis_helper.scale[2]));
        }

        /**
         * @brief Set the Position By ID
         * 
         * @param id 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetPositionByID(uint32_t id, float x, float y, float z) {
            if( m_rendered.m_axis_helper[id].position[0] != x ||
                m_rendered.m_axis_helper[id].position[1] != y ||
                m_rendered.m_axis_helper[id].position[2] != z) {

                m_rendered.m_axis_helper[id].position[0] = x;
                m_rendered.m_axis_helper[id].position[1] = y;
                m_rendered.m_axis_helper[id].position[2] = z;

                for(size_t i = 0; i < m_rendered.m_data_sizes[id]; i++) {
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 0] * m_rendered.m_axis_helper[id].scale[0] + m_rendered.m_axis_helper[id].position[0];
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 1] * m_rendered.m_axis_helper[id].scale[1] + m_rendered.m_axis_helper[id].position[1];
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 2] * m_rendered.m_axis_helper[id].scale[2] + m_rendered.m_axis_helper[id].position[2];
                
                    float h_x = 0.0f;
                    float h_y = 0.0f;
                    float h_z = 0.0f;

                    h_y = m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)];
                    h_z = m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_y - sin(m_rendered.m_axis_helper[id].rotation[0]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[0]) * h_y);

                    h_x = m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)];
                    h_z = m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[1]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[1]) * h_x);

                    h_x = m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)];
                    h_y = m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[2]) * h_y);
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_y + sin(m_rendered.m_axis_helper[id].rotation[2]) * h_x);
                }

                m_vao.Bind();

                m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);

                m_vao.Unbind();
            }
        }

        /**
         * @brief Set the Scale By ID
         * 
         * @param id 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetScaleByID(uint32_t id, float x, float y, float z) {
            if( m_rendered.m_axis_helper[id].scale[0] != x ||
                m_rendered.m_axis_helper[id].scale[1] != y ||
                m_rendered.m_axis_helper[id].scale[2] != z) {

                m_rendered.m_axis_helper[id].scale[0] = x;
                m_rendered.m_axis_helper[id].scale[1] = y;
                m_rendered.m_axis_helper[id].scale[2] = z;

                for(size_t i = 0; i < m_rendered.m_data_sizes[id]; i++) {
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 0] * m_rendered.m_axis_helper[id].scale[0] + m_rendered.m_axis_helper[id].position[0];
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 1] * m_rendered.m_axis_helper[id].scale[1] + m_rendered.m_axis_helper[id].position[1];
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 2] * m_rendered.m_axis_helper[id].scale[2] + m_rendered.m_axis_helper[id].position[2];
                
                    float h_x = 0.0f;
                    float h_y = 0.0f;
                    float h_z = 0.0f;

                    h_y = m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)];
                    h_z = m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_y - sin(m_rendered.m_axis_helper[id].rotation[0]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[0]) * h_y);

                    h_x = m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)];
                    h_z = m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[1]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[1]) * h_x);

                    h_x = m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)];
                    h_y = m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[2]) * h_y);
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_y + sin(m_rendered.m_axis_helper[id].rotation[2]) * h_x);
                }

                m_vao.Bind();

                m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);

                m_vao.Unbind();
            }
        }

        /**
         * @brief Set the Rotation By ID
         * 
         * @param id 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetRotationByID(uint32_t id, float x, float y, float z) {
            if( m_rendered.m_axis_helper[id].rotation[0] != x ||
                m_rendered.m_axis_helper[id].rotation[1] != y ||
                m_rendered.m_axis_helper[id].rotation[2] != z) {

                m_rendered.m_axis_helper[id].rotation[0] = x;
                m_rendered.m_axis_helper[id].rotation[1] = y;
                m_rendered.m_axis_helper[id].rotation[2] = z;

                for(size_t i = 0; i < m_rendered.m_data_sizes[id]; i++) {
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 0] * m_rendered.m_axis_helper[id].scale[0] + m_rendered.m_axis_helper[id].position[0];
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 1] * m_rendered.m_axis_helper[id].scale[1] + m_rendered.m_axis_helper[id].position[1];
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 2] * m_rendered.m_axis_helper[id].scale[2] + m_rendered.m_axis_helper[id].position[2];
                
                    float h_x = 0.0f;
                    float h_y = 0.0f;
                    float h_z = 0.0f;

                    h_y = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)];
                    h_z = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_y - sin(m_rendered.m_axis_helper[id].rotation[0]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[0]) * h_y);

                    h_x = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)];
                    h_z = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[1]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[1]) * h_x);

                    h_x = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)];
                    h_y = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[2]) * h_y);
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_y + sin(m_rendered.m_axis_helper[id].rotation[2]) * h_x);
                }

                m_vao.Bind();

                m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);

                m_vao.Unbind();
            }
        }

        /**
         * @brief Set the Color By ID
         * 
         * @param id 
         * @param r 
         * @param g 
         * @param b 
         * @param a 
         */
        void SetColorByID(uint32_t id, float r, float g, float b, float a) {
            if( m_rendered.m_data[id].m_color[0] != r || 
                m_rendered.m_data[id].m_color[1] != g || 
                m_rendered.m_data[id].m_color[2] != b || 
                m_rendered.m_data[id].m_color[3] != a) {

                for(int i = 0; i < m_rendered.m_data_sizes[id]; i++) {
                    m_rendered.m_data[id].m_color[i * 4 + 0] = m_rendered.m_joined_data.m_color[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 4 + 0)] = r;
                    m_rendered.m_data[id].m_color[i * 4 + 1] = m_rendered.m_joined_data.m_color[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 4 + 1)] = g;
                    m_rendered.m_data[id].m_color[i * 4 + 2] = m_rendered.m_joined_data.m_color[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 4 + 2)] = b;
                    m_rendered.m_data[id].m_color[i * 4 + 3] = m_rendered.m_joined_data.m_color[(m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + (i * 4 + 3)] = a;
                }

                m_vao.Bind();

                m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);

                m_vao.Unbind();
            }
        }

        /**
         * @brief Add model to heap
         * 
         * @param data 
         */
        void AddModel(RenderedData data) {
            m_rendered.PushData(data);

            m_vao.Bind();

            m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
            m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
            m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
            m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
            m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

            m_vao.Unbind();
        }

        /**
         * @brief Remove model from heap by data
         * 
         * @param data 
         */
        void RemoveModel(RenderedData data) {
            m_rendered.PopData(data);

            m_vao.Bind();

            m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
            m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
            m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
            m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
            m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

            m_vao.Unbind();
        }

        /**
         * @brief Remove model from heap by id
         * 
         * @param id 
         */
        void RemoveModel(uint32_t id) {
            m_rendered.PopData(id);
            
            m_vao.Bind();

            m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
            m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
            m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
            m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
            m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

            m_vao.Unbind();
        }

        /**
         * @brief Force buffers data to rejoin (may be needed in some cases to have working models)
         * 
         */
        void ForceRejoin() {
            m_rendered.JoinData();
            
            m_vao.Bind();

            m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
            m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
            m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
            m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
            m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

            m_vao.Unbind();
        }

        /**
         * @brief Set the Rendered object
         * 
         * @param data 
         */
        void SetRendered(Rendered data) {
            m_rendered = data;
        }

        /**
         * @brief Ends renderer
         * 
         */
        virtual void End() override {
            ClearShaders();

            m_sh.~QE_Shader();
            m_vao.~QE_Vao();
            for(Vbo vbo : m_vbos) {
                vbo.~QE_Vbo();
            }

            for(Texture tex : m_textures) {
                tex.~QE_Texture();
            }

            m_ebo.~QE_Ebo();
        }
    };

    typedef QE_Renderer Renderer;
} // namespace qe


#endif