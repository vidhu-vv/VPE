#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "verletSolver.hpp"
#include <cstdlib>

#ifndef VV_PHYSICS_OBJECT_RENDERER
#define VV_PHYSICS_OBJECT_RENDERER
namespace vpe {
    class Renderer {
        public:
        explicit Renderer(sf::RenderTarget& target_);
        void render(const SolverVerlet& solver);
        private:
        sf::RenderTarget& target;
    };
}

#endif // VV_PHYSICS_OBJECT_RENDERER