#include <SFML/Graphics.hpp>
#include "../include/verletSolver.hpp"
#include "../include/renderer.hpp"
#include <cstdint>
#include "../include/pObj.hpp"
#include "../include/particle.hpp"
#include "../include/utils.hpp"
#include <optional>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
    window.setFramerateLimit(60);
    vpe::SolverVerlet solver;
    vpe::Renderer renderer{window};

    solver.setConstraint({400.0f, 300.0f}, 300.0f);
    solver.setSimUpdateRate(60);
    solver.setSubstepCount(8);

    const float object_spawn_delay = 0.5f;
    const float object_spawn_speed = 600.0f;
    const sf::Vector2f object_spawn_pos = {400.0f, 50.0f};
    const sf::Vector2f min_max_radius = {5.0f, 20.0f};
    const uint32_t max_objects = 256;
    const float max_angle = 1.0f;

    sf::Clock clock;


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if(solver.getObjectCount() < max_objects && clock.getElapsedTime().asSeconds() >= object_spawn_delay) {
            clock.restart();
            const float t = solver.getTime();
            vpe::Particle& obj = solver.addPhysicsObject(object_spawn_pos, RNG<float>::getRange(min_max_radius.x, min_max_radius.y), vpe::ParticleType::ELECTRON, -1.0f, 1.0f);
            const float angle  = max_angle * sin(t) + 3.14159265358f * 0.5f;
            solver.setObjectVelocity(obj, object_spawn_speed * sf::Vector2f{cos(angle), sin(angle)});
        }
        solver.update();
        window.clear(sf::Color::White);
        renderer.render(solver);
        window.display();
    }
    return 0;
}