#ifndef BINKBIK_GUIDEDMISSILE_H
#define BINKBIK_GUIDEDMISSILE_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "SoundEffect.h"
#include "Entity.h"

class GuidedMissile : public Entity
{
public:
    GuidedMissile(sf::Vector2f startPosition, float angle, Game &game, float speed);

    void init();

    void draw(sf::RenderWindow &window) override;

    void update(float delta) override;

    float speedx, speedy, damage;

    float targetage = 3.0f;

    float turnrate = 5;

    float rotationspeed;
protected:
    EntityPtr target = nullptr;

    sf::Vector2f startPosition, direction, origin;
    float angle;

    sf::CircleShape smoke[5];

    float smokeframetimer = 0;

    float age = 0;

    void checkAsteroidCollision();

    SoundEffect* soundEffect = SoundEffect::Instance();
};


#endif //BINKBIK_MISSILE_H
