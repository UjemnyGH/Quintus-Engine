#pragma once
#ifndef __P_RIGIDBODY_
#define __P_RIGIDBODY_

#include <glm/glm.hpp>

namespace qe {
    float g_gravity = 10.0f;

    class Rigidbody {
    private:
        

    public:
        float m_velocity = 0.0f;
        glm::vec3 m_direction = glm::vec3(0.0f);
        glm::vec3 m_position = glm::vec3(0.0f);
        float m_friction = 1.0f;
        float m_friction_coefficient = 1.0f;
        float m_angular_friction = 1.0f;
        float m_bouyency = 0.0f;
        float m_mass = 1.0f;
        float m_force = 0.0f;
        float m_down_force = 0.0f;
        float m_acceleration = 0.0f;
        bool m_use_gravity = true;

        Rigidbody() {}

        void AddForce(float force) {
            m_velocity += force;
        }
    };
}

#endif