#ifndef BINKBIK_SPACESHIP_H
#define BINKBIK_SPACESHIP_H

#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "gui/Compass.h"
#include "gui/Inventory.h"
#include "state/Machine.h"
#include "ParticleEmitter.h"
#include "AnimatedSprite.h"
#include "GameSave.h"

class Game;
class Shield;

class Spaceship : public Entity
{
public:
    explicit Spaceship(Game &game);

    void init();

    void loadState(GameSave::SpaceshipState state);

    GameSave::SpaceshipState getState();

    void draw(sf::RenderWindow &window) override;

    void update(float delta) override;

    void move(float delta);

    void brake(float delta);

    float fuel, maxFuel, health, maxHealth, speed, energy = 100, beamenergycost = 5, energyregen = 3;

    int speedLimit, acceleration, rotationSpeed;

    int missilecount = 20;
    int guidedmissilecount = 3;

    void Shoot();

    Compass *homeCompass = nullptr;
    Compass *enemyCompass = nullptr;
    Compass *planetCompass = nullptr;

    // The "currency" of asteroids. Is increased every time an asteroid is destroyed
    int asteroidsDestroyed = 0;

    Inventory inventory;

    bool rocketUpgrade = false;

    enum Drills { IRON, STEEL, TITANIUM, GOLD };

    void setDrill(Drills drillUpgrade);

    Drills getDrill() { return drillUpgrade; };

protected:
    SoundEffect* soundEffect = SoundEffect::Instance();

    AnimatedSprite drill;
    AnimatedSprite propeller;
    AnimatedSprite exhaustLeft;
    AnimatedSprite exhaustRight;

    sf::Vector2f defaultPosition, origin;
    sf::Clock timeBetweenBeams;
    sf::Clock timeBetweenMissiles;

    bool launchleft = false;

    float angleInRadians;

    // Rate at which fuel is consumed when the player/spaceship is moving forward
    float fuelConsumptionRate;

    Drills drillUpgrade = IRON;
    float drillSpeed = 50; // Asteroid mining speed

    sf::Vector2f positionBefore;

    void revertCollision(EntityPtr entity);

    void collision(EntityPtr entity, float &delta);

    void proximity(EntityPtr entity, float& delta);

    // Returns the angle between where the spaceship is pointing and
    // the center of an entity.
    float getAngleBetween(EntityPtr entity);

    ParticleEmitter particles;

    void updatePropulsionSprites(float delta);

    void LaunchMissile();

    void LaunchGuidedMissile();

    void playMissileSound();
};

typedef std::shared_ptr<Spaceship> SpaceshipPtr;


#endif //BINKBIK_SPACESHIP_H
