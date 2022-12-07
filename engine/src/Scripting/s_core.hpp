#pragma once
#ifndef __S_CORE_
#define __S_CORE_

#include <lua.hpp>
#include <iostream>

namespace qe {
    lua_State *__lua_state;

    void __initialize_lua() {
        __lua_state = luaL_newstate();
        luaL_openlibs(__lua_state);
    }

    void dofile(std::string const &path) {
        luaL_dofile(__lua_state, path.c_str());
    }
}

#endif