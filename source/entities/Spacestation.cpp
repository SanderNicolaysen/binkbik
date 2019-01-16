#include <iostream>
#include <plog/include/plog/Log.h>
#include "Spacestation.h"
#include "state/Game.h"

Spacestation::Spacestation(Game &game) : Entity(game)
{
    TYPE = EntityID::SPACESTATION;

    position.x = 0.0f;
    position.y = 0.0f;

    texture = game.getResourceLoader().tex_spacestation;

    sprite.setTexture(*texture);
    sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
    sprite.setPosition(position);
}

void Spacestation::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void Spacestation::update(float delta)
{
    sprite.setRotation(sprite.getRotation() + 30*delta);
}
