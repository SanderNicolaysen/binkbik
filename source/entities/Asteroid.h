#ifndef BINKBIK_ASTEROID_H
#define BINKBIK_ASTEROID_H

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Asteroid : public Entity
{
public:
    explicit Asteroid(sf::Vector2f position, Game &game);

    void draw(sf::RenderWindow &window) override;

    enum ResourceID { ROCK, IRON, SILVER, IRIDIUM, GOLD, PLATINUM, OGRIUM, SPACEIUM };

    ResourceID getResource();

    float health;

protected:
    sf::Vector2f position;

    ResourceID resource;
};

typedef std::shared_ptr<Asteroid> AsteroidPtr;

#endif //BINKBIK_ASTEROID_H
