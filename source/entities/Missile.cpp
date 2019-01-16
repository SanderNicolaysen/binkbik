#include "plog/include/plog/Log.h"
#include <cmath>
#include <Collision.h>

#include "Missile.h"
#include "state/Game.h"

Missile::Missile(sf::Vector2f startPosition,
           float angle,
           Game &game,
           float speed) : Entity(game),
                         startPosition(startPosition),
                         direction(direction),
                         angle(angle),
                          speed(speed)
{
    TYPE = EntityID::MISSILE;
    texture = game.getResourceLoader().tex_missile;

    sprite.setTexture(*texture);

    init();
    LOGD << "Created new missile";
}

void Missile::init()
{
    origin.x = sprite.getLocalBounds().width / 2;
    origin.y = 0;

    sprite.setOrigin(origin);

    smoke[0].setRadius(1);
    smoke[1].setRadius(2);
    smoke[2].setRadius(3);
    smoke[3].setRadius(4);
    smoke[4].setRadius(5);
    for (auto &s : smoke)
    {
        s.setFillColor(sf::Color(130, 130, 130, 200));
        s.setOrigin(s.getRadius(), s.getRadius());
        s.setPosition(-999999, -999999);
    }

    sprite.setPosition(startPosition);
    sprite.setRotation(angle);

    // Amount of damage inflicted on the spaceship if hit by a beam
    damage = 10;
}

void Missile::draw(sf::RenderWindow &window)
{
    for (auto s : smoke)
        window.draw(s);
    window.draw(sprite);
}

void Missile::update(float delta)
{
    if (age > 30)
        game.getEntityCollection().deleteEntity(this);

    checkAsteroidCollision();

    float angleInRadians = (float)((sprite.getRotation()) * (M_PI / 180));

    speed += 300*delta;

    sprite.move(speed * delta * (float) cos(angleInRadians - M_PI_2),
                speed * delta * (float) sin(angleInRadians - M_PI_2));

    if(smokeframetimer > 0.064)
    {
        smoke[0].setPosition(smoke[1].getPosition());
        smoke[1].setPosition(smoke[2].getPosition());
        smoke[2].setPosition(smoke[3].getPosition());
        smoke[3].setPosition(smoke[4].getPosition());
        smoke[4].setPosition(sprite.getPosition().x - (float) cos(angleInRadians - M_PI_2) * 28,
                             sprite.getPosition().y - (float) sin(angleInRadians - M_PI_2) * 28);

        smokeframetimer = 0;
    }
    smokeframetimer += delta;
    age += delta;
}

void Missile::checkAsteroidCollision()
{
    for (auto it : game.getEntityCollection().getEntityRange(ASTEROID)){
        auto asteroid = std::dynamic_pointer_cast<Asteroid>(it.second);

        if (Collision::CircleTest(asteroid->sprite, sprite))
        {
            soundEffect->sounds[SoundEffect::HITBYMISSILE].play();

            asteroid->health -= damage;
            if (asteroid->health <= 0)
            {
                game.getEntityCollection().getSpaceship()->inventory.addResource(asteroid->getResource());
                game.getEntityCollection().deleteEntity(asteroid);
            }
            game.getEntityCollection().deleteEntity(this);
        }
    }

}