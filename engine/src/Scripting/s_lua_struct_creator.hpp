#pragma once
#ifndef __S_LUA_STRUCT_CREATOR_
#define __S_LUA_STRUCT_CREATOR_

#include "s_core.hpp"
#include <vector>
#include <string>

namespace qe {
    enum ScriptPassValueType {
        s_double,
        s_integer,
        s_bool,
        s_string,
    };

    class ScriptLuaStructCreator {
        std::vector<std::string> m_names;
        std::vector<ScriptPassValueType> m_pass_value;
        int64_t m_field_counter = -1;
        std::string m_metatable;
        
    public:
        ScriptLuaStructCreator(std::string const &metatable_name) {
            m_metatable = metatable_name;

            lua_newtable(__lua_state);
            lua_newtable(__lua_fixed_state);

            luaL_setmetatable(__lua_state, m_metatable.c_str());
            luaL_setmetatable(__lua_fixed_state, m_metatable.c_str());
        }

        template<typename T>
        void push_name_with_value(std::string name, ScriptPassValueType type, T *value) {
            m_names.push_back(name);
            m_pass_value.push_back(type);

            luaL_getmetatable(__lua_state, -1);
            luaL_getmetatable(__lua_fixed_state, -1);

            lua_pushinteger(__lua_state, val);
            lua_pushinteger(__lua_fixed_state, val);

            switch(type) {
            case s_double:
                lua_pushnumber(__lua_state, &value);
                lua_pushnumber(__lua_fixed_state, &value);

                break;

            case s_integer:
                lua_pushinteger(__lua_state, &value);
                lua_pushinteger(__lua_fixed_state, &value);

                break;

            case s_bool:
                lua_pushboolean(__lua_state, &value);
                lua_pushboolean(__lua_fixed_state, &value);

                break;

            case s_string:
                lua_pushstring(__lua_state, value);
                lua_pushstring(__lua_fixed_state, value);

                lua_

                break;
            }

            lua_setfield(__lua_state, m_field_counter, m_names[(m_field_counter * -1) - 1].c_str());
            lua_setfield(__lua_fixed_state, m_field_counter, m_names[(m_field_counter * -1) - 1].c_str());

            m_field_counter++;
        }
    };
}

#endif