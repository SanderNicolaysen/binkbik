#include <SFML/OpenGL.hpp>
#include "Spacerock.h"
#include "state/Game.h"

Spacerock::Spacerock(Game &game, PlanetPtr planet, float radius, float speed, float scale, float startPos)
        : Entity(game),
          planet(planet),
          radius(radius),
          V_a(speed)
{
    TYPE = EntityID::SPACEROCK;

    texture = game.getResourceLoader().tex_spacerock;
    sprite.setTexture(*texture);
    sprite.setScale(scale, scale);

    angle = startPos;
}

void Spacerock::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void Spacerock::update(float delta)
{
    sprite.setRotation(angle * 180/float(M_PI) + 90);

    sf::Vector2f pos{
            planet->sprite.getPosition().x + std::cos(angle) * radius,
            planet->sprite.getPosition().y + std::sin(angle) * radius
    };

    angle += delta * V_a;

    sprite.setPosition(pos);
}
