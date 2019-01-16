#ifndef BINKBIK_MISSILE_H
#define BINKBIK_MISSILE_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "SoundEffect.h"
#include "Entity.h"

class Missile : public Entity
{
public:
    Missile(sf::Vector2f startPosition, float angle, Game &game, float speed);

    void init();

    void draw(sf::RenderWindow &window) override;

    void update(float delta) override;

    float speed, damage;
protected:

    sf::CircleShape smoke[5];

    float smokeframetimer = 0;

    float age = 0;

    sf::Vector2f startPosition, direction, origin;
    float angle;

    void checkAsteroidCollision();

    SoundEffect* soundEffect = SoundEffect::Instance();
};


#endif //BINKBIK_MISSILE_H
