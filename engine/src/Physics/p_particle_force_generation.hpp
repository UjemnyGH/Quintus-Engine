#pragma once
#ifndef __P_PARTICLE_FORCE_GENERATOR_
#define __P_PARTICLE_FORCE_GENERATOR_

#include "p_particle.hpp"

namespace qe {
    class ParticleForceGenerator {
    public:
        virtual void updateForce(Particle* particle, real duration) {}
    };

    struct ParticleForceRegistration
    {
        Particle *m_particle;
        ParticleForceGenerator *m_force_gen;

        bool operator==(ParticleForceRegistration const &p) { return p.m_particle == m_particle && p.m_force_gen == m_force_gen; }
    };

    class ParticleForceRegistry {
    protected:
        std::vector<ParticleForceRegistration> m_registrations;

    public:
        void Add(Particle* particle, ParticleForceGenerator* force_generator) {
            m_registrations.push_back({particle, force_generator});
        }

        void Remove(Particle *particle, ParticleForceGenerator *force_generator) {
            ParticleForceRegistration pfr = {particle, force_generator};

            auto iter = std::find(m_registrations.begin(), m_registrations.end(), pfr);
            
            m_registrations.erase(iter);
        }

        void Clear() {
            m_registrations.clear();

            if(!m_registrations.empty()) {
                qe::qe_warn("Particle Force Registry: m_registrations cleard incorrectly!!");
            }
        }

        void UpdateForces(real duration) {
            if(m_registrations.end() != m_registrations.begin() + m_registrations.size()) {
                qe::qe_warn("Particle Force Registry: sizes are uncorrect!");
            }

            for(size_t i = 0; i < m_registrations.size(); i++) {
                m_registrations[i].m_force_gen->updateForce(m_registrations[i].m_particle, duration);
            }
        }
    };

    class ParticleGravity : public ParticleForceGenerator {
    private:
        Vector<real> m_gravity;

    public:
        ParticleGravity(Vector<real> const &gravity) : m_gravity(gravity) {}

        void SetData(Vector<real> const &gravity) {
            m_gravity = gravity;
        }

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

        void SetData(real k1, real k2) {
            m_k1 = k1;
            m_k2 = k2; 
        }

        virtual void updateForce(Particle *particle, real duration) override {
            Vector<real> force = particle->m_velocity;

            if(force != Vector<float>(0.0f, 0.0f, 0.0f)) {
                
                real dragCoefficient = force.length();
                dragCoefficient = m_k1 * dragCoefficient + m_k2 * dragCoefficient * dragCoefficient;

                force = force.normalize();

                force *= -dragCoefficient;

                particle->addForce(force);
            }
        }
    };

    class ParticleDumper : public ParticleForceGenerator {
    private:
        real m_dump_value;
        Vector<real> m_last_velocity;

    public:
        ParticleDumper(real dump_value) : m_dump_value(dump_value) {}

        void SetData(real dump_value) {
            m_dump_value = dump_value;
        }

        virtual void updateForce(Particle *particle, real duration) {
            Vector<float> force = particle->m_velocity;

            force = force.invert() * m_dump_value;

            particle->addForce(force);
        }
    };

    class ParticleDragFluid : public ParticleForceGenerator {
    private:
        real m_area;
        real m_mass_density;
        real m_drag_coefficient;

    public:
        ParticleDragFluid(real area, real mass_density = (real)16, real drag_coefficient = (real)343.0) : m_area(area), m_mass_density(mass_density), m_drag_coefficient(drag_coefficient) {}

        void SetData(real area, real mass_density = (real)16, real drag_coefficient = (real)343.0) { 
            m_area = area;
            m_mass_density = mass_density;
            m_drag_coefficient = drag_coefficient;
        }

        virtual void updateForce(Particle *particle, real duration) override {
            Vector<real> force = particle->m_velocity;

            if(force != Vector<real>(0.0, 0.0, 0.0)) {
                Vector<real> drag = (force * force * m_mass_density * (force / m_drag_coefficient) * m_area) / (real)2.0;

                force.normalize();
                force *= drag.invert();

                particle->addForce(force);
            }
        }
    };

    class ParticleSpring : public ParticleForceGenerator {
    private:
        Particle *m_other;
        real m_spring_constant;
        real m_rest_length;

    public:
        ParticleSpring() = default;

        ParticleSpring(Particle *other, real spring_constant, real rest_length) : m_other(other), m_spring_constant(spring_constant), m_rest_length(rest_length) {}

        void SetData(Particle *other, real spring_constant, real rest_length) {
            m_other = other;
            m_spring_constant = spring_constant; 
            m_rest_length = rest_length;
        }

        virtual void updateForce(Particle *particle, real duration) override {
            Vector<real> force = particle->m_position;

            force -= m_other->m_position;

            real length = force.length();
            length = abs(length - m_rest_length);
            length *= m_spring_constant;

            force.normalize();
            force *= -length;

            particle->addForce(force - ((particle->m_velocity - m_other->m_velocity) * (1.0 - duration)));
        }
    };

    class ParticleAnchoredSpring : public ParticleForceGenerator {
    private:
        Vector<real> m_anchor;
        real m_spring_constant;
        real m_rest_length;

    public:
        ParticleAnchoredSpring(Vector<real> anchor, real spring_constant, real rest_length) : m_anchor(anchor), m_spring_constant(spring_constant), m_rest_length(rest_length) {}

        void SetData(Vector<real> anchor, real spring_constant, real rest_length) {
            m_anchor = anchor;
            m_spring_constant = spring_constant; 
            m_rest_length = rest_length;
        }

        virtual void updateForce(Particle *particle, real duration) override {
            Vector<real> force = particle->m_position;

            force -= m_anchor;

            real length = force.length();
            length = abs(length - m_rest_length);
            length *= m_spring_constant;

            force.normalize();
            force *= -length;

            particle->addForce(force - (particle->m_velocity * (1.0 - duration)));
        }
    };

    class ParticleBungee : public ParticleForceGenerator {
    private:
        Particle *m_other;
        real m_spring_constant;
        real m_rest_length;

    public:
        ParticleBungee(Particle *other, real spring_constant, real rest_length) : m_other(other), m_spring_constant(spring_constant), m_rest_length(rest_length) {}

        void SetData(Particle *other, real spring_constant, real rest_length) {
            m_other = other;
            m_spring_constant = spring_constant; 
            m_rest_length = rest_length;
        }

        virtual void updateForce(Particle *particle, real duration) override {
            Vector<real> force = particle->m_position;

            force -= m_other->m_position;

            real length = force.length();
            if(length <= m_rest_length) {
                return;
            }

            length = m_spring_constant * abs(m_rest_length - length);

            force.normalize();
            force *= -length;

            particle->addForce(force - ((particle->m_velocity - m_other->m_velocity) * (1.0 - duration)));
        }
    };

    class ParticleBuoyancy : public ParticleForceGenerator {
    private:
        real m_max_depth;
        real m_volume;
        real m_water_height;
        real m_liquid_density;
    
    public:
        ParticleBuoyancy(real max_depth, real volume, real water_height, real liquid_density = 1000.0f) : m_max_depth(max_depth), m_volume(volume), m_water_height(water_height), m_liquid_density(liquid_density) {}

        void SetData(real max_depth, real volume, real water_height, real liquid_density = 1000.0f) {
            m_max_depth = max_depth;
            m_volume = volume;
            m_water_height = water_height;
            m_liquid_density = liquid_density; 
        }

        virtual void updateForce(Particle *particle, real duration) override {
            real depth = particle->m_position.y;

            if(depth >= m_water_height + m_max_depth) {
                return;
            }

            Vector<real> force;

            if(depth <= m_water_height - m_max_depth) {
                force.y = m_liquid_density * m_volume;
                particle->addForce(force);

                return;
            }

            force.y = m_liquid_density * m_volume * (depth - m_max_depth - m_water_height) / 2 * m_max_depth;

            particle->addForce(force - (particle->m_velocity * (1.0 - duration)));
        }
    };
}

#endif