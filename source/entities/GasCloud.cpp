#include "GasCloud.h"
#include "state/Game.h"

GasCloud::GasCloud(sf::Vector2f position, Game &game) : Entity(game), position(position)
{
    TYPE = EntityID::GASCLOUD;
    texture = game.getResourceLoader().tex_gasCloud;

    sprite.setScale(3.0f, 3.0f);
    sprite.setTexture(*texture);
    sprite.setPosition(position);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
}

void GasCloud::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void GasCloud::update(float delta)
{

}
