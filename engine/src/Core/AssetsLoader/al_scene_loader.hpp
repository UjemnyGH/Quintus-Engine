#pragma once
#ifndef __AL_SCENE_LOADER_
#define __AL_SCENE_LOADER_

#include <string>
#include "../c_algorithms.hpp"
#include <fstream>

const std::string __scene_file_extension__ = ".qescene";

namespace qe {
    class Scene {
    private:
        std::string m_scene_data;
        std::string m_scene_name;

        enum SceneFlags {
            scene_separator = (uint16_t)0x00,
            scene_begin = (uint16_t)0x0101,
            scene_end = (uint16_t)0x0102,
            scene_beg_name = (uint16_t)0x0201,
            scene_end_name = (uint16_t)0x0202,
            scene_var_beg_name = (uint16_t)0x0301,
            scene_var_end_name = (uint16_t)0x0302,
            scene_var_beg_value = (uint16_t)0x0401,
            scene_var_end_value = (uint16_t)0x0402,
        };

        static std::string make_bits(SceneFlags flag) {
            std::string _flag;
            _flag += (char)(flag >> 8);
            _flag += (char)((flag << 8) >> 8);

            return _flag;
        }

    public:
        Scene(std::string const &scene_name) {
            m_scene_name = scene_name;

            std::ofstream f(scene_name + __scene_file_extension__, std::ios::binary);

            f.close();

            m_scene_data += make_bits(scene_begin);
            m_scene_data += (char)scene_separator;
            m_scene_data += make_bits(scene_beg_name);
            m_scene_data += (char)scene_separator;

            for(auto name_ch : scene_name) {
                m_scene_data += name_ch;
            }

            m_scene_data += (char)scene_separator;
            m_scene_data += make_bits(scene_end_name);
            m_scene_data += (char)scene_separator;
        }

        void GenerateSceneFile() {
            m_scene_data += make_bits(scene_end);
            m_scene_data += (char)scene_separator;

            std::ofstream f(m_scene_name + __scene_file_extension__, std::ios::binary);

            f.write(m_scene_data.c_str(), m_scene_data.size());

            f.close(); 
        }

        template<class T>
        static std::string GeneratePartSceneData(T const &data, std::string const &variables_name) {
            std::string _scene;

            _scene += make_bits(scene_var_beg_name);
            _scene += (char)scene_separator;
            
            for(auto name_ch : variables_name) {
                _scene += name_ch;
            }
            
            _scene += (char)scene_separator;
            _scene += make_bits(scene_var_end_name);
            _scene += (char)scene_separator;

            _scene += make_bits(scene_var_beg_value);
            _scene += (char)scene_separator;

            std::string var_data = std::to_string(data);

            for(auto v : var_data) {
                _scene += v;
            }
            
            _scene += (char)scene_separator;
            _scene += make_bits(scene_var_end_value);
            _scene += (char)scene_separator;

            return _scene; 
        }

        std::string getSceneData() { return m_scene_data; }

        void setSceneData(std::string const &scene_data) { m_scene_data = scene_data; }

        void addSceneData(std::string const &partial_scene_data) { m_scene_data += partial_scene_data; }
    };
}

#endif 
