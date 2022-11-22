#pragma once
#ifndef __P_RIGIDBODY_
#define __P_RIGIDBODY_

#include <glm/glm.hpp>

namespace qe {
    float g_gravity = 10.0f;

    class QE_Rigidbody {
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

        QE_Rigidbody() {}

        void AddForce(float force) {
            m_velocity += force;
        }

        void BasicCalculations(float delta_time) {
            m_acceleration = m_velocity / delta_time;
            m_force = m_acceleration * m_mass;
            m_friction = m_friction_coefficient * m_force;

            m_velocity -= m_friction;
            m_position.y -= m_down_force;

            if(m_use_gravity) {
                m_down_force = g_gravity * m_mass;
            }
            else {
                m_down_force = 0.0f;
            }
        }
    };
}

#endif