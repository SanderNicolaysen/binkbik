#ifndef BINKBIK_ENERGYBAR_H
#define BINKBIK_ENERGYBAR_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "ResourceLoader.h"
#include "entities/Entity.h"
#include "entities/Spaceship.h"

class EnergyBar
{
public:
    explicit EnergyBar(ResourceLoader &rl);

    void init();

    void draw(sf::RenderWindow &window, SpaceshipPtr spaceship);

protected:
    sf::Vector2f defaultPositionEnergyBar, defaultPositionLightning, defaultPositonEnergyLeft;

    sf::Sprite lightning;

    sf::RectangleShape energyBar;

    sf::Text energyLeft;
};

#endif //BINKBIK_ENERGYBAR_H