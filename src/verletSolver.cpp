#include "../include/verletSolver.hpp"
#include "../include/pObj.hpp"
#include "../include/particle.hpp"
#include "../include/utils.hpp"
#include <iostream>
#include <cmath>
vpe::SolverVerlet::SolverVerlet() {};

vpe::Particle& vpe::SolverVerlet::addPhysicsObject(sf::Vector2f pos_, float radius_, ParticleType type_, float charge_, float mass_) {
    return objects.emplace_back(type_, pos_, radius_, charge_, mass_);
}

void vpe::SolverVerlet::update() {
    time+=dt;
    const float substepDt = getStepDt();
    for(uint32_t i{substeps}; --i;) {
       applyForce();
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

void vpe::SolverVerlet::setObjectVelocity(Particle& obj, sf::Vector2f velocity_) {
    obj.setVelocity(velocity_, getStepDt());
}

const std::vector<vpe::Particle>& vpe::SolverVerlet::getObjects() const {
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

void vpe::SolverVerlet::applyForce() {
    const uint64_t obj_count = objects.size();
    const float nuclear_binding_radius = 60.0f;
    const float electric_force_cutoff = 20.0f;
    const float nuclear_binding_strength = 8000.0f;
    for(uint64_t i{0}; i < obj_count; ++i) {
        Particle& obj_a = objects[i];
        for(uint64_t j{i+1}; j < obj_count; ++j) {
            Particle& obj_b = objects[j];
            const sf::Vector2f delta = obj_b.position - obj_a.position;
            const float dist2 = std::max(delta.x * delta.x + delta.y * delta.y, electric_force_cutoff * electric_force_cutoff);
            const float dist2n = delta.x * delta.x + delta.y * delta.y;
            if(dist2 == 0.0f) {
                obj_a.position += sf::Vector2f{0.1f, 0.0f};
            }
            if((obj_a.type == ParticleType::PROTON && obj_b.type == ParticleType::NEUTRON) || (obj_a.type == ParticleType::NEUTRON && obj_b.type == ParticleType::PROTON) || (obj_a.type == ParticleType::PROTON && obj_b.type == ParticleType::PROTON)) {
                if(dist2n < nuclear_binding_radius * nuclear_binding_radius) {
                    const float dist = std::sqrt(dist2n);
                    const sf::Vector2f n = delta/dist;
                    const float force_magnitude = nuclear_binding_strength * (1.0f - dist / nuclear_binding_radius);
                    obj_a.accelerate(n * 100.0f * force_magnitude / obj_a.mass);
                    obj_b.accelerate(-n * 100.0f * force_magnitude / obj_b.mass);
                }
            }
                
            const sf::Vector2f n = delta / std::sqrt(dist2);
            const float force_magnitude = MATH::K_CONST * (obj_a.charge * obj_b.charge) / (dist2); 
            const sf::Vector2f force = n * force_magnitude;
            obj_a.accelerate(-force / obj_a.mass);
            obj_b.accelerate(force / obj_b.mass); 
        }
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
            if((dist2 < radius_sum * radius_sum )) {
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
    const float constraint_radius = constraint.z;
    const float buffer = 20.0f;
    const float repelling_strength = 1000.0f;
    for(auto& obj : objects) {
        const sf::Vector2f delta = sf::Vector2f{constraint.x, constraint.y} - obj.position;
        const float dist2 = delta.x * delta.x + delta.y * delta.y;
        const float radius_diff = constraint.z - obj.radius;
        if(dist2 > (radius_diff - buffer) * (radius_diff - buffer)) {
            const float dist = std::sqrt(dist2);
            const sf::Vector2f n = delta/dist;
            obj.accelerate(n * repelling_strength * std::cbrt(obj.mass));
        }
        if(dist2 > radius_diff * radius_diff) {
            const float dist = std::sqrt(dist2);
            const sf::Vector2f n = delta/dist;
            obj.position = sf::Vector2f{constraint.x, constraint.y} - n * (constraint.z - obj.radius);

        }
    }
}

void vpe::SolverVerlet::updateObjects(float dt_) {
    const float damping = 0.999f;
    for(auto& obj : objects) {
        obj.update(dt_);
        obj.setVelocity(obj.getVelocity(dt_) * damping, dt_);
    }
}



