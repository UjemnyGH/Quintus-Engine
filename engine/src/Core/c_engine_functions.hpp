#pragma once
#ifndef __C_ENGINE_FUNCTIONS_
#define __C_ENGINE_FUNCTIONS_

#include <iostream>

namespace qe
{
    void qe_term(std::string msg) {
        std::cerr << msg << std::endl;

        exit(msg.size());
    }    
}

#endif