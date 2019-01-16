#ifndef BINKBIK_BEAM_H
#define BINKBIK_BEAM_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Entity.h"

class Beam : public Entity
{
public:
    Beam(sf::Vector2f startPosition,
         float angle,
         Game &game);

    void init();

    void draw(sf::RenderWindow &window) override;

    void update(float delta) override;

    int damage;
protected:
    sf::Vector2f startPosition, direction, origin;

    int speed;
    float angle;
};

#endif //BINKBIK_BEAM_H
