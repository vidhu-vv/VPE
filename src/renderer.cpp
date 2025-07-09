#include "../include/renderer.hpp"


vpe::Renderer::Renderer(sf::RenderTarget& target_)
 : target{target_}
{};

void vpe::Renderer::render(const SolverVerlet& solver){
    const sf::Vector3f constraint = solver.getConstraint();
    sf::CircleShape constraint_background{constraint.z};
    constraint_background.setOrigin({constraint.z, constraint.z});
    constraint_background.setFillColor(sf::Color::Black);
    constraint_background.setPosition({constraint.x, constraint.y});
    constraint_background.setPointCount(128);
    target.draw(constraint_background);

    sf::CircleShape circle{1.0f};
    circle.setPointCount(32);
    circle.setOrigin({1.0f, 1.0f});
    const auto& objects = solver.getObjects();
    for (const auto& obj : objects) {
        circle.setPosition(obj.position);
        circle.setScale({obj.radius, obj.radius});
        circle.setFillColor(obj.color);
        target.draw(circle);
    }

        
}