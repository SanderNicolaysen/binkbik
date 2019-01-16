#include "Collision.h"
#include "Shield.h"
#include "state/Game.h"
#include "entities/Enemy.h"
#include "Spacerock.h"

// Shield colors:
    // Health: <50%, 100%] -> Color blue
    // Health: <20%, 50%] -> Color orange
    // Health: <0%, 20%] -> Color Red

// Shield will recharge when spaceship is not engaged in combat

// TODO: Add red flash on shield when hit by laser

Shield::Shield(sf::Vector2f position, Game& game) : Entity(game), position(position)
{
    TYPE = EntityID::SHIELD;

    sprite.setTexture(*game.getResourceLoader().tex_shield);

    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

    spaceship = game.getEntityCollection().getSpaceship();

    maxHealth = 50;
    health = maxHealth;
    rechargeRate = 3.0f;

    scale = 0.0f;

    upgrade = false;
    activated = false;
}

void Shield::update(float delta)
{
    if (!upgrade)
        return;

    SpaceshipPtr spaceship = game.getEntityCollection().getSpaceship();
    bool enemyNearby = false;
    for (auto it: game.getEntityCollection().getEntityRange(Entity::ENEMY))
    {
        EnemyPtr enemy = std::dynamic_pointer_cast<Enemy>(it.second);
        if (enemy->distanceFromEnemy() < enemy->distanceFromCombat)
            enemyNearby = true;
    }

    // If there are no more enemies, we should still recharge
    if (!enemyNearby)
    {
        if (health < maxHealth)
            recharge(delta);
    }

    if (!activated)
        return;

    // Shield animation
    if (scale < 0.385f)
        sprite.setScale(scale += timer.restart().asSeconds(), scale += timer.restart().asSeconds());

    position.x = spaceship->sprite.getPosition().x;
    position.y = spaceship->sprite.getPosition().y;
    sprite.setPosition(position);

    checkCollisions(delta);

    // Set colors for shield
    if (health/maxHealth > 0.5f)
        sprite.setColor(sf::Color::Cyan);
    else if (health/maxHealth <= 0.5f && health/maxHealth > 0.2f)
        sprite.setColor(sf::Color(255, 150, 0));
    else
        sprite.setColor(sf::Color::Red);
}

void Shield::draw(sf::RenderWindow &window)
{
    if (!upgrade || !activated)
        return;

    window.draw(sprite);
}

void Shield::checkCollisions(float &delta)
{
    for (auto it : game.getEntityCollection())
    {
        if (it.first == Entity::SHIELD)
            continue;

        EntityPtr entity = it.second;
        if (Collision::BoundingBoxTest(sprite, entity->sprite))
        {
            if (Collision::CircleTest(sprite, entity->sprite))
            {
                collideWith(entity, delta);
            }
        }
    }
}

void Shield::collideWith(EntityPtr entity, float &delta)
{
    if (entity->TYPE == Entity::BEAM)
    {
        decreaseHealth(10);
        game.getEntityCollection().deleteEntity(entity);
    }
    else if (entity->TYPE == Entity::GASCLOUD)
    {
        if (this->health <= 0)
        {
            this->health = 0;
            activated = false;
            return;
        }

        // The gas cloud damages the spaceship
        if (health > 0)
            health -= 10.0f * delta;
    }
    else if (entity->TYPE == Entity::SPACEROCK && std::dynamic_pointer_cast<Spacerock>(entity)->lastImpact < time(nullptr) - 2)
    {
        decreaseHealth(5);

        std::dynamic_pointer_cast<Spacerock>(entity)->lastImpact = time(nullptr);
    }
}

void Shield::decreaseHealth(int health)
{
    if (this->health <= 0)
    {
        this->health = 0;
        activated = false;
        return;
    }

    this->health -= health;
}

void Shield::recharge(float delta)
{
    if (health >= maxHealth)
    {
        health = maxHealth;
        return;
    }

    health += rechargeRate * delta;
}
