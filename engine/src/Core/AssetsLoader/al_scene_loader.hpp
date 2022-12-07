#pragma once
#ifndef __AL_SCENE_LOADER_
#define __AL_SCENE_LOADER_

#include <string>
#include "../c_algorithms.hpp"
#include <map>
#include <sstream>
#include <fstream>

const std::string __scene_file_extension__ = ".qescene";

namespace qe {
    enum ScenePossibleValuesTypes {
        v_float,
        v_double,
        v_int,
        v_uint,
        v_char,
        v_string,
        v_bool,
        v_null,
    };

    struct ScenePossibleValues {
        union {
            float m_flt[0x100];
            double m_dbl[0x100];
            int m_int[0x100];
            uint m_uint[0x100];
            char m_char[0x100];
            char *m_string[0x100];
            bool m_bool[0x100];
        };

        ScenePossibleValues() = default;

        ScenePossibleValuesTypes m_values_type = v_null;
        uint32_t m_scene_values_size = 0;
        std::string m_var_name;

        void add_value(std::string const &data, uint32_t string_size = 0) {
            if(m_values_type == v_null) {
                return;
            }

            std::stringstream ss(data.c_str());

            if(m_scene_values_size < 0x100) {
                switch(m_values_type) {
                    case v_float:
                        ss >> m_flt[m_scene_values_size];
                        m_scene_values_size++;

                        break;

                    case v_double:
                        ss >> m_dbl[m_scene_values_size];
                        m_scene_values_size++;

                        break;

                    case v_int:
                        ss >> m_int[m_scene_values_size];
                        m_scene_values_size++;

                        break;

                    case v_uint:
                        ss >> m_uint[m_scene_values_size];
                        m_scene_values_size++;

                        break;

                    case v_char:
                        ss >> m_char[m_scene_values_size];
                        m_scene_values_size++;

                        break;

                    case v_string:
                        m_string[m_scene_values_size] = new char[data.size()];
                        m_string[m_scene_values_size] = (char*)data.data();
                        m_scene_values_size++;

                        break;

                    case v_bool:
                        m_bool[m_scene_values_size] = (data.c_str()[0] << 7) >> 7;
                        m_scene_values_size++;

                        break;

                    default:
                        break;
                }
            }
        }

        template<class T>
        T operator[](uint32_t n) {
            switch(m_values_type) {
                case v_float:
                    return m_flt[n];

                    break;

                case v_double:
                    return m_dbl[n];

                    break;

                case v_int:
                    return m_int[n];

                    break;

                case v_uint:
                    return m_uint[n];

                    break;

                case v_char:
                    return m_char[n];

                    break;

                case v_string:
                    return m_string[n];

                    break;

                case v_bool:
                    return m_bool[n];

                    break;

                default:
                    break;
            }

            return (uint64_t)-1;
        }

        auto getElement(uint32_t n) {
            switch(m_values_type) {
                case v_float:
                    return m_flt[n];

                    break;

                /*case v_double:
                    return m_dbl[n];

                    break;

                case v_int:
                    return &m_int[n];

                    break;

                case v_uint:
                    return m_uint[n];

                    break;

                case v_char:
                    return m_char[n];

                    break;

                case v_string:
                    return m_string[n];

                    break;

                case v_bool:
                    return m_bool[n];

                    break;*/

                default:
                    break;
            }
        }

        ~ScenePossibleValues() {}
    };

    struct SceneValuesStruct {
        std::string m_struct_name;
        std::vector<ScenePossibleValues> m_scene_struct;

        void add_scene_values(ScenePossibleValues values) {
            m_scene_struct.push_back(values);
        }
    };

    class Scene {
    private:
        std::string m_scene_data;
        std::string m_scene_name;

        enum SceneFlags {
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

            scene_struct_beg = (uint16_t)0x0501,

            scene_struct_name_beg = (uint16_t)0x0601,
        };

        std::vector<SceneValuesStruct> m_scene_structs;

        static std::string make_bits(SceneFlags flag) {
            std::string _flag;
            _flag += (char)(flag >> 8);
            _flag += (char)((flag << 8) >> 8);

            return _flag;
        }

        static char get_first(SceneFlags flag) {
            return (char)((flag << 8) >> 8);
        }

        static char get_second(SceneFlags flag) {
            return (char)(flag >> 8);
        }

    public:
        Scene(std::string const &scene_name) {
            m_scene_name = scene_name;

            std::ofstream f(scene_name + __scene_file_extension__, std::ios::binary);

            f.close();

            m_scene_data += make_bits(scene_begin);
            m_scene_data += (char)scene_separator;
            m_scene_data += make_bits(scene_beg_name);

            for(auto name_ch : scene_name) {
                m_scene_data += name_ch;
            }

            m_scene_data += (char)scene_separator;
        }

        Scene() = default;

        void GenerateSceneFile() {
            std::ofstream f(m_scene_name + __scene_file_extension__, std::ios::binary);

            f.write(m_scene_data.c_str(), m_scene_data.size());

            f.close(); 
        }

        template<class T>
        static std::string GeneratePartSceneData(T const *data, uint32_t data_size, std::string const &variable_name) {
            std::string _scene;

            _scene += make_bits(scene_var_beg_name);
            
            for(auto name_ch : variable_name) {
                _scene += name_ch;
            }
            
            _scene += (char)scene_separator;
            _scene += make_bits(scene_var_beg_value);

            std::string var_data = std::to_string(data);

            for(auto v : var_data) {
                _scene += v;
            }
            
            _scene += (char)scene_separator;

            return _scene; 
        }

        void AddStructData(std::string const &struct_name) {
            m_scene_data += make_bits(scene_struct_beg);
            m_scene_data += (char)scene_separator;
            m_scene_data += make_bits(scene_struct_name_beg);

            for(auto ch : struct_name) {
                m_scene_data += ch;
            }

            m_scene_data += (char)scene_separator;
        }

        template<class T>
        void AddPartSceneData(T const *data, uint8_t const data_size, std::string const &variables_name, ScenePossibleValuesTypes const &var_type) {
            m_scene_data += make_bits(scene_var_beg_name);
            
            for(auto name_ch : variables_name) {
                m_scene_data += name_ch;
            }
            
            m_scene_data += (char)scene_separator;

            switch(var_type) {
                case v_float:
                    m_scene_data += make_bits(scene_var_float);

                    break;

                case v_double:
                    m_scene_data += make_bits(scene_var_double);

                    break;

                case v_int:
                    m_scene_data += make_bits(scene_var_int);

                    break;

                case v_uint:
                    m_scene_data += make_bits(scene_var_uint);

                    break;

                case v_char:
                    m_scene_data += make_bits(scene_var_char);

                    break;

                case v_string:
                    m_scene_data += make_bits(scene_var_string);

                    break;

                case v_bool:
                    m_scene_data += make_bits(scene_var_bool);

                    break;

                default:
                    break;
            }

            m_scene_data += (char)scene_separator;

            m_scene_data += make_bits(scene_var_amount);

            m_scene_data += data_size;

            m_scene_data += (char)scene_separator;
            m_scene_data += make_bits(scene_var_beg_value);

            for(uint8_t i = 0; i < data_size; i++) {
                std::string var_data = std::to_string(data[i]);

                for(auto v : var_data) {
                    m_scene_data += v;
                }

                m_scene_data += (char)scene_separator;
                m_scene_data += make_bits(scene_var_value_separator);
            }

            m_scene_data += (char)scene_separator;
        }

        void ReadScene(std::string const &scene_name) {
            std::ifstream f(scene_name + __scene_file_extension__, std::ios::binary | std::ios::ate);

            std::string _scene;

            _scene.resize(f.tellg());

            f.seekg(std::ios::beg);

            f.read(_scene.data(), _scene.size());

            f.close();

            bool scene_beginned = false;
            bool separator_find = false;
            bool value_to_convert_recording = false;
            bool size_set = false;
            uint32_t values_amount_in_variable = 0;
            uint32_t current_value = 0;
            uint32_t keyword_count;
            std::string value_to_convert;

            char keyword_buff[2] = {_scene.c_str()[0], _scene.c_str()[1]};

            for(uint32_t i = 0; i < _scene.size(); i++) {
                if(_scene.c_str()[i] == (char)scene_separator) {
                    separator_find = true;
                    value_to_convert_recording = false;
                    keyword_count = 0;
                    keyword_buff[0] = keyword_buff[1] = (char)0;
                }

                if(separator_find && keyword_count < 2) {
                    keyword_buff[keyword_count] = _scene.c_str()[i + 1];
                    keyword_count++;

                    if(make_bits(scene_var_beg_name) == std::string(&keyword_buff[0]) + std::string(&keyword_buff[1])) {
                        size_set = false;
                    }
                }

                std::string keyword;
                keyword += keyword_buff[0];
                keyword += keyword_buff[1];

                printf("0x%02x Key buffer: 0x%02x 0x%02x\n", _scene.c_str()[i], keyword_buff[0], keyword_buff[1]);

                if(keyword_count > 1 || !scene_beginned) {
                    if(keyword == make_bits(scene_begin)) {
                        scene_beginned = true;
                    }
                    else if(keyword == make_bits(scene_beg_name)) {
                        if(_scene.c_str()[i + 2] != (char)scene_separator) {
                            m_scene_name += _scene.c_str()[i + 2];
                        }
                    }
                    else if(keyword == make_bits(scene_var_beg_name)) {
                        if(_scene.c_str()[i + 2] != (char)scene_separator) {
                            if(!size_set) {
                                m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.resize(m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() + 1);

                                size_set = true;
                            }
                            m_scene_structs[m_scene_structs.size() - 1].m_scene_struct[m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() - 1].m_var_name += _scene.c_str()[i + 2];
                        }
                    }
                    else if(keyword == make_bits(scene_var_amount)) {
                        values_amount_in_variable = (uint32_t)_scene.c_str()[i + 2];
                    }
                    else if(keyword == make_bits(scene_var_float)) {
                        m_scene_structs[m_scene_structs.size() - 1].m_scene_struct[m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() - 1].m_values_type = v_float;
                    }
                    else if(keyword == make_bits(scene_var_double)) {
                        m_scene_structs[m_scene_structs.size() - 1].m_scene_struct[m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() - 1].m_values_type = v_double;
                    }
                    else if(keyword == make_bits(scene_var_int)) {
                        m_scene_structs[m_scene_structs.size() - 1].m_scene_struct[m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() - 1].m_values_type = v_int;
                    }
                    else if(keyword == make_bits(scene_var_uint)) {
                        m_scene_structs[m_scene_structs.size() - 1].m_scene_struct[m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() - 1].m_values_type = v_uint;
                    }
                    else if(keyword == make_bits(scene_var_char)) {
                        m_scene_structs[m_scene_structs.size() - 1].m_scene_struct[m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() - 1].m_values_type = v_char;
                    }
                    else if(keyword == make_bits(scene_var_string)) {
                        m_scene_structs[m_scene_structs.size() - 1].m_scene_struct[m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() - 1].m_values_type = v_string;
                    }
                    else if(keyword == make_bits(scene_var_bool)) {
                        m_scene_structs[m_scene_structs.size() - 1].m_scene_struct[m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() - 1].m_values_type = v_bool;
                    }
                    else if(keyword == make_bits(scene_var_value_separator)) {
                        if(!value_to_convert_recording && keyword_count > 1) {
                            m_scene_structs[m_scene_structs.size() - 1].m_scene_struct[m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() - 1].add_value(value_to_convert);
                            float val = m_scene_structs[m_scene_structs.size() - 1].m_scene_struct[m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() - 1].getElement(m_scene_structs[m_scene_structs.size() - 1].m_scene_struct[m_scene_structs[m_scene_structs.size() - 1].m_scene_struct.size() - 1].m_scene_values_size);

                            std::cout << "Pushed value " << val << " Value " << value_to_convert.c_str() << std::endl;
                        }

                        current_value++;

                        if(current_value < values_amount_in_variable && keyword_count > 1) {
                            value_to_convert += _scene.c_str()[i + 2];

                            value_to_convert_recording = true;
                        }
                    }
                    else if(keyword == make_bits(scene_var_beg_value)) {
                        current_value = 0;

                        if(current_value < values_amount_in_variable && keyword_count > 1) {
                            value_to_convert += _scene.c_str()[i + 2];

                            value_to_convert_recording = true;
                        }
                    }
                    else if(keyword == make_bits(scene_struct_beg)) {
                        m_scene_structs.push_back(SceneValuesStruct());
                        keyword_buff[0] = keyword_buff[1] = (char)0;
                    }
                    else if(keyword == make_bits(scene_struct_name_beg)) {
                        if(_scene.c_str()[i + 2] != (char)scene_separator) {
                            m_scene_structs[m_scene_structs.size() - 1].m_struct_name += _scene.c_str()[i + 2];
                        }
                    }
                }
            }
        }

        std::string getSceneData() { return m_scene_data; }

        std::string getSceneName() { return m_scene_name; }

        std::vector<SceneValuesStruct>* getSceneValues() { return &m_scene_structs; }

        void setSceneData(std::string const &scene_data) { m_scene_data = scene_data; }

        void addSceneData(std::string const &partial_scene_data) { m_scene_data += partial_scene_data; }
    };
}

#endif 
