#ifndef BINKBIK_PLANET_H
#define BINKBIK_PLANET_H

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Planet : public Entity
{
public:
    Planet(int planetID, sf::Vector2f position, Game &game);

    void draw(sf::RenderWindow &window) override;

    void update(float delta) override;

    const int planetID;

    void visit();

    bool visited = false;
};

typedef std::shared_ptr<Planet> PlanetPtr;

#endif //BINKBIK_PLANET_H
