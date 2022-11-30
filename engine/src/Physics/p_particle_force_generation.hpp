#pragma once
#ifndef __P_PARTICLE_FORCE_GENERATOR_
#define __P_PARTICLE_FORCE_GENERATOR_

#include "p_particle.hpp"

namespace qe {
    class ParticleForceGenerator {
    public:
        virtual void updateForce(Particle* particle, real duration) {}
    };

    class ParticleForceRegistry {
    protected:
        struct ParticleForceRegistration
        {
            Particle *m_particle;
            ParticleForceGenerator *m_force_gen;
        };

        std::vector<ParticleForceRegistration> m_registrations;

    public:
        void Add(Particle* particle, ParticleForceGenerator* force_generator) {
            m_registrations.push_back({particle, force_generator});
        }

        /*void Remove(Particle *particle, ParticleForceGenerator *force_generator) {

            ParticleForceRegistration reg = {particle, force_generator};

            uint32_t iter = Search(m_registrations, reg);
            
            m_registrations.erase(m_registrations.begin() + iter);
        }*/

        void Clear() {
            m_registrations.clear();
        }

        void UpdateForces(real duration) {
            for(std::vector<ParticleForceRegistration>::iterator i = m_registrations.begin(); i != m_registrations.end(); i++) {
                i->m_force_gen->updateForce(i->m_particle, duration);
            }
        }
    };

    class ParticleGravity : public ParticleForceGenerator {
    private:
        Vector<real> m_gravity;

    public:
        ParticleGravity(Vector<real> const &gravity) : m_gravity(gravity) {}

        virtual void updateForce(Particle *particle, real duration) override {
            if(!particle->hasFiniteMass()) return;

            particle->addForce(m_gravity);
        }
    };

    class ParticleDrag : public ParticleForceGenerator {
    private:
        real m_k1;

        real m_k2;

    public:
        ParticleDrag(real k1, real k2) : m_k1(k1), m_k2(k2) {}

        virtual void updateForce(Particle *particle, real duration) override {
            Vector<real> force = particle->m_velocity;

            real dragCoefficient = force.length();
            dragCoefficient = m_k1 * dragCoefficient + m_k2 * dragCoefficient * dragCoefficient;

            force = force.normalize();

            force *= -dragCoefficient;

            particle->addForce(force);
        }
    };
}

#endif