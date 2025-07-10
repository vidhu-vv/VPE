#ifndef VV_PARTICLE
#define VV_PARTICLE
#pragma once
#include <SFML/Graphics.hpp>
#include "pObj.hpp"

namespace vpe {
    enum class ParticleType {
        ELECTRON,
        PROTON,
        NEUTRON,
    };
    class Particle : public PhysicsObjectVerlet {
        public:
            ParticleType type;
            float charge;
            float mass;

            Particle(ParticleType type_, sf::Vector2f pos_, float radius_, float charge_, float mass_);
    };
}
#endif // VV_PARTICLE