#include <plog/include/plog/Log.h>
#include <iostream>
#include <cmath>

#include "Beam.h"
#include "state/Game.h"

Beam::Beam(sf::Vector2f startPosition,
           float angle,
           Game &game) : Entity(game),
                         startPosition(startPosition),
                         direction(direction),
                         angle(angle)
{
    TYPE = EntityID::BEAM;
    texture = game.getResourceLoader().tex_beam;

    sprite.setScale(sf::Vector2f(0.15f, 0.05f));
    sprite.setTexture(*texture);

    init();
    LOGD << "Created new beam";
}

void Beam::init()
{
    origin.x = sprite.getLocalBounds().width;
    origin.y = sprite.getLocalBounds().height / 2;

    sprite.setOrigin(origin);

    sprite.setPosition(startPosition);
    sprite.setRotation(angle - 90);

    speed = 1400;

    // Amount of damage inflicted on the spaceship if hit by a beam
    damage = 2;
}

void Beam::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void Beam::update(float delta)
{
    float angleInRadians = (float)((sprite.getRotation()+90) * (M_PI / 180));

    sprite.move(speed * delta * (float) cos(angleInRadians - M_PI_2),
                speed * delta * (float) sin(angleInRadians - M_PI_2));
}