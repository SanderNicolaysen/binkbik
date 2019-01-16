#ifndef BINKBIK_HEALTHBAR_H
#define BINKBIK_HEALTHBAR_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "ResourceLoader.h"
#include "entities/Spaceship.h"

class HealthBar
{
public:
    explicit HealthBar(ResourceLoader &rl);

    void init();

    void draw(sf::RenderWindow &window, SpaceshipPtr spaceship);

protected:
    sf::Vector2f defaultPositionHealthBar, defaultPositionHealthHeart, defaultPositionHealthLeft;

    sf::Sprite healthHeart;

    sf::RectangleShape healthBar;

    sf::Text healthLeft;
};

#endif //BINKBIK_HEALTHBAR_H
