#ifndef BINKBIK_FUELBAR_H
#define BINKBIK_FUELBAR_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "ResourceLoader.h"
#include "entities/Entity.h"
#include "entities/Spaceship.h"

class FuelBar
{
public:
    explicit FuelBar(ResourceLoader &rl);

    void init();

    void draw(sf::RenderWindow &window, SpaceshipPtr spaceship);

protected:
    sf::Vector2f defaultPositionFuelBar, defaultPositionFuelCan, defaultPositonFuelLeft;

    sf::Sprite fuelCan;

    sf::RectangleShape fuelBar;

    sf::Text fuelLeft;
};

#endif //BINKBIK_FUELBAR_H
