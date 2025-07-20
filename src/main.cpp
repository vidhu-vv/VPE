#include <SFML/Graphics.hpp>
#include "../include/verletSolver.hpp"
#include "../include/renderer.hpp"
#include <cstdint>
#include "../include/pObj.hpp"
#include "../include/particle.hpp"
#include "../include/utils.hpp"
#include <SFML/Window.hpp>
#include <optional>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
    window.setFramerateLimit(60);
    vpe::SolverVerlet solver;
    vpe::Renderer renderer{window};

    solver.setConstraint({400.0f, 300.0f}, 300.0f);
    solver.setSimUpdateRate(60);
    solver.setSubstepCount(16);

    const float object_spawn_delay = 0.5f;
    const float object_spawn_speed = 600.0f;
    const sf::Vector2f object_spawn_pos = {400.0f, 50.0f};
    const sf::Vector2f min_max_radius = {5.0f, 20.0f};
    const uint32_t max_objects = 256;
    const float max_angle = 1.0f;

    const vpe::Particle reference_particles[3] = {
        vpe::Particle(vpe::ParticleType::ELECTRON, sf::Vector2f{0.0f, 0.0f}, ELECTRON::RADIUS, ELECTRON::CHARGE, ELECTRON::MASS),
        vpe::Particle(vpe::ParticleType::PROTON, sf::Vector2f{0.0f, 0.0f}, PROTON::RADIUS, PROTON::CHARGE, PROTON::MASS),
        vpe::Particle(vpe::ParticleType::NEUTRON, sf::Vector2f{0.0f, 0.0f}, NEUTRON::RADIUS, NEUTRON::CHARGE, NEUTRON::MASS)
    };

    int current_spawn_type_int = 0;
    sf::Clock clock;


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if(mouseEvent->button == sf::Mouse::Button::Left) {
                    vpe::Particle& obj = solver.addPhysicsObject(sf::Vector2f{static_cast<float>(mouseEvent->position.x), static_cast<float>(mouseEvent->position.y)}, reference_particles[current_spawn_type_int].getRadius(), reference_particles[current_spawn_type_int].getType(), reference_particles[current_spawn_type_int].getCharge(), reference_particles[current_spawn_type_int].getMass());
                    solver.setObjectVelocity(obj, sf::Vector2f{0.0f, 0.0f});
                }
            }
            else if(const auto* buttonEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if(buttonEvent->code == sf::Keyboard::Key::Space) {
                        current_spawn_type_int = (current_spawn_type_int + 1) % 3;
                        std::cout << "Current spawn type: ";
                        switch(current_spawn_type_int) {
                            case 0:
                                std::cout << "ELECTRON\n";
                                break;
                            case 1:
                                std::cout << "PROTON\n";
                                break;
                            case 2:
                                std::cout << "NEUTRON\n";
                                break;
                        }
                    }
                }
        }
        solver.update();
        window.clear(sf::Color::White);
        renderer.render(solver);
        window.display();
    }
    return 0;
}