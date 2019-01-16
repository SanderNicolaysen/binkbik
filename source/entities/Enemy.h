#ifndef BINKBIK_ENEMY_H
#define BINKBIK_ENEMY_H

#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "SoundEffect.h"
#include "Planet.h"

class Enemy : public Entity
{
public:
    Enemy(PlanetPtr planet, Game &game);

    void init();

    void draw(sf::RenderWindow &window) override;

    void update(float delta) override;

    void move(float delta);

    void shoot();

    void damageTaken();

    void pauseEnemyMusic();

    int distanceFromEnemy();

    int distanceFromCombat;

    int health;

    // The planet this enemy is orbiting
    PlanetPtr planet;

protected:
    sf::Vector2f origin;
    float angleInRadians, speed;

    sf::Clock timeElapsed;

    SoundEffect* soundEffect = SoundEffect::Instance();

    sf::RectangleShape healtBar;

    void updateHealthBar();
};

typedef std::shared_ptr<Enemy> EnemyPtr;

#endif //BINKBIK_ENEMY_H
