#include "plog/include/plog/Log.h"
#include <cmath>
#include "Collision.h"

#include "GuidedMissile.h"
#include "state/Game.h"

GuidedMissile::GuidedMissile(sf::Vector2f startPosition,
                 float angle,
                 Game &game,
                 float speed) : Entity(game),
                                startPosition(startPosition),
                                direction(direction),
                                angle(angle)
{
    TYPE = EntityID::GUIDEDMISSILE;
    texture = game.getResourceLoader().tex_guidedmissile;

    speedx = speed * float(cos((angle-90) * (M_PI / 180)));
    speedy = speed * float(sin((angle-90) * (M_PI / 180)));

    sprite.setTexture(*texture);

    init();
    LOGD << "Created new missile";
}

void GuidedMissile::init()
{
    origin.x = sprite.getLocalBounds().width / 2;
    origin.y = 0;

    sprite.setOrigin(origin);

    sprite.setPosition(startPosition);
    sprite.setRotation(angle);

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


    // Amount of damage inflicted on the spaceship if hit by a missile
    damage = 10;
}

void GuidedMissile::draw(sf::RenderWindow &window)
{
    for (auto s : smoke)
        window.draw(s);
    window.draw(sprite);
}

void GuidedMissile::update(float delta)
{
    if (age > 30)
        game.getEntityCollection().deleteEntity(this);

    checkAsteroidCollision();
    float angleInRadians = (float) ((sprite.getRotation()) * (M_PI / 180));

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

    speedx *= 0.99;
    speedy *= 0.99;

    speedx += delta * (500 * (float) cos(angleInRadians - M_PI_2));
    speedy += delta * (500 * (float) sin(angleInRadians - M_PI_2));

    sprite.move(speedx * delta, speedy * delta);

    if (targetage > 2.0f || target == nullptr)
    {
        target = nullptr;
        for (auto it : game.getEntityCollection().getEntityRange(ENEMY))
        {
            if (target == nullptr)
            {
                target = it.second;
            } else if (game.getDistance(it.second->sprite, sprite) < game.getDistance(target->sprite, sprite))
                target = it.second;
        }
        if (target == nullptr)
        {
            for (auto it : game.getEntityCollection().getEntityRange(ASTEROID))
            {
                if (target == nullptr)
                {
                    target = it.second;
                } else if (game.getDistance(it.second->sprite, sprite) < game.getDistance(target->sprite, sprite))
                    target = it.second;
            }
        }
        targetage = 0;
    }

    rotationspeed *= 0.95;

    float facingAngle = sprite.getRotation();

    sf::Vector2f targetDir = sprite.getPosition() - target->sprite.getPosition();

    float targetAngle = atan2f(targetDir.x, targetDir.y) * 180 / float(M_PI);
    if (targetAngle < 0)
        targetAngle = - targetAngle;
    else
        targetAngle = 360 - targetAngle;

    float angleDiff = facingAngle - targetAngle;
    if (angleDiff <= -180)
        angleDiff = angleDiff + 360;
    if (angleDiff > 180)
        angleDiff = angleDiff - 360;


    if (angleDiff < 0)
    {
        rotationspeed += delta * turnrate;
    }
    else
    {
        rotationspeed -= delta * turnrate;
    }

    sprite.rotate(rotationspeed);

    smokeframetimer += delta;
    targetage += delta;
    age += delta;
}

void GuidedMissile::checkAsteroidCollision()
{
    if (target != nullptr && target->TYPE == ASTEROID)
    {
        auto asteroid = std::dynamic_pointer_cast<Asteroid>(target);
        if (asteroid->health <= 0)
        {
            target = nullptr;
            return;
        }

        if (Collision::CircleTest(target->sprite, sprite))
        {
            soundEffect->sounds[SoundEffect::HITBYMISSILE].play();

            asteroid->health -= damage;
            if (asteroid->health <= 0)
            {
                game.getEntityCollection().getSpaceship()->inventory.addResource(asteroid->getResource());
                game.getEntityCollection().deleteEntity(target);
                target = nullptr;
            }
            game.getEntityCollection().deleteEntity(this);
        }
    }
}