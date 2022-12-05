#pragma once
#ifndef __R_MODEL_LOADER_
#define __R_MODEL_LOADER_

#include "r_renderer_helpers.hpp"
#include "../Core/c_engine_functions.hpp"
#include "r_model_loader_helpers.hpp"
#include <sstream>
#include <bit>

namespace qe
{
    class QE_ModelLoader {
    private:
        std::vector<RenderedData> m_data;
        bool m_model_reloaded = true;

        GLTF_Data load_data_from_gltf_json_file(std::string const gltf_path) {
            GLTF_Data data;

            std::ifstream f(gltf_path, std::ios::binary);

            if(f.bad() || f.fail() || !f.is_open()) {
                std::cerr << "Cannot open file: " << gltf_path << std::endl;
            }

            std::string line;

            bool scene_pick = false;

            while(!f.eof()) {
                getline(f, line);
                
                if(line.find("\"scene\"") == 0) {
                    uint32_t scene;
                    void *flushed;

                    std::stringstream ss(line.c_str());

                    ss >> flushed >> flushed >> scene;

                    data.scene = scene;
                }
                else if(line.find("\"scenes\"") == 0) {
                    scene_pick = true;
                }
            }
        }

        std::vector<BufferMeshData> LoadGLTFBin(std::string path) {
            std::vector<BufferMeshData> result;

            FILE *f = fopen(path.c_str(), "rb");

            fseek(f, 0, SEEK_END);

            std::vector<byte_t> data(ftell(f));

            fseek(f, 0, SEEK_SET);

            fread(data.data(), sizeof(byte_t), data.size(), f);

            fclose(f);

            byte_t bytes[4];
            uint32_t byte_iterator = 0;
            uint32_t byte_counter = 0;

            for(byte_t byte : data) {
                bytes[3 - byte_iterator] = byte;

                byte_iterator++;



                byte_counter++;
            }
        }

        std::vector<BufferMeshData> LoadOBJ(std::string path) {
            std::vector<BufferMeshData> data;
            std::vector<BufferMeshData> helper_saved;
            BufferMeshData helper;
            uint32_t current_pusher = 0;

            std::ifstream f(path, std::ios::binary);

            if(f.bad() || f.fail() || !f.is_open()) {
                std::cerr << "Bad file: " << path << '\n';

                return data;
            }

            std::string line;

            while(!f.eof()) {
                getline(f, line);

                if(line.find("o ") == 0) {
                    data.resize(current_pusher + 1);

                    current_pusher++;

                    if(m_debug) {
                        std::cout << line << std::endl;
                    }
                }
                else if(line.find("v ") == 0) {
                    std::stringstream ss(line.c_str() + 2);

                    float x, y, z;

                    ss >> x >> y >> z;

                    helper.m_vertices.push_back(x);
                    helper.m_vertices.push_back(y);
                    helper.m_vertices.push_back(z);

                    if(m_debug) {
                        std::cout << line << std::endl;
                    }
                }
                else if(line.find("vt ") == 0) {
                    std::stringstream ss(line.c_str() + 3);

                    float x, y;

                    ss >> x >> y;

                    helper.m_texture_coordinates.push_back(x);
                    helper.m_texture_coordinates.push_back(y);

                    if(m_debug) {
                        std::cout << line << std::endl;
                    }
                }
                else if(line.find("vn ") == 0) {
                    std::stringstream ss(line.c_str() + 3);

                    float x, y, z;

                    ss >> x >> y >> z;

                    helper.m_normals.push_back(x);
                    helper.m_normals.push_back(y);
                    helper.m_normals.push_back(z);

                    if(m_debug) {
                        std::cout << line << std::endl;
                    }
                }
                else if(line.find("f ") == 0) {
                    bool doubleSlash = false;
                    uint32_t slash_amount = 0;

                    for(int i = 1; i < line.size(); i++) {
                        if(line[i - 1] == '/' && line[i] == '/') {
                            doubleSlash = true;

                            line[i - 1] = ' ';
                            line[i] = ' ';

                            slash_amount++;
                        }
                        else if(line[i - 1] == '/' && line[i] != '/') {
                            doubleSlash = false;

                            line[i - 1] = ' ';

                            slash_amount++;
                        }
                    }

                    if(m_debug) {
                        std::cout << line << std::endl;
                    }

                    std::stringstream ss(line.c_str() + 2);

                    uint32_t iv[4], it[4], in[4];
                    if(slash_amount >= 7) {
                        if(doubleSlash) {
                            ss >> iv[0] >> in[0] >> iv[1] >> in[1] >> iv[2] >> in[2] >> iv[3] >> in[3];
                        
                            for(int i = 0; i < 3; i++) {
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3 + 2]);

                                m_vertices_amount_loaded++;

                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3 + 2]);
                            }

                            for(int i = 1; i < 4; i++) {
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i - (i == 1 ? 1 : 0)] - 1) * 3]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i - (i == 1 ? 1 : 0)] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i - (i == 1 ? 1 : 0)] - 1) * 3 + 2]);

                                m_vertices_amount_loaded++;

                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i - (i == 1 ? 1 : 0)] - 1) * 3]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i - (i == 1 ? 1 : 0)] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i - (i == 1 ? 1 : 0)] - 1) * 3 + 2]);

                            }
                        }
                        else {
                            ss >> iv[0] >> it[0] >> in[0] >> iv[1] >> it[1] >> in[1] >> iv[2] >> it[2] >> in[2] >> iv[3] >> it[3] >> in[3];

                            if(m_debug) {
                                printf("Faces check:\t%d %d %d - %d %d %d - %d %d %d - %d %d %d\n", iv[0], it[0], in[0], iv[1], it[1], in[1], iv[2], it[2], in[2], iv[3], it[3], in[3]);
                            }

                            for(int i = 0; i < 3; i++) {
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3 + 2]);

                                m_vertices_amount_loaded++;

                                data[current_pusher - 1].m_texture_coordinates.push_back(helper.m_texture_coordinates[(it[i] - 1) * 2]);
                                data[current_pusher - 1].m_texture_coordinates.push_back(helper.m_texture_coordinates[(it[i] - 1) * 2 + 1]);

                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3 + 2]);
                            }

                            for(int i = 1; i < 4; i++) {
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i - (i == 1 ? 1 : 0)] - 1) * 3]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i - (i == 1 ? 1 : 0)] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i - (i == 1 ? 1 : 0)] - 1) * 3 + 2]);

                                m_vertices_amount_loaded++;

                                data[current_pusher - 1].m_texture_coordinates.push_back(helper.m_texture_coordinates[(it[i - (i == 1 ? 1 : 0)] - 1) * 2]);
                                data[current_pusher - 1].m_texture_coordinates.push_back(helper.m_texture_coordinates[(it[i - (i == 1 ? 1 : 0)] - 1) * 2 + 1]);

                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i - (i == 1 ? 1 : 0)] - 1) * 3]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i - (i == 1 ? 1 : 0)] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i - (i == 1 ? 1 : 0)] - 1) * 3 + 2]);

                            }
                        }
                    }
                    else {
                        if(doubleSlash) {
                            ss >> iv[0] >> in[0] >> iv[1] >> in[1] >> iv[2] >> in[2] >> iv[3] >> in[3];
                        
                            for(int i = 0; i < 3; i++) {
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3 + 2]);

                                m_vertices_amount_loaded++;

                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3 + 2]);
                            }
                        }
                        else {
                            ss >> iv[0] >> it[0] >> in[0] >> iv[1] >> it[1] >> in[1] >> iv[2] >> it[2] >> in[2];

                            if(m_debug) {
                                printf("Faces check:\t%d %d %d - %d %d %d - %d %d %d\n", iv[0], it[0], in[0], iv[1], it[1], in[1], iv[2], it[2], in[2]);
                            }

                            for(int i = 0; i < 3; i++) {
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_vertices.push_back(helper.m_vertices[(iv[i] - 1) * 3 + 2]);

                                m_vertices_amount_loaded++;

                                data[current_pusher - 1].m_texture_coordinates.push_back(helper.m_texture_coordinates[(it[i] - 1) * 2]);
                                data[current_pusher - 1].m_texture_coordinates.push_back(helper.m_texture_coordinates[(it[i] - 1) * 2 + 1]);

                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3 + 1]);
                                data[current_pusher - 1].m_normals.push_back(helper.m_normals[(in[i] - 1) * 3 + 2]);
                            }
                        }
                    }
                }
            }

            return data;
        }
    
    public:
        uint32_t m_vertices_amount_loaded = 0;

        QE_ModelLoader() {}

        bool m_debug = false;

        void LoadModelUnindexed(const std::string path, const ModelType type = ModelType::OBJ) {
            m_model_reloaded = true;
            std::vector<BufferMeshData> data;

            switch(type) {
            default:
            case ModelType::OBJ:
                data = LoadOBJ(path);
                m_data.clear();
                m_data.resize(data.size());
                
                for(int i = 0; i < data.size(); i++) {
                    std::copy(data[i].m_vertices.begin(), data[i].m_vertices.end(), std::back_inserter(m_data[i].m_vertices));
                    std::copy(data[i].m_normals.begin(), data[i].m_normals.end(), std::back_inserter(m_data[i].m_normals));
                    std::copy(data[i].m_texture_coordinates.begin(), data[i].m_texture_coordinates.end(), std::back_inserter(m_data[i].m_texture_coordinates));

                    for(int j = 0; j < m_data[i].m_vertices.size() / 3; j++) {
                        if(m_debug) {
                            std::cerr << m_data[i].m_vertices[j * 3] << ' ' << m_data[i].m_vertices[j * 3 + 1] << ' ' << m_data[i].m_vertices[j * 3 + 2] << '\n';
                        }

                        m_data[i].m_color.push_back(1.0f);
                        m_data[i].m_color.push_back(1.0f);
                        m_data[i].m_color.push_back(1.0f);
                        m_data[i].m_color.push_back(1.0f);
                    }

                    m_data[i].m_indices.push_back(-1);
                }
                
                break;

            case ModelType::GLTF:
                // TODO: GLTF loader

                break;

            case ModelType::STL:
                // TODO: STL loader

                break;

            case ModelType::DAE:
                // TODO: DAE loader

                break;

            case ModelType::PLY:
                // TODO: PLY loader

                break;

            case ModelType::X3D:
                // TODO: X3D loader

                break;

            case ModelType::FBX:
                // TODO: FBX loader

                break;

            case ModelType::ABC:
                // TODO: ABC loader

                break;
            }
        }

        RenderedData GetModelData() {
            m_model_reloaded = false;

            return m_data[0];
        }

        std::vector<RenderedData> GetModelVectorData() {
            m_model_reloaded = false;
            
            return m_data;
        }

        ~QE_ModelLoader() {}
    };

    typedef QE_ModelLoader ModelLoader;
} // namespace qe


#endif