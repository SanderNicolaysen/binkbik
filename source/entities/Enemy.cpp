#include <plog/include/plog/Log.h>
#include <iostream>
#include <cmath>

#include "Beam.h"
#include "Enemy.h"
#include "state/Game.h"
#include "Collision.h"
#include "Missile.h"
#include "GuidedMissile.h"

Enemy::Enemy(PlanetPtr planet, Game &game) : Entity(game), planet(planet)
{
    TYPE = EntityID::ENEMY;
    texture = game.getResourceLoader().tex_enemy;

    sprite.setScale(sf::Vector2f(0.2f, 0.2f));
    sprite.setTexture(*texture);

    distanceFromCombat = 1500;

    init();
    LOGD << "Created new enemy";
}

void Enemy::init()
{
    origin.x = sprite.getLocalBounds().width / 2;
    origin.y = sprite.getLocalBounds().height / 2;

    sprite.setOrigin(origin);
    sprite.setScale(sf::Vector2f(0.5f, 0.5f));

    health = 100;
    angleInRadians = 0;

    timeElapsed.restart();
}

void Enemy::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
    window.draw(healtBar);
}

void Enemy::update(float delta)
{
    // Move the enemy
    move(delta);

    updateHealthBar();

    if (distanceFromEnemy() < distanceFromCombat)
    {
        shoot();
    }

    damageTaken();
}

void Enemy::move(float delta)
{
    float orbitRadius = planet->sprite.getGlobalBounds().width/2;
    // Move enemy in a circular motion around a planet (which has the position orbitcenter, and the radius orbitradius)
    sprite.setPosition((orbitRadius + 250) * cosf(angleInRadians) + planet->sprite.getPosition().x,
                       (orbitRadius + 250) * sinf(angleInRadians) + planet->sprite.getPosition().y);

    angleInRadians += delta / 10.0f;

    if (angleInRadians >= 2 * M_PI)
        angleInRadians = 0;
}

void Enemy::shoot()
{
    SpaceshipPtr spaceship = game.getEntityCollection().getSpaceship();

    float xPosPlayer = spaceship->sprite.getPosition().x;
    float yPosPlayer = spaceship->sprite.getPosition().y;
    float xPosEnemy = sprite.getPosition().x;
    float yPosEnemy = sprite.getPosition().y;

    float angle = atan2f(yPosEnemy - yPosPlayer, xPosEnemy - xPosPlayer) * (180/3.14159f);

    // Turn the enemy towards the spaceship
    sprite.setRotation(angle - 90);

    // The enemy "within range" will fire a beam in the direction of the spaceship about once every second
    if (timeElapsed.getElapsedTime().asSeconds() > 1)
    {
        soundEffect->sounds[SoundEffect::BEAMSHOT].play();

        float xPosBeam = sprite.getPosition().x;
        float yPosBeam = sprite.getPosition().y;

        game.getEntityCollection().addEntity(EntityPtr(new Beam(sf::Vector2f(xPosBeam, yPosBeam), angle-90, game)));

        timeElapsed.restart();
    }
}

void Enemy::damageTaken()
{
    auto range = game.getEntityCollection().getEntityRange(Entity::SPACESHIPBEAM);

    for (auto &it : range) {
        EntityPtr entity = it.second;

        //Finner ut om entity har truffet Enemy.
        if (Collision::BoundingBoxTest(entity->sprite, sprite)) {

            soundEffect->sounds[SoundEffect::HITBYBEAM].play();

            if (this->health > 0)
            {
                this->health -= std::dynamic_pointer_cast<Beam>(entity)->damage;
                game.getEntityCollection().deleteEntity(entity);
            }

            if (this->health <= 0)
            {
                pauseEnemyMusic();

                game.getEntityCollection().deleteEntity(this);
            }
        }
    }

    range = game.getEntityCollection().getEntityRange(Entity::MISSILE);

    for (auto &it : range) {
        EntityPtr entity = it.second;

        //Finner ut om entity har truffet Enemy.
        if (Collision::BoundingBoxTest(entity->sprite, sprite)) {

            soundEffect->sounds[SoundEffect::HITBYMISSILE].play();

            auto missile = std::dynamic_pointer_cast<Missile>(entity);
            float missiledamage = missile->damage;
            if (this->health > 0)
            {
                this->health -= missiledamage;
                game.getEntityCollection().deleteEntity(entity);
            }

            if (this->health <= 0)
            {
                pauseEnemyMusic();

                game.getEntityCollection().deleteEntity(this);
            }
        }
    }

    range = game.getEntityCollection().getEntityRange(Entity::GUIDEDMISSILE);

    for (auto &it : range) {
        EntityPtr entity = it.second;

        //Finner ut om entity har truffet Enemy.
        if (Collision::BoundingBoxTest(entity->sprite, sprite)) {

            soundEffect->sounds[SoundEffect::HITBYMISSILE].play();

            auto missile = std::dynamic_pointer_cast<GuidedMissile>(entity);
            float missiledamage = missile->damage;
            if (this->health > 0)
            {
                this->health -= missiledamage;
                game.getEntityCollection().deleteEntity(entity);
            }

            if (this->health <= 0)
            {
                pauseEnemyMusic();

                game.getEntityCollection().deleteEntity(this);
            }
        }
    }
}

void Enemy::pauseEnemyMusic()
{
    if (soundEffect->enemyNearby.getStatus() == soundEffect->enemyNearby.Playing)
    {
        soundEffect->enemyNearby.pause();

        soundEffect->exploration.play();
    }
}

int Enemy::distanceFromEnemy()
{
    SpaceshipPtr spaceship = game.getEntityCollection().getSpaceship();

    // Check if the spaceship is within the "detection range" (800 px) of the enemy
    return sqrt(pow(sprite.getPosition().x - spaceship->sprite.getPosition().x, 2) + pow(sprite.getPosition().y - spaceship->sprite.getPosition().y, 2));
}

void Enemy::updateHealthBar()
{
    healtBar.setPosition(sprite.getPosition().x - 50.0f, sprite.getPosition().y - 110.0f);
    healtBar.setFillColor(sf::Color::Red);

    healtBar.setSize(sf::Vector2f((float(health) / 100.0f) * 100.0f, 5.0f));

}
