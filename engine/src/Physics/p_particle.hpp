#pragma once
#ifndef __P_PARTICLE_

#include "../Core/c_algorithms.hpp"
#include "p_physics_definitions.hpp"
#include <limits>

namespace qe {
    class Particle {
    protected:
        real m_inverse_mass = 1.0;
        real m_create_time;
        real m_delete_time = 10.0;
        bool m_delete_on_time = false;

    public:
        Vector<real> m_position = Vector<real>((real)0.0);
        Vector<real> m_velocity = Vector<real>((real)0.0);
        Vector<real> m_acceleration = Vector<real>((real)0.0);
        Vector<real> m_force = Vector<real>((real)0.0);

        real m_particle_damping = 0.999998;
    
        void setInverseMass(real inverse_mass) { m_inverse_mass = inverse_mass; }

        void setMass(real mass) { m_inverse_mass = 1.0 / mass; }

        real getMass() { return 1.0 / m_inverse_mass; }

        void integrate(real duration) {
            m_position += m_velocity * duration;

            Vector<real> result_acceleration = m_acceleration;
            result_acceleration += m_force * m_inverse_mass;

            m_velocity += result_acceleration * duration;

            m_velocity *= m_particle_damping;

            m_force = (real)0.0;
        }

        bool hasFiniteMass() { return m_inverse_mass >= 0.0f; }

        void addForce(Vector<real> force) {
            m_force += force;
        }

        void deleteOnTime(bool state = false) { m_delete_on_time = state; }

        void setDeleteTime(real create_time, real delete_time = (real)10.0) {
            m_create_time = create_time;
            m_delete_time = delete_time;
        }

        bool deleteParticle(real current_time) { return m_create_time + m_delete_time < current_time && m_delete_on_time; }
    };
}

#endif