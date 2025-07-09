#ifndef VV_PHYSICS_OBJECT_VERLET
#define VV_PHYSICS_OBJECT_VERLET
#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

namespace vpe {
    class PhysicsObjectVerlet {
    public:
        sf::Vector2f position;
        sf::Vector2f previousPosition;
        sf::Vector2f acceleration;
        float radius = 10.0f;
        sf::Color color = sf::Color::White;

        PhysicsObjectVerlet();
        PhysicsObjectVerlet(sf::Vector2f pos_, float radius_);

        void update(float dt);
        void accelerate(sf::Vector2f acc_);
        void setVelocity(sf::Vector2f velocity_, float dt_);
        void addVelocity(sf::Vector2f velocity_, float dt_);

        [[nodiscard]]
        sf::Vector2f getVelocity(float dt_) const;
    };
}
#endif // VV_PHYSICS_OBJECT_VERLET