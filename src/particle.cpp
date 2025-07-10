#include "../include/particle.hpp"

vpe::Particle::Particle(ParticleType type_, sf::Vector2f pos_, float radius_, float charge_, float mass_)
    : PhysicsObjectVerlet(pos_, radius_)
    , type{type_}
    , charge{charge_}
    , mass{mass_}
    {
        switch (type_)
        {
        case ParticleType::ELECTRON:
            color = sf::Color::Blue;
            break;
        case ParticleType::PROTON:
            color = sf::Color::Red; 
            break;
        case ParticleType::NEUTRON:
            color = sf::Color::Green; 
            break;
        default:
            break;
        }
    }
        