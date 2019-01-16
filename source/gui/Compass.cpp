#include "Compass.h"
#include "state/Game.h"

void Compass::draw(sf::RenderWindow &window)
{
    if(target != nullptr)
        window.draw(sprite);
}

Compass::Compass(Game &game, EntityPtr target, sf::Color color) : game(game), target(target)
{
    sprite.setTexture(*game.getResourceLoader().tex_compass);

    sprite.setColor(color);

    sprite.setOrigin(16,32);

    this->target = target;
}

Compass::Compass(Game &game, int type, sf::Color color) : game(game), type(type)
{
    sprite.setTexture(*game.getResourceLoader().tex_compass);

    sprite.setColor(color);

    sprite.setOrigin(16,32);

    this->type = type;

    this->target = nearestEntity();
}

void Compass::update(float delta)
{
    if(type != -1)
    {
        lastTargetUpdate += delta;

        if(lastTargetUpdate > 2.0f)
        {
            target = nullptr;
            target = nearestEntity();
            lastTargetUpdate = 0;
        }
    }

    if(target != nullptr)
    {
        float deltax =
                game.getEntityCollection().getSpaceship()->sprite.getPosition().x - target->sprite.getPosition().x;
        float deltay =
                game.getEntityCollection().getSpaceship()->sprite.getPosition().y - target->sprite.getPosition().y;

        float angle = atan2f(deltay, deltax);

        sf::Vector2f position(
                (game.getEntityCollection().getSpaceship()->sprite.getPosition().x -
                 std::cos(angle) * distanceFromShip),
                (game.getEntityCollection().getSpaceship()->sprite.getPosition().y - std::sin(angle) * distanceFromShip)
        );

        sprite.setPosition(position);

        sprite.setRotation(float(angle * 180 / M_PI) - 90);
    }
}

EntityPtr Compass::nearestEntity()
{

    EntityPtr nearestEntity = game.getEntityCollection().getEntityRange(Entity::EntityID(type)).begin()->second;

    if (game.getEntityCollection().count(Entity::EntityID(type)) == 0)
        return nullptr;

    for (auto it: game.getEntityCollection().getEntityRange(Entity::EntityID(type)))
    {
        if(game.getDistance(game.getEntityCollection().getSpaceship(), it.second) < game.getDistance(game.getEntityCollection().getSpaceship(), nearestEntity))
            nearestEntity = it.second;
    }

    return nearestEntity;
}