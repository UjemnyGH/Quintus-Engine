#pragma once
#ifndef __UI_COMPONENTS_
#define __UI_COMPONENTS_

#include "../Core/c_algorithms.hpp"
#include "../Core/c_hard_coded_stuff.hpp"
#include "../Core/c_layers.hpp"
#include "../Core/c_buffers.hpp"

namespace qe {
    namespace ui {
        struct UIRendered {
            std::vector<float> m_vertices;
            std::vector<float> m_colors;
            std::vector<float> m_texture_coordinates;
            std::vector<float> m_used_texture;
            std::vector<std::string> m_object_names;
            uint32_t m_size;
            uint32_t m_objects_amount = 0;
        };

        const UIRendered square = {
            {
                1.0f, 1.0f,
                -1.0f, 1.0f,
                1.0f, -1.0f,

                -1.0f, 1.0f,
                1.0f, -1.0f,
                -1.0f, -1.0f,
            },
            {
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,

                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
            },
            {
                1.0f, 0.0f,
                0.0f, 0.0f, 
                1.0f, 1.0f,

                0.0f, 0.0f, 
                1.0f, 1.0f,
                0.0f, 1.0f
            },
            {
                0.0f,
                0.0f,
                0.0f,
                0.0f,
                0.0f,
                0.0f,
            }
        };

        class UIRenderer : public Layer {
        private:
            const shader_t ui_vertex = "#version 450 core\n"
            "layout(location = 0) in vec4 aPos;\n"
            "layout(location = 1) in vec4 aCol;\n"
            "layout(location = 2) in vec2 aTexCoords;\n"
            "layout(location = 3) in float aTexID;\n"
            "out vec4 Col;\n"
            "out vec2 TexCoords;\n"
            "out float TexID;\n"
            "void main() {\n"
            "gl_Position = aPos;\n"
            "Col = aCol;\n"
            "TexCoords = aTexCoords;\n"
            "TexID = aTexID;\n"
            "}\n\0";

            const shader_t ui_fragment = "#version 450 core\n"
            "uniform sampler2D Textures[32];"
            "in vec4 Col;\n"
            "in vec2 TexCoords;\n"
            "in float TexID;\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "int TID = int(TexID);\n"
            "if(TID != 0) {\n"
            "FragColor = texture(Textures[TID], TexCoords) * Col;\n"
            "}\n"
            "else {\n"
            "FragColor = Col;\n"
            "}\n"
            "}\n\0";

            Shader m_shader;
            Vao m_vao;
            Vbo m_pos;
            Vbo m_col;
            Vbo m_tex_coord;
            Vbo m_used_tex;
            Texture m_textures[32];

        protected:
            UIRendered m_rendered;
            uint32_t m_textures_amount = 0;
            std::vector<uint32_t> m_texture_priority_queue;

        public:
            virtual void Start() override {
                m_shader.AttachShader(qe::LoadShader(ui_vertex, qe::vertex));
                m_shader.AttachShader(qe::LoadShader(ui_fragment, qe::fragment));
                m_shader.LinkProgram();

                m_vao.Bind();
                m_shader.Bind();

                for(int i = 0; i < 32; i++) {
                    m_textures[i].Init();
                }

                m_textures[0].Uniform(m_shader.m_id, "Textures");

                m_shader.Unbind();
                m_vao.Unbind();
            }

            virtual void Update() override {
                m_vao.Bind();
                m_shader.Bind();

                for(int i = 0; i < 32; i++) {
                    m_textures[i].Bind(i);
                }

                glDrawArrays(GL_TRIANGLES, 0, m_rendered.m_vertices.size() / 2);

                m_shader.Unbind();
                m_vao.Unbind();
            }

            void AddTexture(std::string path) {
                if(m_textures_amount < 32 && m_texture_priority_queue.size() < 1) {
                    m_textures[m_textures_amount].Bind(path, GL_REPEAT, false);
                    m_textures_amount++;
                }
                else if(m_texture_priority_queue.size() > 0) {
                    m_textures[m_texture_priority_queue[0]].Bind(path, GL_REPEAT, false);
                    m_texture_priority_queue.erase(m_texture_priority_queue.begin());
                }
                else {
                    m_textures[31].Bind(path, GL_REPEAT, false);
                }
            }

            void RemoveTexture(uint32_t id) {
                m_texture_priority_queue.push_back(id < 32 ? id : 31);
                m_textures[id < 32 ? id : 31].~QE_Texture();
                m_textures[id < 32 ? id : 31].Init();
            }

            void AddData(UIRendered const &data, std::string const object_name = "___UIComp___") {
                m_rendered.m_objects_amount++;
                std::copy(m_rendered.m_vertices.begin(), m_rendered.m_vertices.end(), std::back_inserter(data.m_vertices));
                std::copy(m_rendered.m_colors.begin(), m_rendered.m_colors.end(), std::back_inserter(data.m_colors));
                std::copy(m_rendered.m_texture_coordinates.begin(), m_rendered.m_texture_coordinates.end(), std::back_inserter(data.m_texture_coordinates));
                std::copy(m_rendered.m_used_texture.begin(), m_rendered.m_used_texture.end(), std::back_inserter(data.m_used_texture));
                m_rendered.m_object_names.push_back(object_name + std::to_string(m_rendered.m_objects_amount));

                m_rendered.m_size = (m_rendered.m_vertices.size() / 2) / m_rendered.m_objects_amount;
            }

            void RemoveData(uint32_t id) {
                m_rendered.m_object_names.erase(m_rendered.m_object_names.begin() + id);
                m_rendered.m_vertices.erase(m_rendered.m_vertices.begin() + (id * m_rendered.m_size * 2), m_rendered.m_vertices.begin() + (id * m_rendered.m_size * 2) + (m_rendered.m_size * 2));
                m_rendered.m_colors.erase(m_rendered.m_colors.begin() + (id * m_rendered.m_size * 4), m_rendered.m_colors.begin() + (id * m_rendered.m_size * 4) + (m_rendered.m_size * 4));
                m_rendered.m_texture_coordinates.erase(m_rendered.m_texture_coordinates.begin() + (id * m_rendered.m_size * 2), m_rendered.m_texture_coordinates.begin() + (id * m_rendered.m_size * 2) + (m_rendered.m_size * 2));
                m_rendered.m_used_texture.erase(m_rendered.m_used_texture.begin() + (id * m_rendered.m_size), m_rendered.m_used_texture.begin() + (id * m_rendered.m_size) + m_rendered.m_size);
                m_rendered.m_objects_amount--;
                m_rendered.m_size = (m_rendered.m_vertices.size() / 2) / m_rendered.m_objects_amount;
            }

            void SetPosition(uint32_t id, float x, float y) {

            }
        };

        class UIComponent : public UIRenderer {
        public:
            UIComponent() {
                
            }
        };

        class Button : public UIComponent {
        private:
            float *m_mouse_x;
            float *m_mouse_y;

            Vector<float> m_button_params = Vector<float>(0.0f, 0.0f, 0.1f, 0.1f);

        public:
            Button(float *mouse_x, float *mouse_y) : m_mouse_x(mouse_x), m_mouse_y(mouse_y) { }

            void SetButtonPosition(float x, float y) { m_button_params.x = x; m_button_params.y = y; }
            void SetButtonSize(float x, float y) { m_button_params.z = x; m_button_params.w = y; }

            bool CheckOnDrag() {
                return *m_mouse_x < m_button_params.x + m_button_params.z && *m_mouse_x > m_button_params.x - m_button_params.z && *m_mouse_y < m_button_params.y + m_button_params.w && *m_mouse_y > m_button_params.y - m_button_params.w; 
            }
        };
    }   
}


#endif