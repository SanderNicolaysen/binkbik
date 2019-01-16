#ifndef BINKBIK_SHIELD_H
#define BINKBIK_SHIELD_H

#include <SFML/Graphics.hpp>

#include "Spaceship.h"
#include "Entity.h"

class Game;

class Shield : public Entity
{
public:
    Shield(sf::Vector2f position, Game& game);

    void update(float delta) override;

    void draw(sf::RenderWindow &window) override;

    void decreaseHealth(int health);

    void recharge(float delta);

    void checkCollisions(float& delta);

    void collideWith(EntityPtr entity, float& delta);

    bool upgrade, activated;

    sf::Clock timer;
    float scale;

    float health, maxHealth;

protected:
    float rechargeRate;

    sf::Sprite sprite;

    sf::Vector2f position;

    SpaceshipPtr spaceship;

    SoundEffect* soundEffect = SoundEffect::Instance();
};

typedef std::shared_ptr<Shield> ShieldPtr;

#endif //BINKBIK_SHIELD_H
