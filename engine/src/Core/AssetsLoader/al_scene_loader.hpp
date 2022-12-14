#pragma once
#ifndef __AL_SCENE_LOADER_
#define __AL_SCENE_LOADER_

#include <string>
#include "../c_algorithms.hpp"
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <bit>

const std::string __scene_file_extension__ = ".qescene";

namespace qe {
    /*enum SceneFlags {
        scene_separator = (uint16_t)0x00,

        scene_begin = (uint16_t)0x0101,

        scene_beg_name = (uint16_t)0x0201,

        scene_var_beg_name = (uint16_t)0x0301,
        scene_var_amount = (uint32_t)0x0303,
        scene_var_float = (uint16_t)0x0304,
        scene_var_double = (uint16_t)0x0305,
        scene_var_int = (uint16_t)0x0306,
        scene_var_uint = (uint16_t)0x0307,
        scene_var_char = (uint16_t)0x0308,
        scene_var_string = (uint16_t)0x0309,
        scene_var_bool = (uint16_t)0x030a,
        scene_var_value_separator = (uint16_t)0x030b,

        scene_var_beg_value = (uint16_t)0x0401,
        scene_var_beg_value = (uint16_t)0x0401,

        scene_struct_beg = (uint16_t)0x0501,

        scene_struct_name_beg = (uint16_t)0x0601,
    };*/

    struct SceneVariableValues {
        std::vector<float> m_values;
    };

    struct SceneStruct {
        std::string m_struct_name;
        std::unordered_map<std::string, SceneVariableValues> m_struct_variables;
    };

    struct SceneInfo {
        std::string m_scene_name;
        std::vector<SceneStruct> m_scene_structs;
    };

    class Scene {
    private:
        std::string m_scene;
        std::string m_scene_path;

        SceneInfo m_scene_info;

    public:
        void BeginScene(std::string scene_name) {
            m_scene_path = scene_name + __scene_file_extension__;

            std::ofstream f(m_scene_path, std::ios::binary);

            f.close();

            m_scene += "SCENE " + scene_name + "\n";

            m_scene_info.m_scene_name = scene_name;
        }

        void AddStruct(std::string struct_name) {
            m_scene += "STRUCT " + struct_name + "\n";
            m_scene_info.m_scene_structs.resize(m_scene_info.m_scene_structs.size() + 1);
            m_scene_info.m_scene_structs[m_scene_info.m_scene_structs.size() - 1].m_struct_name = struct_name;
        }

        template<class T>
        void AddValue(std::string name, T *values, uint32_t const &values_amount) {
            m_scene += "VAR " + name + " " + std::to_string(values_amount) + "\n";
            SceneVariableValues vals;
            vals.m_values.resize(values_amount);

            for(int i = 0; i < values_amount; i++) {
                m_scene += std::to_string(vals.m_values[i]) + " ";
            }

            m_scene += "\n";
        }

        void GenerateScene() {
            std::ofstream f(m_scene_path, std::ios::binary);

            f.write(m_scene.c_str(), m_scene.size());

            f.close();
        }

        void ReadScene(std::string scene_path) {
            m_scene_path = scene_path + __scene_file_extension__;

            std::ifstream f(m_scene_path, std::ios::binary);

            std::string line;

            bool variables = false;
            size_t variables_size = 0;

            while(!f.eof()) {
                getline(f, line);

                if(line.find("SCENE ") == 0) {
                    m_scene_info.m_scene_name = line.c_str() + 6;
                }
                else if(line.find("STRUCT ") == 0) {
                    m_scene_info.m_scene_structs.resize(m_scene_info.m_scene_structs.size() + 1);
                    m_scene_info.m_scene_structs[m_scene_info.m_scene_structs.size() - 1].m_struct_name = line.c_str() + 7;
                }
                else if(line.find("VAR ") == 0) {
                    variables = true;
                    std::stringstream ss(line.c_str() + 4);

                    uint32_t size;
                    std::string name;

                    ss.seekg(std::ios::beg);

                    ss >> name >> size;
                    ss.seekg(ss.tellg());

                    SceneVariableValues values;

                    values.m_values.resize(size);

                    for(uint32_t i = 0; i < size; i++) {
                        ss.seekg(ss.tellg());
                        ss >> values.m_values[i];
                    }

                    m_scene_info.m_scene_structs[m_scene_info.m_scene_structs.size() - 1].m_struct_variables.insert(std::make_pair(name, values));
                }
            }

            f.close();
        }

        SceneInfo* GetScenePtr() { return &m_scene_info; }
    };
}

#endif