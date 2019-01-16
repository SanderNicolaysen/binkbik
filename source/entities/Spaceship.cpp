#include <plog/Log.h>
#include <iostream>
#include <cmath>
#include <SFML/Window/Keyboard.hpp>
#include "Spaceship.h"
#include "Collision.h"
#include "state/Game.h"
#include "Beam.h"
#include "Shield.h"
#include "Spacerock.h"
#include "Missile.h"
#include "GuidedMissile.h"
#include "Enemy.h"

Spaceship::Spaceship(Game &game) :
        Entity(game),
        inventory(game.getResourceLoader()),
        particles(1000),
        drill(game, game.getResourceLoader().tex_drill, sf::IntRect(0, 0, 128, 64), 3),
        propeller(game, game.getResourceLoader().tex_propeller, sf::IntRect(0, 0, 32, 32), 4),
        exhaustLeft(game, game.getResourceLoader().tex_exhaust, sf::IntRect(0, 0, 32, 64), 16),
        exhaustRight(game, game.getResourceLoader().tex_exhaust, sf::IntRect(0, 0, 32, 64), 16)
{
    TYPE = EntityID::SPACESHIP;
    texture = game.getResourceLoader().tex_spaceship;

    defaultPosition.x = 400.0f;
    defaultPosition.y = 0.0f;
    rotationSpeed = 300;
    sprite.setTexture(*texture);
    sf::Vector2f scale(0.55f, 0.55f);
    sprite.setScale(scale);

    propeller.start();
    exhaustLeft.start();
    exhaustLeft.setFrameDuration(16);

    exhaustRight.start();
    exhaustRight.setFrameDuration(16);

    soundEffect->sounds[SoundEffect::PROPELLER].play();

    drill.setScale(scale);
    drill.setFrameDuration(80);

    init();
    LOGD << "Created new spaceship";
}

void Spaceship::init()
{
    sprite.setPosition(defaultPosition);

    origin.x = sprite.getLocalBounds().width / 2;
    origin.y = sprite.getLocalBounds().height / 2;
    sprite.setOrigin(origin);
    drill.setOrigin(origin);
    propeller.setOrigin(sf::Vector2f(16,0));
    exhaustLeft.setOrigin(sf::Vector2f(39,0));
    exhaustRight.setOrigin(sf::Vector2f(-8,0));

    speed = 0;
    health = 100.0f;
    maxHealth = 100.0f;
    fuel = 100.0f;
    maxFuel = 100.0f;
    fuelConsumptionRate = 2.0f;
    acceleration = 250;
    speedLimit = 300;
}

void Spaceship::loadState(GameSave::SpaceshipState state)
{
    fuel = state.fuel;
    maxFuel = state.maxFuel;
    health = state.health;
    maxHealth = state.maxHealth;

    speed = state.speed;
    speedLimit = state.speedLimit;
    acceleration = state.acceleration;
    rotationSpeed = state.rotationSpeed;
    sprite.setRotation(state.rotation);

    rocketUpgrade = state.rocketUpgrade;
    setDrill((Spaceship::Drills)state.drillUpgrade);

    missilecount = state.missilecount;
    guidedmissilecount = state.guidedmissilecount;

    sprite.setPosition(state.position.first, state.position.second);
}

GameSave::SpaceshipState Spaceship::getState()
{
    GameSave::SpaceshipState state;

    state.fuel = fuel;
    state.maxFuel = maxFuel;
    state.health = health;
    state.maxHealth = maxHealth;

    state.speed = speed;
    state.speedLimit = speedLimit;
    state.acceleration = acceleration;
    state.rotationSpeed = rotationSpeed;
    state.rotation = sprite.getRotation();

    state.rocketUpgrade = rocketUpgrade;
    state.drillUpgrade = (unsigned int)drillUpgrade;

    state.missilecount = missilecount;
    state.guidedmissilecount = guidedmissilecount;

    state.position.first = sprite.getPosition().x;
    state.position.second = sprite.getPosition().y;

    return state;
}

void Spaceship::draw(sf::RenderWindow &window)
{
    window.draw(sprite);

    // Draw Compasses
    if (homeCompass != nullptr)
        homeCompass->draw(window);
    if (enemyCompass != nullptr)
        enemyCompass->draw(window);
    if (planetCompass != nullptr)
        planetCompass->draw(window);

    if (rocketUpgrade)
    {
        exhaustLeft.draw(window);
        exhaustRight.draw(window);
    }
    else
    {
        propeller.draw(window);
    }
    drill.draw(window);
    window.draw(particles);
}

void Spaceship::update(float delta)
{
    // store the current position before moving
    positionBefore = sprite.getPosition();

    // Move the spaceship
    move(delta);

    // Move the particles FAAAAR away so we can't see them. They will be moved back when needed
    particles.setEmitter(sf::Vector2f(-999999, -999999));

    // Check for collisions
    for (auto it: game.getEntityCollection())
    {
        if (it.first == SPACESHIP) continue;

        if (Collision::BoundingBoxTest(sprite, it.second->sprite))
        {
            proximity(it.second, delta);

            if (Collision::CircleTest(sprite, it.second->sprite))
                collision(it.second, delta);
        }
    }

    if (soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].getVolume() > 0)
        soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].setVolume(std::max(soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].getVolume() - delta * 100.0f, 0.0f));

    // Turn on/off enemy music when engaging an enemy.
    bool enemyNearby = false;
    for (auto it: game.getEntityCollection().getEntityRange(Entity::ENEMY))
    {
        EnemyPtr enemy = std::dynamic_pointer_cast<Enemy>(it.second);
        if (enemy->distanceFromEnemy() < enemy->distanceFromCombat)
            enemyNearby = true;
    }

    if (enemyNearby)
    {
        if (soundEffect->exploration.getStatus() == soundEffect->exploration.Playing)
        {
            soundEffect->exploration.pause();
            soundEffect->enemyNearby.play();
        }
    }
    else if (soundEffect->enemyNearby.getStatus() == soundEffect->enemyNearby.Playing)
    {
        soundEffect->enemyNearby.pause();

        soundEffect->exploration.play();
    }


    //Updates
    if (homeCompass != nullptr)
        homeCompass->update(delta);
    if (enemyCompass != nullptr)
        enemyCompass->update(delta);
    if (planetCompass != nullptr)
        planetCompass->update(delta);


    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        Shoot();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        LaunchMissile();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        LaunchGuidedMissile();

    // Start the drilling animation when we press space
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) drill.start();
    else drill.stop();

    drill.setPosition(sprite.getPosition());
    drill.setRotation(sprite.getRotation());
    drill.update(delta);

    updatePropulsionSprites(delta);

    particles.update();

    if (energy < 100)
    {
        energy += delta * energyregen;
        if (energy > 100)
            energy = 100;
    }
}

void Spaceship::move(float delta)
{
    angleInRadians = (float)(sprite.getRotation() * (M_PI / 180));

    if (fuel <= 0 || health <= 0)
    {
        brake(delta);
        soundEffect->decreaseThrustSound(delta);
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if (rocketUpgrade)
        {
            if (soundEffect->sounds[SoundEffect::THRUST].getVolume() < soundEffect->thrustMaxVolume)
                soundEffect->sounds[SoundEffect::THRUST].setVolume(soundEffect->thrustMaxVolume);

            if (soundEffect->sounds[SoundEffect::THRUST].getStatus() == sf::Sound::Stopped)
            {
                soundEffect->sounds[SoundEffect::THRUST].play();
            }
            if (exhaustLeft.getStartRect().top != 0)
            {
                exhaustLeft.setStartRect(sf::IntRect(0, 0, 32, 64));
                exhaustLeft.start();
                exhaustRight.setStartRect(sf::IntRect(0, 0, 32, 64));
                exhaustRight.start();
            }
        }

        // If the spaceship is moving backwards, pressing up will "brake" the spaceship and "pull" it in the other direction
        if (speed < 0)
            speed += acceleration * 2 * delta;
        else
            speed += acceleration * delta;

        // When moving forward, make sure we don't go over the speedlimit!
        if (speed > speedLimit)
            speed = speedLimit;

        sprite.move(speed * delta * (float)cos(angleInRadians - M_PI_2), speed * delta * (float)sin(angleInRadians - M_PI_2));

        fuel -= fuelConsumptionRate * delta;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if (rocketUpgrade)
        {
            if (soundEffect->sounds[SoundEffect::THRUST].getVolume() < soundEffect->thrustMaxVolume)
                soundEffect->sounds[SoundEffect::THRUST].setVolume(soundEffect->thrustMaxVolume);

            if (soundEffect->sounds[SoundEffect::THRUST].getStatus() == sf::Sound::Stopped)
            {
                soundEffect->sounds[SoundEffect::THRUST].play();
            }
        }

        // If the spaceship is moving forward, pressing down will "brake" the spaceship and "pull" it in the other direction
        if (speed > 0)
            speed -= acceleration * 2 * delta;
        else
            speed -= acceleration * delta;

        // When moving backwards, make sure we don't go over the speedlimit!
        if (speed < -speedLimit)
            speed = -speedLimit;

        sprite.move(speed * delta * (float)cos(angleInRadians - M_PI_2), speed * delta * (float)sin(angleInRadians - M_PI_2));

        fuel -= fuelConsumptionRate * delta;
    }
    else
    {
        brake(delta);

        soundEffect->decreaseThrustSound(delta);

        if (rocketUpgrade) {
            if (!exhaustLeft.isStopped()) {
                if (exhaustLeft.getStartRect().top == 0) {
                    exhaustLeft.setStartRect(sf::IntRect(0, 64, 32, 64));
                    exhaustLeft.setFrame(0);
                    exhaustRight.setStartRect(sf::IntRect(0, 64, 32, 64));
                    exhaustRight.setFrame(0);
                } else if (exhaustLeft.getFrame() > 14) {
                    exhaustLeft.stop();
                    exhaustRight.stop();
                }
            }
        }
    }

    if (fuel < 0)
        fuel = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        sprite.rotate(rotationSpeed * delta);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        sprite.rotate(-rotationSpeed * delta);
}

void Spaceship::brake(float delta)
{
    // If the player doesnt press up or down, or if the spaceship doesnt have any fuel left, the spaceship will slow down to a halt by itself
    if (speed > 0) {
        speed -= 150 / 2 * delta;
        if (speed < 0) speed = 0;
    } else if (speed < 0) {
        speed += 150 / 2 * delta;
        if (speed > 0) speed = 0;
    }

    sprite.move(speed * delta * (float) cos(angleInRadians - M_PI_2),
                speed * delta * (float) sin(angleInRadians - M_PI_2));
}

void Spaceship::revertCollision(EntityPtr entity)
{
    sf::Vector2f newPos = sprite.getPosition();
    sf::Vector2f prevPos = positionBefore;

    // Calculate the distance between the entity and our spaceship
    sf::Vector2f distance = entity->sprite.getPosition() - sprite.getPosition();

    // Check collision in y-direction, and move the spaceship back until there
    // is no more collision detected.
    sprite.setPosition(prevPos.x, newPos.y);
    while (Collision::CircleTest(sprite, entity->sprite))
    {
        if (distance.y > 0) sprite.setPosition(prevPos.x, --newPos.y);
        else sprite.setPosition(prevPos.x, ++newPos.y);
    }

    // Do the same thing but in the direction of the x-axis
    sprite.setPosition(newPos.x, prevPos.y);
    while (Collision::CircleTest(sprite, entity->sprite))
    {
        if (distance.x > 0) sprite.setPosition(--newPos.x, prevPos.y);
        else sprite.setPosition(++newPos.x, prevPos.y);
    }

    sprite.setPosition(newPos);
}

void Spaceship::proximity(EntityPtr entity, float &delta)
{
    switch (entity->TYPE)
    {
        case SPACESTATION:
            game.displayPopupText("PRESS SPACE TO ENTER STORE");
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                game.setNestedState(NestedMachine::STORE);
            break;

        case ASTEROID:
            // Destroy the asteroid by pressing space
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && std::abs(getAngleBetween(entity)) < 45)
            {
                AsteroidPtr asteroid = std::dynamic_pointer_cast<Asteroid>(entity);

                asteroid->health -= drillSpeed * delta;
                if (asteroid->health <= 0)
                {
                    inventory.addResource(asteroid->getResource());
                    game.getEntityCollection().deleteEntity(entity);
                }

                float angle = sprite.getRotation();
                float xPos = sprite.getPosition().x - float(cos((angle + 90) / 180 * M_PI)) * sprite.getLocalBounds().height/2 * sprite.getScale().x;
                float yPos = sprite.getPosition().y - float(sin((angle + 90) / 180 * M_PI)) * sprite.getLocalBounds().height/2 * sprite.getScale().x;
                particles.setEmitter(sf::Vector2f(xPos, yPos));
            }
            break;

        case ENEMY:
            // Destroy the enemy by drilling! :D
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && std::abs(getAngleBetween(entity)) < 45) {
                EnemyPtr enemy = std::dynamic_pointer_cast<Enemy>(entity);
                if (enemy->health > 0)
                    enemy->health -= drillSpeed / 5 * delta;
                if (enemy->health <= 0)
                    game.getEntityCollection().deleteEntity(entity);
            }
            break;

        case PLANET:
            game.displayPopupText("PRESS SPACE TO VIEW PLANET INFO");
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                PlanetPtr planet = std::dynamic_pointer_cast<Planet>(entity);
                game.setPlanetView(planet->planetID);
                game.setNestedState(NestedMachine::PLANETVIEW);
                planet->visited = true;
                return;
            }
            std::dynamic_pointer_cast<Planet>(entity)->visit();
            break;

        default:
            break;
    }
}

void Spaceship::collision(EntityPtr entity, float &delta)
{
    switch (entity->TYPE)
    {
        case ASTEROID:
        case PLANET:
        case SPACESTATION:
        case ENEMY:
            revertCollision(entity);
            break;

        case BEAM:
            soundEffect->sounds[SoundEffect::HITBYBEAM].play();

            // The beam hits and damages the spaceship, and is destroyed
            health = health - 10 < 0 ? 0 : health - 10;

            game.getEntityCollection().deleteEntity(entity);
            game.flashVignette(sf::Color::Red);
            break;

        case SPACEROCK:
            {
                SpacerockPtr spacerock = std::dynamic_pointer_cast<Spacerock>(entity);
                if (spacerock->lastImpact < time(nullptr) - 2)
                {
                    // If shield is activated do not lose health from spaceship
                    if (std::dynamic_pointer_cast<Shield>(game.getEntityCollection().getEntityRange(Entity::SHIELD).begin()->second)->activated)
                        break;

                    soundEffect->smallImpact(game.generator);

                    health = health - 10 < 0 ? 0 : health - 10;
                    game.flashVignette(sf::Color(255, 0, 0));
                    spacerock->lastImpact = time(nullptr);
                }
            }
            break;

        case GASCLOUD:
            // If shield is activated
            if (std::dynamic_pointer_cast<Shield>(game.getEntityCollection().getEntityRange(Entity::SHIELD).begin()->second)->activated)
                return;

            if (soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].getVolume() < soundEffect->insideGasCloudMaxVolume)
                soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].setVolume(soundEffect->insideGasCloudMaxVolume);
            if (soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].getStatus() == sf::Sound::Stopped)
                soundEffect->sounds[SoundEffect::INSIDEGASCLOUD].play();

            // The gas cloud damages the spaceship
            health = health - 10.0f * delta < 0 ? 0 : health - 10.0f * delta;

            game.flashVignette(sf::Color(0, 255, 0, 170));
            break;
    }
}

void Spaceship::Shoot()
{
    float angle = sprite.getRotation();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E) && timeBetweenBeams.getElapsedTime().asMilliseconds() > 90 && energy >= beamenergycost)
    {
        float xPosBeam = sprite.getPosition().x - float(cos((angle + 90) / 180 * M_PI)) * 100;
        float yPosBeam = sprite.getPosition().y - float(sin((angle + 90) / 180 * M_PI)) * 100;

        soundEffect->sounds[SoundEffect::BEAMSHOT].play();

        Beam* beam = new Beam(sf::Vector2f(xPosBeam, yPosBeam), angle, game);
        beam->TYPE = SPACESHIPBEAM;

        game.getEntityCollection().addEntity(EntityPtr(beam));

        timeBetweenBeams.restart();
        energy -= beamenergycost;
    }
}

void Spaceship::LaunchMissile()
{
    float angle = sprite.getRotation();

    if(timeBetweenMissiles.getElapsedTime().asMilliseconds() > 150 && missilecount > 0) {
        playMissileSound();

        missilecount--;

        float xPosBeam;
        float yPosBeam;

        if (launchleft)
        {
            xPosBeam = sprite.getPosition().x - float(cos((angle) / 180 * M_PI)) * 40;
            yPosBeam = sprite.getPosition().y - float(sin((angle) / 180 * M_PI)) * 40;
            launchleft = false;
        }
        else
        {
            xPosBeam = sprite.getPosition().x + float(cos((angle) / 180 * M_PI)) * 40;
            yPosBeam = sprite.getPosition().y + float(sin((angle) / 180 * M_PI)) * 40;
            launchleft = true;
        }

        Missile *missile = new Missile(sf::Vector2f(xPosBeam, yPosBeam), angle, game, speed);

        game.getEntityCollection().addEntity(EntityPtr(missile));

        timeBetweenMissiles.restart();
    }
}

void Spaceship::LaunchGuidedMissile()
{
    float angle = sprite.getRotation();

    if(timeBetweenMissiles.getElapsedTime().asMilliseconds() > 150 && guidedmissilecount > 0) {
        playMissileSound();

        guidedmissilecount--;

        float xPosBeam;
        float yPosBeam;

        if (launchleft)
        {
            xPosBeam = sprite.getPosition().x - float(cos((angle) / 180 * M_PI)) * 40;
            yPosBeam = sprite.getPosition().y - float(sin((angle) / 180 * M_PI)) * 40;
            launchleft = false;
        } else
        {
            xPosBeam = sprite.getPosition().x + float(cos((angle) / 180 * M_PI)) * 40;
            yPosBeam = sprite.getPosition().y + float(sin((angle) / 180 * M_PI)) * 40;
            launchleft = true;
        }

        GuidedMissile *missile = new GuidedMissile(sf::Vector2f(xPosBeam, yPosBeam), angle, game, speed);

        game.getEntityCollection().addEntity(EntityPtr(missile));

        timeBetweenMissiles.restart();
    }
}

float Spaceship::getAngleBetween(EntityPtr entity)
{
    float facingAngle = sprite.getRotation();

    sf::Vector2f targetDir = sprite.getPosition() - entity->sprite.getPosition();

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

    return angleDiff;
}

void Spaceship::updatePropulsionSprites(float delta)
{
    angleInRadians = (float)(sprite.getRotation() * (M_PI / 180));
    if (rocketUpgrade)
    {
        exhaustLeft.setPosition(sf::Vector2f(sprite.getPosition().x - 79 * (float)cos(angleInRadians - M_PI_2), sprite.getPosition().y - 79 * (float)sin(angleInRadians - M_PI_2)));
        exhaustLeft.setRotation(sprite.getRotation());
        exhaustLeft.update(delta);

        exhaustRight.setPosition(sf::Vector2f(sprite.getPosition().x - 79 * (float)cos(angleInRadians - M_PI_2), sprite.getPosition().y - 79 * (float)sin(angleInRadians - M_PI_2)));
        exhaustRight.setRotation(sprite.getRotation());
        exhaustRight.update(delta);
    }
    else
    {
        propeller.setPosition(sf::Vector2f(sprite.getPosition().x - 75 * (float) cos(angleInRadians - M_PI_2),
                                           sprite.getPosition().y - 75 * (float) sin(angleInRadians - M_PI_2)));
        propeller.setRotation(sprite.getRotation());
        propeller.update(delta);

        unsigned propFrameDuration = 128;

        if (speed > 0)
        {
            propFrameDuration = unsigned(unsigned(128) - unsigned(128) * (speed / float(speedLimit)));
            propeller.setScale(sf::Vector2f(1,1));
        }
        else if (speed < 0)
        {
            propFrameDuration = unsigned(unsigned(128) - unsigned(128) * (std::abs(speed) / float(speedLimit)));
            propeller.setScale(sf::Vector2f(-1,1));
        }

        if (propFrameDuration < 6)
            propeller.setStartRect(sf::IntRect(0, 64, 32, 32));
        else if (propFrameDuration < 48)
            propeller.setStartRect(sf::IntRect(0, 32, 32, 32));
        else
            propeller.setStartRect(sf::IntRect(0, 0, 32, 32));

        propeller.setFrameDuration(propFrameDuration);

        soundEffect->sounds[SoundEffect::PROPELLER].setPitch(std::abs(speed) / float(speedLimit) * 3.0f);
    }
}

void Spaceship::playMissileSound()
{
    float x = (float)cos((sprite.getRotation() * (M_PI / 180)));

    if (launchleft)
    {
        soundEffect->sounds[SoundEffect::LEFTMISSILE].setPosition(-x, 0, 1);
        soundEffect->sounds[SoundEffect::LEFTMISSILE].play();
    }
    else
    {
        soundEffect->sounds[SoundEffect::RIGHTMISSILE].setPosition(x, 0, 1);
        soundEffect->sounds[SoundEffect::RIGHTMISSILE].play();
    }
}

void Spaceship::setDrill(Spaceship::Drills drillUpgrade)
{
    if (drillUpgrade == IRON)
    {
        drill.setStartRect(sf::IntRect(0, 0, 128, 64));
        drillSpeed = 50;
    }
    else if (drillUpgrade == STEEL)
    {
        drill.setStartRect(sf::IntRect(0, 64, 128, 64));
        drillSpeed = 90;
    }
    else if (drillUpgrade == TITANIUM)
    {
        drill.setStartRect(sf::IntRect(0, 128, 128, 64));
        drillSpeed = 170;
    }
    else if (drillUpgrade == GOLD)
    {
        drill.setStartRect(sf::IntRect(0, 192, 128, 64));
        drillSpeed = 250;
    }
    drill.reset(); // Just to make sure the new drill is visible straight away

    this->drillUpgrade = drillUpgrade;
}
