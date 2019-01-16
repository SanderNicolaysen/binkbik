#include <iostream>
#include <plog/include/plog/Log.h>
#include "Asteroid.h"
#include "state/Game.h"

Asteroid::Asteroid(sf::Vector2f position, Game &game) : Entity(game), position(position)
{
    TYPE = EntityID::ASTEROID;

    sprite.setPosition(position);

    AsteroidPtr asteroidPtr = std::make_shared<Asteroid>(*this);

    double threshold = game.getDistance(asteroidPtr, game.getEntityCollection().getSpacestation())/40000;
    int resourcelvl = 0;

    std::uniform_real_distribution<double> rng (0,1);

    for (int i = 0; i < 7; ++i)
    {
        if (rng(game.generator) < threshold)
            resourcelvl++;
        else
            break;
    }

    resource = (ResourceID) resourcelvl;

    switch (resource)
    {
        case ROCK:
            texture = game.getResourceLoader().tex_asteroid1;
            break;
        case IRON:
            texture = game.getResourceLoader().tex_asteroid2;
            break;
        case SILVER:
            texture = game.getResourceLoader().tex_asteroid3;
            break;
        case IRIDIUM:
            texture = game.getResourceLoader().tex_asteroid4;
            break;
        case GOLD:
            texture = game.getResourceLoader().tex_asteroid5;
            break;
        case PLATINUM:
            texture = game.getResourceLoader().tex_asteroid6;
            break;
        case OGRIUM:
            texture = game.getResourceLoader().tex_asteroid7;
            break;
        case SPACEIUM:
            texture = game.getResourceLoader().tex_asteroid8;
            break;
        default:
            break;
    }

    sprite.setTexture(*texture);

    sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);

    health = 100 + resourcelvl * 25;
}

void Asteroid::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

Asteroid::ResourceID Asteroid::getResource()
{
    return resource;
}
