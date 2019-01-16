#include <iostream>
#include "Planet.h"
#include "state/Game.h"

Planet::Planet(int planetID, sf::Vector2f position, Game &game) : Entity(game), planetID(planetID) {
    TYPE = EntityID::PLANET;

    texture = game.getResourceLoader().tex_planets[planetID - 1];

    sprite.setTexture(*texture);
    sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
    sprite.setPosition(position);
    sprite.setScale(0.45f, 0.45f);

}

void Planet::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void Planet::update(float delta) // Temporary..
{

}

void Planet::visit()
{
    if (!visited)
        return;

    int visitedPlanetsCount = 0;
    for (auto it: game.getEntityCollection().getEntityRange(Entity::PLANET))
        if (std::dynamic_pointer_cast<Planet>(it.second)->visited) visitedPlanetsCount++;

    if (visitedPlanetsCount == game.getEntityCollection().count(PLANET))
    {
        if (!game.endGame)
        {
            game.endGame = true;
            game.endGameView();
        }
    }

}
