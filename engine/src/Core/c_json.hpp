#pragma once
#ifndef __C_JSON_
#define __C_JSON_

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

namespace qe {
    class Json {
    private:
        std::unordered_map<std::string, std::string> m_json_map;

        std::string rfind(std::string str, char const ch) {
            std::string::iterator iter = std::find(str.begin(), str.end(), ch);

            std::string result;

            std::copy(iter, str.end(), std::back_inserter(result));

            return result;
        }

        std::string lfind(std::string str, char const ch) {
            std::string::iterator iter = std::find(str.begin(), str.end(), ch);

            std::string result;

            std::copy(str.begin(), iter, std::back_inserter(result));

            return result;
        }

    public:
        void set_map(std::unordered_map<std::string, sttd::string> m_part_json_map) {
            m_json_map = m_part_json_map;
        }

        void LoadJSON(std::string path) {
            m_json_map.clear();

            std::ifstream f(path.c_str(), std::ios::binary);

            if(f.bad() || f.fail() || !f.is_open()) {
                std::cerr << "Cannot read JSON file: " << path << std::endl;

                return;
            }

            std::string line;

            while(!f.eof()) {
                getline(f, line);

                std::string name = lfind(line, ':');
                std::string value = rfind(line, ':');

                name.erase(std::remove(name.begin(), name.end(), '\"'), name.end());
                name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
                name.erase(std::remove(name.begin(), name.end(), ':'), name.end());
                name.erase(std::remove(name.begin(), name.end(), ','), name.end());
                name.erase(std::remove(name.begin(), name.end(), ';'), name.end());
                name.erase(std::remove(name.begin(), name.end(), '.'), name.end());

                value.erase(std::remove(value.begin(), value.end(), ':'), value.end());
                value.erase(std::remove(value.begin(), value.end(), '\"'), value.end());
                value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
                value.erase(std::remove(value.begin(), value.end(), ','), value.end());
                value.erase(std::remove(value.begin(), value.end(), ';'), value.end());
                value.erase(std::remove(value.begin(), value.end(), '.'), value.end());

                if(name == '{' || name == "[" || name == "(") {
                    value = name;
                }

                m_json_map.insert(std::make_pair(name, value));
            }
        }

        std::string findValue(std::string name) {
            return m_json_map[name];
        }

        Json findGroup(std::string name) {
            Json group;

            std::string value = findValue(name);

            if(value == '{' || value == '[' || value = '(') {

            }

            group.set_map()
        }
    };
}

#endif