#pragma once
#ifndef __P_PARTICLE_

#include "../Core/c_algorithms.hpp"
#include "p_physics_definitions.hpp"
#include <limits>

namespace qe {
    class Particle {
    protected:
        real m_inverse_mass = 1.0;

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

        void addForce(Vector<real> force) {
            m_force += force;
        }
    };
}

#endif