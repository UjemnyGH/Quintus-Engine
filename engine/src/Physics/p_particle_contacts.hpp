#pragma once
#ifndef __P_PARTICLE_CONTACTS_
#define __P_PARTICLE_CONTACTS_

#include "../Math/m_math_core.hpp"
#include "p_particle.hpp" 

namespace qe {
    class ParticleContact {
    public:
        Particle *m_particles[2];
        real m_restitution;
        Vector<real> m_contact_normal;
        real m_penetration;

    private:
        void _resolve_velocity(real duration) {
            real separating_velocity = CalculateSeparatingVelocity();

            if(separating_velocity > 0) {
                return;
            }

            real next_separating_velocity = -separating_velocity * m_restitution;

            Vector<real> accceleration_caused_velocity = m_particles[0]->m_acceleration;

            if(m_particles[1]) {
                accceleration_caused_velocity -= m_particles[1]->m_acceleration;
            }

            real accceleration_caused_separating_velocity = accceleration_caused_velocity.dot(m_contact_normal) * duration;

            if(accceleration_caused_separating_velocity < 0) {
                next_separating_velocity += m_restitution * accceleration_caused_separating_velocity;
            }

            real delta_velocity = next_separating_velocity - separating_velocity;

            real total_inverse_mass = m_particles[0]->getInverseMass();

            if(m_particles[1]) {
                total_inverse_mass += m_particles[1]->getInverseMass();
            }

            if(total_inverse_mass <= 0) {
                return;
            }

            real impulse = delta_velocity / total_inverse_mass;

            Vector<real> impulse_per_mass = m_contact_normal * impulse;

            m_particles[0]->m_velocity = m_particles[0]->m_velocity + impulse_per_mass * m_particles[0]->getInverseMass();

            if(m_particles[1]) {
                m_particles[1]->m_velocity = m_particles[1]->m_velocity + impulse_per_mass * -m_particles[1]->getInverseMass();
            }
        }

        void _resolve_interpenetration(real duration) {
            if(m_penetration <= 0) {
                return;
            }

            real total_mass_inverse = m_particles[0]->getInverseMass();

            if(m_particles[1]) {
                total_mass_inverse += m_particles[1]->getInverseMass();
            }

            if(total_mass_inverse <= 0) {
                return;
            }

            Vector<real> move_per_mass = m_contact_normal * (-m_penetration / total_mass_inverse);

            m_particles[0]->m_position = m_particles[0]->m_position * move_per_mass * m_particles[0]->getInverseMass();

            if(m_particles[1]) {
                m_particles[1]->m_position = m_particles[1]->m_position * move_per_mass * m_particles[1]->getInverseMass();
            }
        }

    protected:
        void Resolve(real duration) {
            _resolve_velocity(duration);
            _resolve_interpenetration(duration);
        }

        real CalculateSeparatingVelocity() const {
            Vector<real> relative_velocity = m_particles[0]->m_velocity;

            if(m_particles[1]) {
                relative_velocity -= m_particles[1]->m_velocity;
            }

            return relative_velocity.dot(m_contact_normal);
        }
    };
}

#endif