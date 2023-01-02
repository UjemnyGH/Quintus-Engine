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
    glm::mat4 g_projection = glm::mat4(1.0f);
    glm::mat4 g_view = glm::mat4(1.0f);
    glm::vec3 g_globalLight = glm::vec3(1.0f, 1.0f, 1.0f);

    class QE_Renderer : public Layer {
    private:
        Vao m_vao;
        std::vector<Vbo> m_vbos;
        std::array<Texture, 32> m_textures;
        Shader m_sh;
        Ebo m_ebo;
        std::vector<uint32_t> m_shaders;
        uint32_t m_textures_amount = 1;
        std::vector<uint32_t> m_textures_priority_queue;

        Rendered m_rendered;
        AxisHelper m_renderer_axis_helper;

        glm::mat4 m_model = glm::mat4(1.0f);


        bool m_buffer_needs_rebind = false;

    public:
        bool m_render = true;
        bool m_with_indices = false;
        bool m_triangles = true;
        bool m_renderer_created = false;
        bool m_render_pixels = false;
        bool m_render_with_g_projection = true;
        bool m_render_with_g_view = true;

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

            m_vao.Bind();

            m_sh.Bind();

            for(int i = 0; i < 32; i++) {
                m_textures[i].Init();
            }

            m_textures[0].Uniform(m_sh.m_id, "Textures");

            m_vbos.resize(5);
            
            m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
            m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
            m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
            m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
            m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

            m_vao.Unbind();
            m_sh.Unbind();

            m_renderer_created = true;
        }

        // FIXME: Adding more than 1 texture can be buggy
        /**
         * @brief Add texture to textures heap
         * 
         * @param name 
         */
        void AddTexture(const std::string name) {
            //printf("Texxure amount: %d, Priority %d, Name %s\n", m_textures_amount, m_textures_priority_queue.size(), name.c_str());
            //fflush(stdout);
            m_sh.Bind();
            m_vao.Bind();

            if(m_textures_amount < 32 && m_textures_priority_queue.size() < 1) {
                m_textures[m_textures_amount].Bind(name, GL_REPEAT, m_render_pixels);
                m_textures_amount++;
            }
            else if(m_textures_priority_queue.size() > 0) {
                m_textures[m_textures_priority_queue[0]].Bind(name, GL_REPEAT, m_render_pixels);

                m_textures_priority_queue.erase(m_textures_priority_queue.begin());
                m_textures_amount++;
            }
            else {
                m_textures[31].Bind(name, GL_REPEAT, m_render_pixels);
            }

            m_vao.Unbind();
            m_sh.Unbind();
        }

        void AddTextureWithForcedID(const std::string name, uint32_t id) {
            m_sh.Bind();
            m_vao.Bind();

            m_textures[id].Bind(name, GL_REPEAT, m_render_pixels);

            m_vao.Unbind();
            m_sh.Unbind();
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
         * @brief Set the Model Texture object
         * 
         * @param model_id 
         * @param texture_id 
         */
        void SetModelTexture(uint32_t model_id, uint32_t texture_id) {
            //printf("Model end: %d Model size: %d Textture id: %d\n",  m_rendered.m_data_end[model_id], m_rendered.m_data_sizes[model_id], texture_id);
            //fflush(stdout);

            for(int i = m_rendered.m_data_end[model_id] - m_rendered.m_data_sizes[model_id]; i < m_rendered.m_data_end[model_id]; i++) {
                m_rendered.m_texture_index[i] = static_cast<float>(texture_id);
                //printf("Texture: %f set to texture index %d times\n", static_cast<float>(texture_id), i);
                //fflush(stdout);
            }

            m_vao.Bind();
            
            m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

            m_vao.Unbind();
        }

        /**
         * @brief Update (render) renderer stuff
         * 
         */
        virtual void Update() override {
            if(m_render) {
                m_sh.Bind();
                m_vao.Bind();

                for(int i = 1; i < 32; i++) {
                    m_textures[i].Bind(i);
                }

                if(m_render_with_g_projection) {
                    glUniformMatrix4fv(glGetUniformLocation(m_sh.m_id, "projection"), 1, GL_FALSE, glm::value_ptr(g_projection));
                }
                else {
                    glUniformMatrix4fv(glGetUniformLocation(m_sh.m_id, "projection"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
                }

                if(m_render_with_g_view) {
                    glUniformMatrix4fv(glGetUniformLocation(m_sh.m_id, "view"), 1, GL_FALSE, glm::value_ptr(g_view));
                }
                else {
                    glUniformMatrix4fv(glGetUniformLocation(m_sh.m_id, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
                }

                glUniformMatrix4fv(glGetUniformLocation(m_sh.m_id, "model"), 1, GL_FALSE, glm::value_ptr(m_model));
                glUniform3f(glGetUniformLocation(m_sh.m_id, "globalLight"), g_globalLight.x, g_globalLight.y, g_globalLight.z);

                if(!m_with_indices & m_triangles) {
                    glDrawArrays(GL_TRIANGLES, 0, m_rendered.m_joined_data.m_vertices.size() / 3);
                }
                else if(m_with_indices & m_triangles) {
                    glDrawElements(GL_TRIANGLES, m_rendered.m_joined_data.m_vertices.size() / 3, GL_UNSIGNED_INT, nullptr);
                }
                else if(!m_with_indices & !m_triangles) {
                    glDrawArrays(GL_LINES, 0, m_rendered.m_joined_data.m_vertices.size() / 3);
                }
                else if(m_with_indices & !m_triangles) {
                    glDrawElements(GL_LINES, m_rendered.m_joined_data.m_vertices.size() / 3, GL_UNSIGNED_INT, nullptr);
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
            m_model = glm::rotate(m_model, m_renderer_axis_helper.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
            m_model = glm::rotate(m_model, m_renderer_axis_helper.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
            m_model = glm::rotate(m_model, m_renderer_axis_helper.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
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
            m_model = glm::rotate(m_model, m_renderer_axis_helper.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
            m_model = glm::rotate(m_model, m_renderer_axis_helper.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
            m_model = glm::rotate(m_model, m_renderer_axis_helper.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
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
            m_model = glm::rotate(m_model, m_renderer_axis_helper.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
            m_model = glm::rotate(m_model, m_renderer_axis_helper.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
            m_model = glm::rotate(m_model, m_renderer_axis_helper.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
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
        void SetPositionByID(uint32_t id, float x, float y, float z, bool bind_to_buffer = true) {
            if( m_rendered.m_axis_helper[id].position[0] != x ||
                m_rendered.m_axis_helper[id].position[1] != y ||
                m_rendered.m_axis_helper[id].position[2] != z) {

                m_rendered.m_axis_helper[id].position[0] = x;
                m_rendered.m_axis_helper[id].position[1] = y;
                m_rendered.m_axis_helper[id].position[2] = z;

                for(size_t i = 0; i < m_rendered.m_data_sizes[id]; i++) {
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 0] * m_rendered.m_axis_helper[id].scale[0] + m_rendered.m_axis_helper[id].position[0];
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 1] * m_rendered.m_axis_helper[id].scale[1] + m_rendered.m_axis_helper[id].position[1];
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 2] * m_rendered.m_axis_helper[id].scale[2] + m_rendered.m_axis_helper[id].position[2];
                
                    float h_x = 0.0f;
                    float h_y = 0.0f;
                    float h_z = 0.0f;

                    h_y = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)];
                    h_z = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_y - sin(m_rendered.m_axis_helper[id].rotation[0]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[0]) * h_y);

                    h_x = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)];
                    h_z = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[1]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[1]) * h_x);

                    h_x = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)];
                    h_y = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[2]) * h_y);
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_y + sin(m_rendered.m_axis_helper[id].rotation[2]) * h_x);
                }

                if(bind_to_buffer) {
                    m_vao.Bind();

                    m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
                    
                    m_vao.Unbind();
                }
                else {
                    m_buffer_needs_rebind = true;
                }
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
        void SetScaleByID(uint32_t id, float x, float y, float z, bool bind_to_buffer = true) {
            if( m_rendered.m_axis_helper[id].scale[0] != x ||
                m_rendered.m_axis_helper[id].scale[1] != y ||
                m_rendered.m_axis_helper[id].scale[2] != z) {

                m_rendered.m_axis_helper[id].scale[0] = x;
                m_rendered.m_axis_helper[id].scale[1] = y;
                m_rendered.m_axis_helper[id].scale[2] = z;

                for(size_t i = 0; i < m_rendered.m_data_sizes[id]; i++) {
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 0] * m_rendered.m_axis_helper[id].scale[0] + m_rendered.m_axis_helper[id].position[0];
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 1] * m_rendered.m_axis_helper[id].scale[1] + m_rendered.m_axis_helper[id].position[1];
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 2] * m_rendered.m_axis_helper[id].scale[2] + m_rendered.m_axis_helper[id].position[2];
                
                    float h_x = 0.0f;
                    float h_y = 0.0f;
                    float h_z = 0.0f;

                    h_y = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)];
                    h_z = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_y - sin(m_rendered.m_axis_helper[id].rotation[0]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[0]) * h_y);

                    h_x = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)];
                    h_z = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[1]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[1]) * h_x);

                    h_x = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)];
                    h_y = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[2]) * h_y);
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_y + sin(m_rendered.m_axis_helper[id].rotation[2]) * h_x);
                }

                if(bind_to_buffer) {
                    m_vao.Bind();

                    m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
                    
                    m_vao.Unbind();
                }
                else {
                    m_buffer_needs_rebind = true;
                }
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
        void SetRotationByID(uint32_t id, float x, float y, float z, bool bind_to_buffer = true) {
            if( m_rendered.m_axis_helper[id].rotation[0] != x ||
                m_rendered.m_axis_helper[id].rotation[1] != y ||
                m_rendered.m_axis_helper[id].rotation[2] != z) {

                m_rendered.m_axis_helper[id].rotation[0] = x;
                m_rendered.m_axis_helper[id].rotation[1] = y;
                m_rendered.m_axis_helper[id].rotation[2] = z;

                for(size_t i = 0; i < m_rendered.m_data_sizes[id]; i++) {
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 0] * m_rendered.m_axis_helper[id].scale[0] + m_rendered.m_axis_helper[id].position[0];
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 1] * m_rendered.m_axis_helper[id].scale[1] + m_rendered.m_axis_helper[id].position[1];
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)] = m_rendered.m_original_data[id].m_vertices[i * 3 + 2] * m_rendered.m_axis_helper[id].scale[2] + m_rendered.m_axis_helper[id].position[2];
                
                    float h_x = 0.0f;
                    float h_y = 0.0f;
                    float h_z = 0.0f;

                    h_y = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)];
                    h_z = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_y - sin(m_rendered.m_axis_helper[id].rotation[0]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[0]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[0]) * h_y);

                    h_x = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)];
                    h_z = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[1]) * h_z);
                    m_rendered.m_data[id].m_vertices[i * 3 + 2] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 2)] = (cos(m_rendered.m_axis_helper[id].rotation[1]) * h_z + sin(m_rendered.m_axis_helper[id].rotation[1]) * h_x);

                    h_x = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)];
                    h_y = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)];
                
                    m_rendered.m_data[id].m_vertices[i * 3 + 0] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 0)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_x - sin(m_rendered.m_axis_helper[id].rotation[2]) * h_y);
                    m_rendered.m_data[id].m_vertices[i * 3 + 1] = m_rendered.m_joined_data.m_vertices[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 3 + 1)] = (cos(m_rendered.m_axis_helper[id].rotation[2]) * h_y + sin(m_rendered.m_axis_helper[id].rotation[2]) * h_x);
                }

                if(bind_to_buffer) {
                    m_vao.Bind();

                    m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
                    
                    m_vao.Unbind();
                }
                else {
                    m_buffer_needs_rebind = true;
                }
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
        void SetColorByID(uint32_t id, float r, float g, float b, float a, bool bind_to_buffer = true) {
            if( m_rendered.m_data[id].m_color[0] != r || 
                m_rendered.m_data[id].m_color[1] != g || 
                m_rendered.m_data[id].m_color[2] != b || 
                m_rendered.m_data[id].m_color[3] != a) {

                for(int i = 0; i < m_rendered.m_data_sizes[id]; i++) {
                    m_rendered.m_data[id].m_color[i * 4 + 0] = m_rendered.m_joined_data.m_color[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 4 + 0)] = r;
                    m_rendered.m_data[id].m_color[i * 4 + 1] = m_rendered.m_joined_data.m_color[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 4 + 1)] = g;
                    m_rendered.m_data[id].m_color[i * 4 + 2] = m_rendered.m_joined_data.m_color[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 4 + 2)] = b;
                    m_rendered.m_data[id].m_color[i * 4 + 3] = m_rendered.m_joined_data.m_color[(((m_rendered.m_data_end[id] - m_rendered.m_data_sizes[id]) + i) * 4 + 3)] = a;
                }

                if(bind_to_buffer) {
                    m_vao.Bind();

                    m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);

                    m_vao.Unbind();
                }
                else {
                    m_buffer_needs_rebind = true;
                }
            }
        }

        /**
         * @brief Add model to heap
         * 
         * @param data 
         */
        void AddModel(RenderedData data, std::string model_name = "__Model__", bool bind_to_buffer = true) {
            m_rendered.PushData(data, model_name);

            if(bind_to_buffer) {
                m_vao.Bind();

                m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
                m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
                m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
                m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
                m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

                m_vao.Unbind();
            }
            else {
                m_buffer_needs_rebind = true;
            }
        }

        /**
         * @brief Remove model from heap by data
         * 
         * @param data 
         */
        void RemoveModel(RenderedData data, bool bind_to_buffer = true) {
            m_rendered.PopData(data);

            if(bind_to_buffer) {
                m_vao.Bind();

                m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
                m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
                m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
                m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
                m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

                m_vao.Unbind();
            }
            else {
                m_buffer_needs_rebind = true;
            }
        }

        /**
         * @brief Remove model from heap by id
         * 
         * @param id 
         */
        void RemoveModel(uint32_t id, bool bind_to_buffer = true) {
            m_rendered.PopData(id);
            
            if(bind_to_buffer) {
                m_vao.Bind();

                m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
                m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
                m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
                m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
                m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

                m_vao.Unbind();
            }
            else {
                m_buffer_needs_rebind = true;
            }
        }

        /**
         * @brief Remove model from heap by id
         * 
         * @param id 
         */
        void RemoveModel(std::string name, bool bind_to_buffer = true) {
            m_rendered.PopData(name);
            
            if(bind_to_buffer) {
                m_vao.Bind();

                m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
                m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
                m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
                m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
                m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

                m_vao.Unbind();
            }
            else {
                m_buffer_needs_rebind = true;
            }
        }

        /**
         * @brief Get the Id From Name
         * 
         * @return uint32_t 
         */
        uint32_t GetIdFromName(std::string name) {
            if(m_rendered.m_original_data.size() > 1) {
                return Search(m_rendered.m_model_name, name);
            }
            else {
                return 0;
            }
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
         * @brief Binds data to buffers passed to GPU, useful if you loading big models on separated thread
         * 
         * @param force_rebind 
         */
        void BindToBuffers(bool force_rebind = false) {
            if(m_buffer_needs_rebind || force_rebind) {
                m_buffer_needs_rebind = false;

                m_vao.Bind();

                m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
                m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
                m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
                m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
                m_vbos[4].Bind(m_rendered.m_texture_index, 4, 1);

                m_vao.Unbind();
            }
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
         * @brief Get the Rendered data pointer
         * 
         * @return Rendered* 
         */
        Rendered *GetRenderedPtr() { return &m_rendered; }

        /**
         * @brief Get the Model Position object
         * 
         * @param id model ID
         * @return glm::vec3 
         */
        math::Vector<float> GetModelPosition(uint32_t id) {
            return math::Vector<float>(m_rendered.m_axis_helper[id].position[0], m_rendered.m_axis_helper[id].position[1], m_rendered.m_axis_helper[id].position[2]);
        }

        /**
         * @brief Get the Model Scale object
         * 
         * @param id model ID
         * @return glm::vec3 
         */
        math::Vector<float> GetModelScale(uint32_t id) {
            return math::Vector<float>(m_rendered.m_axis_helper[id].scale[0], m_rendered.m_axis_helper[id].scale[1], m_rendered.m_axis_helper[id].scale[2]);
        }

        /**
         * @brief Get the Model Rotation object
         * 
         * @param id model ID
         * @return glm::vec3 
         */
        math::Vector<float> GetModelRotation(uint32_t id) {
            return math::Vector<float>(m_rendered.m_axis_helper[id].rotation[0], m_rendered.m_axis_helper[id].rotation[1], m_rendered.m_axis_helper[id].rotation[2]);
        }

        /**
         * @brief Get the Model Color object
         * 
         * @param id 
         * @return Vector<float> 
         */
        Vector<float> GetModelColor(uint32_t id) {
            return Vector<float>(m_rendered.m_data[id].m_color[0], m_rendered.m_data[id].m_color[1], m_rendered.m_data[id].m_color[2], m_rendered.m_data[id].m_color[3]);
        }

        /**
         * @brief Get the Model Amount object
         * 
         * @return uint32_t 
         */
        uint32_t GetModelAmount() { return m_rendered.m_data_sizes.size(); }

        /**
         * @brief Get the Shader ID
         * 
         * @return uint32_t 
         */
        uint32_t GetShader() { return m_sh.m_id; }

        /**
         * @brief Bind vertex array object
         * 
         */
        void BindVAO() { m_vao.Bind(); }

        /**
         * @brief Unbind vertex array object
         * 
         */
        void UnbindVAO() { m_vao.Unbind(); }

        void SetInt1(std::string name, int v0) {
            m_vao.Bind();

            glUniform1i(glGetUniformLocation(m_sh.m_id, name.c_str()), v0);

            m_vao.Unbind();
        }

        void SetFloat1(std::string name, float v0) {
            m_vao.Bind();

            glUniform1f(glGetUniformLocation(m_sh.m_id, name.c_str()), v0);

            m_vao.Unbind();
        }

        void SetFloat2(std::string name, float v0, float v1) {
            m_vao.Bind();

            glUniform2f(glGetUniformLocation(m_sh.m_id, name.c_str()), v0, v1);

            m_vao.Unbind();
        }

        void SetFloat3(std::string name, float v0, float v1, float v2) {
            m_vao.Bind();

            glUniform3f(glGetUniformLocation(m_sh.m_id, name.c_str()), v0, v1, v2);

            m_vao.Unbind();
        }

        void SetFloat4(std::string name, float v0, float v1, float v2, float v3) {
            m_vao.Bind();

            glUniform4f(glGetUniformLocation(m_sh.m_id, name.c_str()), v0, v1, v2, v3);

            m_vao.Unbind();
        }

        /**
         * @brief Debug info on public bool variables (m_render, m_with_indices, m_triangles)
         * 
         */
        void __debug_public_info() {
            printf("Render: %d, Triangles: %d, Indices: %d\n", m_render, m_triangles, m_with_indices);
        }

        void __debug_texture_indices() {
            for(auto i : m_rendered.m_texture_index) {
                printf("%f\n", i);
            }
        }

        void __debug_colors() {
            for(int i = 0; i < m_rendered.m_joined_data.m_color.size() / 4; i++) {
                printf("Iter: %d          %f %f %f %f\n", i, m_rendered.m_joined_data.m_color[i * 4 + 0], m_rendered.m_joined_data.m_color[i * 4 + 1], m_rendered.m_joined_data.m_color[i * 4 + 2], m_rendered.m_joined_data.m_color[i * 4 + 3]);
                fflush(stdout);
            }
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