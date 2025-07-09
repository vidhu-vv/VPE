#include "../include/pObj.hpp"

vpe::PhysicsObjectVerlet::PhysicsObjectVerlet() {};
vpe::PhysicsObjectVerlet::PhysicsObjectVerlet(sf::Vector2f pos_, float radius_)
 : position{pos_}
 , radius{radius_}
 , previousPosition{pos_}
 , acceleration{0.0f, 0.0f}
{};

void vpe::PhysicsObjectVerlet::update(float dt) {
    const sf::Vector2f displacement = position - previousPosition;
    previousPosition = position;
    position += displacement + acceleration * dt * dt;
    acceleration = {};
};

void vpe::PhysicsObjectVerlet::accelerate(sf::Vector2f acc_) {
    acceleration += acc_;
};

void vpe::PhysicsObjectVerlet::setVelocity(sf::Vector2f velocity_, float dt_) {
    previousPosition = position - velocity_ * dt_;
};

void vpe::PhysicsObjectVerlet::addVelocity(sf::Vector2f velocity_, float dt_) {
    previousPosition -= velocity_ * dt_;
};

sf::Vector2f vpe::PhysicsObjectVerlet::getVelocity(float dt_) const {
    return (position - previousPosition) / dt_;
};



