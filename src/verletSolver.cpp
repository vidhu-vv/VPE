#include "../include/verletSolver.hpp"
#include "../include/pObj.hpp"

vpe::SolverVerlet::SolverVerlet() {};

vpe::PhysicsObjectVerlet& vpe::SolverVerlet::addPhysicsObject(sf::Vector2f pos_, float radius_) {
    return objects.emplace_back(pos_, radius_);
}

void vpe::SolverVerlet::update() {
    time+=dt;
    const float substepDt = getStepDt();
    for(uint32_t i{substeps}; --i;) {
        applyGravity();
        updateObjects(substepDt);
        applyConstraints();
        checkCollisions(substepDt);
    }
}
void vpe::SolverVerlet::setSimUpdateRate(uint32_t rate) {
    dt = 1.0f / static_cast<float>(rate);
}

void vpe::SolverVerlet::setConstraint(sf::Vector2f pos_, float radius_) {
    constraint = {pos_.x, pos_.y, radius_};
}

void vpe::SolverVerlet::setSubstepCount(uint32_t count) {
    substeps = count;
}

void vpe::SolverVerlet::setObjectVelocity(PhysicsObjectVerlet& obj, sf::Vector2f velocity_) {
    obj.setVelocity(velocity_, getStepDt());
}

const std::vector<vpe::PhysicsObjectVerlet>& vpe::SolverVerlet::getObjects() const {
    return objects;
}

sf::Vector3f vpe::SolverVerlet::getConstraint() const {
    return constraint;
}

uint64_t vpe::SolverVerlet::getObjectCount() const {
    return objects.size();
}

float vpe::SolverVerlet::getTime() const {
    return time;
}

float vpe::SolverVerlet::getStepDt() const {
    return dt / static_cast<float>(substeps);
}

void vpe::SolverVerlet::applyGravity() {
    for(auto& obj : objects) {
        obj.accelerate(gravity);
    }
}

void vpe::SolverVerlet::checkCollisions(float dt_) {
    const float response_coef = 0.75f;
    const uint64_t obj_count = objects.size();
    for(uint64_t i{0}; i < obj_count; ++i) {
        PhysicsObjectVerlet& obj_a = objects[i];
        for(uint64_t j{i + 1}; j < obj_count; ++j) {
            PhysicsObjectVerlet& obj_b = objects[j];
            const sf::Vector2f delta = obj_b.position - obj_a.position;
            const float dist2 = delta.x * delta.x + delta.y * delta.y;
            const float radius_sum = obj_a.radius + obj_b.radius;
            if(dist2 < radius_sum * radius_sum) {
                const float dist = std::sqrt(dist2);
                const sf::Vector2f n = delta/dist;
                const float mass_ratio_a = obj_a.radius / radius_sum;
                const float mass_ratio_b = obj_b.radius / radius_sum;
                const float penetration = response_coef * 0.5f * (dist - radius_sum);
                obj_a.position += n * penetration * mass_ratio_b;
                obj_b.position -= n * penetration * mass_ratio_a;
            }
        }
    }
}

void vpe::SolverVerlet::applyConstraints() {
    for(auto& obj : objects) {
        const sf::Vector2f delta = sf::Vector2f{constraint.x, constraint.y} - obj.position;
        const float dist2 = delta.x * delta.x + delta.y * delta.y;
        const float radius_diff = constraint.z - obj.radius;
        if(dist2 > radius_diff * radius_diff) {
            const float dist = std::sqrt(dist2);
            const sf::Vector2f n = delta/dist;
            obj.position = sf::Vector2f{constraint.x, constraint.y} - n * (constraint.z - obj.radius);

        }
    }
}

void vpe::SolverVerlet::updateObjects(float dt_) {
    for(auto& obj : objects) {
        obj.update(dt_);
    }
}



