#pragma once
#ifndef __C_TIME_
#define __C_TIME

#include <GLFW/glfw3.h>
#include "c_layers.hpp"

namespace qe
{
    class QE_Time : public Layer {
    private:
        static float last, current, delta;
    
    public:
        virtual void Start() override { QE_Time::last = 0.0f; QE_Time::current = 0.0f; QE_Time::delta = 0.0f; }
        virtual void Update() override { QE_Time::last = QE_Time::current; QE_Time::current = glfwGetTime(); QE_Time::delta = QE_Time::current - QE_Time::last; }
        
        static float GetTime() { return current; }
        static float GetDeltaTime() { return delta; }
    };

    typedef QE_Time Time;
} // namespace qe


#endif