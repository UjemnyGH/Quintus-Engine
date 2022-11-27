#pragma once
#ifndef __P_RAYCAST_
#define __P_RAYCAST_

#include "../Core/c_algorithms.hpp"
#include "p_collider.hpp"

namespace qe {
    struct Ray {
        Vector<float> m_ray_start;
        Vector<float> m_ray_end;
        float m_ray_length;
    };

    class Raycast2D {
    private:
        Ray *m_ray;
        std::unordered_map<std::string, Collider>* m_collider_map_ptr;
    
    public:
        Raycast2D() {}

        void SetRay(Ray *ray) {
            m_ray = ray; 
        }

        Ray *GetRayPtr() { return m_ray; }

        Vector<float> GetRayIntersectionPosition() {
            
        }
    };
}

#endif