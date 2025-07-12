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

            [[nodiscard]]
            ParticleType getType() const { return type; }
            [[nodiscard]]
            float getCharge() const { return charge; }
            [[nodiscard]]
            float getMass() const { return mass; }
            [[nodiscard]]
            float getRadius() const { return radius; }
    };
}
#endif // VV_PARTICLE