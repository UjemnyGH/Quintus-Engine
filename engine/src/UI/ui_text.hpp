#pragma once
#ifndef __UI_TEXT_
#define __UI_TEXT_

#include "../../vendor/stb/stb_truetype.h"
#include "../../vendor/glad/glad.h"
#include "../Core/c_buffers.hpp"
#include "../Core/c_engine_functions.hpp"
#include "../Core/c_algorithms.hpp"
#include "../Core/c_layers.hpp"
#include <iostream>

qe::shader_t text_vertex_shader = "#version 450 core\n"
"layout(location = 0) in vec4 aPos;\n"
"layout(location = 1) in vec2 aTex;\n"
"out vec2 Tex;\n"
"void main() {\n"
"gl_Position = aPos;\n"
"Tex = aTex;\n"
"}\n";

qe::shader_t text_fragment_shader = "#version 450 core\n"
"uniform sampler2D Texture;"
"in vec2 Tex;\n"
"out vec4 Text;\n"
"void main() {\n"
"Text = texture(Texture, Tex);\n"
"}\n";

namespace qe {
    namespace ui {
        class Text : public Layer {
        private:
            Shader m_sh;
            Vao m_vao;
            Vbo m_position_buff, m_texture_buff;
            Texture m_texture;
            Vector<float> m_text_parameters = {0.0f, 0.0f, 1.0f, 1.0f};

            std::vector<float> m_position;
            std::vector<float> m_texture_coordinates;

            unsigned char ttf_buffer[1 << 20];
            unsigned char temp_bitmap[512 * 512];

            stbtt_bakedchar cdata[96];

        public:
            Text() {
                m_layer_debug_name = "Text";
                m_auto_start = true;
            }

            void LoadTTF(std::string const font) {
                fread(ttf_buffer, 1, 1 << 20, fopen(font.c_str(), "rb"));

                stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata);
            }

            virtual void Start() override {
                m_sh.AttachShader(LoadShader(text_vertex_shader, vertex));
                m_sh.AttachShader(LoadShader(text_fragment_shader, fragment));
                m_sh.LinkProgram();

                m_vao.Init();
                m_position_buff.Init();
                m_texture_buff.Init();
                m_texture.Init();
            }

            virtual void Update() override {
                m_sh.Bind();
                m_vao.Bind();

                m_position_buff.Bind(m_position, 0, 2);
                m_texture_buff.Bind(m_texture_coordinates, 1, 2);

                m_sh.Bind();

                glUniform1i(glGetUniformLocation(m_sh.m_id, "Texture"), 0);

                glDrawArrays(GL_TRIANGLES, 0, m_position.size() / 3);

                m_sh.Unbind();
                m_vao.Unbind();
            }
        };

        unsigned char* make_bitmap(std::string text, std::string font) {
            long size;
            unsigned char* fontBuffer;
            
            FILE* fontFile = fopen(font.c_str(), "rb");
            fseek(fontFile, 0, SEEK_END);
            size = ftell(fontFile);
            fseek(fontFile, 0, SEEK_SET);
            
            fontBuffer = new unsigned char[size];
            
            fread(fontBuffer, size, 1, fontFile);
            fclose(fontFile);

            stbtt_fontinfo info;
            if (!stbtt_InitFont(&info, fontBuffer, 0))
            {
                printf("Failed to load font %s\n", font.c_str());
            }
            
            int b_width = 512;
            int b_height = 128;
            int l_height = 64;

            unsigned char* bitmap = new unsigned char[b_width * b_height];
            
            float scale = stbtt_ScaleForPixelHeight(&info, l_height);
            
            int font_x = 0;
            
            int ascent, descent, lineGap;
            stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
            
            ascent = ascent * scale;
            descent = descent * scale;
            
            int i;
            for (uint32_t i = 0; i < text.length(); i++)
            {
                int ax;
                int lsb;
                stbtt_GetCodepointHMetrics(&info, text.c_str()[i], &ax, &lsb);
                
                int c_x1, c_y1, c_x2, c_y2;
                stbtt_GetCodepointBitmapBox(&info, text.c_str()[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
                
                int y = ascent + c_y1;
                
                int byteOffset = font_x + lsb * scale + (y * b_width);
                stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_width, scale, scale, text.c_str()[i]);

                font_x += ax * scale;
                
                int kern;
                kern = stbtt_GetCodepointKernAdvance(&info, text.c_str()[i], text.c_str()[i + 1]);
                font_x += kern * scale;
            }
            
            free(fontBuffer);
            free(bitmap);

            delete[] fontBuffer;
            
            return bitmap;
        }

        void RenderText(std::string text, float x, float y, float size_x, float size_y, std::string font) {
            unsigned char ttf_buffer[1 << 20];
            unsigned char temp_bitmap[512 * 512];

            stbtt_bakedchar cdata[96];

            fread(ttf_buffer, 1, 1 << 20, fopen(font.c_str(), "rb"));

            stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata);

            std::vector<float> pos;
            std::vector<float> texPos;

            Shader sh;

            sh.AttachShader(LoadShader(text_vertex_shader, vertex));
            sh.AttachShader(LoadShader(text_fragment_shader, fragment));
            sh.LinkProgram();

            Vao vao;
            vao.Bind();

            Vbo position;
            Vbo texPosition;

            Texture tex;
            tex.Init();

            glBindTexture(GL_TEXTURE_2D, tex.m_id);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            for(char ch : text) {
                if(ch >= 32 && ch <= 128) {
                    stbtt_aligned_quad q;
                    stbtt_GetBakedQuad(cdata, 512, 512, ch - 32, &x, &y, &q, 1);

                    /*texPos.push_back(q.s0 / 512.0f); texPos.push_back(q.t0 / 512.0f);
                    texPos.push_back(q.s1 / 512.0f); texPos.push_back(q.t0 / 512.0f);
                    texPos.push_back(q.s1 / 512.0f); texPos.push_back(q.t1 / 512.0f);
                    texPos.push_back(q.s1 / 512.0f); texPos.push_back(q.t0 / 512.0f);
                    texPos.push_back(q.s1 / 512.0f); texPos.push_back(q.t1 / 512.0f);
                    texPos.push_back(q.s0 / 512.0f); texPos.push_back(q.t1 / 512.0f);*/

                    texPos.push_back(1.0f); texPos.push_back(1.0f);
                    texPos.push_back(0.0f); texPos.push_back(1.0f);
                    texPos.push_back(1.0f); texPos.push_back(0.0f);
                    texPos.push_back(0.0f); texPos.push_back(1.0f);
                    texPos.push_back(1.0f); texPos.push_back(0.0f);
                    texPos.push_back(0.0f); texPos.push_back(0.0f);

                    pos.push_back((q.x0 / 32.0f) * size_x); pos.push_back((q.y0 / 32.0) * size_y);
                    pos.push_back((q.x1 / 32.0f) * size_x); pos.push_back((q.y0 / 32.0) * size_y);
                    pos.push_back((q.x0 / 32.0f) * size_x); pos.push_back((q.y1 / 32.0) * size_y);
                    pos.push_back((q.x1 / 32.0f) * size_x); pos.push_back((q.y0 / 32.0) * size_y);
                    pos.push_back((q.x0 / 32.0f) * size_x); pos.push_back((q.y1 / 32.0) * size_y);
                    pos.push_back((q.x1 / 32.0f) * size_x); pos.push_back((q.y1 / 32.0) * size_y);
                }
            }

            position.Bind(pos, 0, 2);
            texPosition.Bind(texPos, 1, 2);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
			glGenerateMipmap(GL_TEXTURE_2D);

            sh.Bind();

            glUniform1i(glGetUniformLocation(sh.m_id, "Texture"), 0);

            glDrawArrays(GL_TRIANGLES, 0, pos.size());

            sh.Unbind();
            vao.Unbind();
        }
    }
}

#endif