#pragma once

#ifndef VV_VERLET_PHYSICS_SOLVER
#define VV_VERLET_PHYSICS_SOLVER
#include <SFML/Graphics.hpp>
#include <optional>
#include "pObj.hpp"
#include <cstdlib>
namespace vpe {
    class SolverVerlet {
        public:
            SolverVerlet();
            PhysicsObjectVerlet& addPhysicsObject(sf::Vector2f pos_, float radius_);

            void update();
            void setSimUpdateRate(uint32_t rate);
            void setConstraint(sf::Vector2f pos_, float radius_);
            void setSubstepCount(uint32_t count);
            void setObjectVelocity(PhysicsObjectVerlet& obj, sf::Vector2f velocity_);
            [[nodiscard]]
            const std::vector<PhysicsObjectVerlet>& getObjects() const;
            [[nodiscard]]
            sf::Vector3f getConstraint() const;
            [[nodiscard]]
            uint64_t getObjectCount() const;
            [[nodiscard]]
            float getTime() const;
            [[nodiscard]]
            float getStepDt() const;
        private:
            uint32_t substeps = 1;
            sf::Vector2f gravity = {0.0f, 1000.0f};
            sf::Vector3f constraint = {0.0f, 0.0f, 0.0f}; 
            std::vector<PhysicsObjectVerlet> objects;
            float time = 0.0f;
            float dt = 0.0f;

            void applyGravity();
            void checkCollisions(float dt_);
            void applyConstraints();
            void updateObjects(float dt_);
    };
}

#endif // VV_VERLET_PHYSICS_SOLVER