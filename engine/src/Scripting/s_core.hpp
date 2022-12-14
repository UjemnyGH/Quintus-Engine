#pragma once
#ifndef __S_CORE_
#define __S_CORE_

#include "../../vendor/lua/lua.h"
#include "../../vendor/lua/lualib.h"
#include "../../vendor/lua/lauxlib.h"
#include <iostream>
#include <fstream>
#include "../Core/c_layers.hpp"
#include "../Core/c_time.hpp"

namespace qe {
    const std::string __lua_extension__ = ".lua";

    lua_State *__lua_state;
    lua_State *__lua_fixed_state;

    void __initialize_lua() {
        if(__lua_state == nullptr) {
            __lua_state = luaL_newstate();
            __lua_fixed_state = luaL_newstate();
            luaL_openlibs(__lua_state);
            luaL_openlibs(__lua_fixed_state);
        }
    }

    void __dofile(std::string const &path) {
        luaL_dofile(__lua_state, path.c_str());
        luaL_dofile(__lua_fixed_state, path.c_str());
    }

    class ScriptLua : public Layer {
    private:
        std::string m_script_name = "script";
        std::string m_class_name = "script";
        qe::Time* m_time_ptr;

    public:
        ScriptLua() {
            m_auto_start = true;
            m_explict_fixed_update_implementation = true;
            m_layer_debug_name = "Script";

            __initialize_lua();
        }

        void load_script(std::string file_name = "script", std::string class_name = "script") {
            m_auto_start = true;
            m_explict_fixed_update_implementation = true;
            m_layer_debug_name = "Script";

            m_script_name = file_name;
            m_class_name = class_name;

            std::fstream f;

            f.open(m_script_name + __lua_extension__);


            if(f.fail()) {
                std::string __file_contents_to_create = m_class_name + " = {}\n\nreturn " + m_class_name;

                f.write(__file_contents_to_create.c_str(), __file_contents_to_create.size() - 1);
            }

            f.close();

            // FIXME: 
            luaL_dofile(__lua_state, std::string(m_script_name + __lua_extension__).c_str());
            luaL_dofile(__lua_fixed_state, std::string(m_script_name + __lua_extension__).c_str());
            lua_setglobal(__lua_state, m_class_name.c_str());
            lua_setglobal(__lua_fixed_state, m_class_name.c_str());
        }

        virtual void Start() override {
            lua_getglobal(__lua_state, m_class_name.c_str());
            lua_getfield(__lua_state, -1, "Start");
            if(lua_pcall(__lua_state, 0, 0, 0) != 0) {
                std::cerr << "LUA ERROR:\tFile: " << m_script_name << " Module name:" << m_class_name;
                //luaL_error(__lua_state, " Error in running %s\n", lua_tostring(__lua_state, -1));
            }
        }

        virtual void Update() override {
            lua_getglobal(__lua_state, m_class_name.c_str());
            lua_getfield(__lua_state, -1, "Update");
            if(lua_pcall(__lua_state, 0, 0, 0) != 0) {
                std::cerr << "LUA ERROR:\tFile: " << m_script_name << " Module name:" << m_class_name;
                //luaL_error(__lua_state, " Error in running %s\n", lua_tostring(__lua_state, -1));
            }
        }

        virtual void FixedUpdateExplict() override {
            lua_getglobal(__lua_fixed_state, m_class_name.c_str());
            lua_getfield(__lua_fixed_state, -1, "FixedUpdate");
            if(lua_pcall(__lua_fixed_state, 0, 0, 0) != 0) {
                std::cerr << "LUA ERROR:\tFile: " << m_script_name << " Module name:" << m_class_name;
                //luaL_error(__lua_state, " Error in running %s\n", lua_tostring(__lua_state, -1));
            }
        }

        virtual void End() {
            lua_settop(__lua_state, 0);
            lua_settop(__lua_fixed_state, 0);
        }
    };
}

#endif