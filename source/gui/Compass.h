#ifndef BINKBIK_COMPASS_H
#define BINKBIK_COMPASS_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "entities/Entity.h"

class Game;

class Compass
{
public:
    explicit Compass(Game &game, EntityPtr target, sf::Color color);

    Compass(Game &game, int type, sf::Color color);

    void draw(sf::RenderWindow &window);

    void update(float delta);

    EntityPtr nearestEntity();

protected:
    float distanceFromShip = 75;
    float lastTargetUpdate = 0;
    sf::Sprite sprite;
    EntityPtr target;
    int type = -1;
    Game &game;
};


#endif //BINKBIK_COMPASS_H
