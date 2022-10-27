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

namespace qe
{
    class QE_Renderer : public Layer {
    private:
        Vao m_vao;
        std::vector<Vbo> m_vbos;
        Shader m_sh;
        Ebo m_ebo;
        std::vector<uint32_t> m_shaders;

        Rendered m_rendered;

        static glm::mat4 projection;
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);

    public:
        void ReattachShaders() {
            m_sh.Unbind();
            m_sh.~QE_Shader();

            for(uint32_t shader : m_shaders) {
                m_sh.AttachShader(shader);
            }

            m_sh.LinkProgram();
        }

        void AddShader(uint32_t shader_id) {
            m_shaders.push_back(shader_id);

            ReattachShaders();
        }

        void ClearShaders() {
            m_shaders.clear();

            ReattachShaders();
        }

        virtual void Start() override {
            ClearShaders();

            m_vao.Bind();

            m_vbos.resize(4);
            
            m_vbos[0].Bind(m_rendered.m_joined_data.m_vertices, 0);
            m_vbos[1].Bind(m_rendered.m_joined_data.m_color, 1, 4);
            m_vbos[2].Bind(m_rendered.m_joined_data.m_texture_coordinates, 2, 2);
            m_vbos[3].Bind(m_rendered.m_joined_data.m_normals, 3);
        }

        virtual void End() override {
            ClearShaders();

            m_sh.~QE_Shader();
            m_vao.~QE_Vao();
            for(Vbo vbo : m_vbos) {
                vbo.~QE_Vbo();
            }

            m_ebo.~QE_Ebo();
        }
    };
} // namespace qe


#endif