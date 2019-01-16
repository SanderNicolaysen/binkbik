#include <iostream>
#include <utility>
#include "Game.h"
#include "entities/Enemy.h"
#include "entities/GasCloud.h"
#include "entities/RockBeltGenerator.h"
#include "state/nestedStates/PlanetView.h"

Game::Game(Machine &context, sf::RenderWindow &window, ResourceLoader &rl)
        : State(context, window),
          nestedMachine(context, window, *this),
          generator(time(nullptr)),
          rl(rl),
          fuelBar(rl),
          healthBar(rl),
          energyBar(rl),
          popupText(rl),
          vignette(rl)
{
    placePlanets();

    entities.addEntity(std::make_shared<Spaceship>(*this));
    spaceship = entities.getSpaceship();

    // Put enemies in orbit around a few planets
    entities.addEntity(EntityPtr(new Enemy(entities.getPlanet(3), *this)));
    entities.addEntity(EntityPtr(new Enemy(entities.getPlanet(4), *this)));
    entities.addEntity(EntityPtr(new Enemy(entities.getPlanet(7), *this)));
    entities.addEntity(EntityPtr(new Enemy(entities.getPlanet(8), *this)));
    entities.addEntity(EntityPtr(new Enemy(entities.getPlanet(9), *this)));
    entities.addEntity(EntityPtr(new Enemy(entities.getPlanet(10), *this)));
    entities.addEntity(EntityPtr(new Enemy(entities.getPlanet(11), *this)));
    entities.addEntity(EntityPtr(new Enemy(entities.getPlanet(12), *this)));
    entities.addEntity(EntityPtr(new Enemy(entities.getPlanet(13), *this)));

    placeStaticEntities();

    background.setTexture(*rl.tex_background);
    background.setTextureRect(sf::IntRect(-50000, -50000, 100000, 100000));
    background.setOrigin(background.getLocalBounds().width/2, background.getLocalBounds().height/2);

    soundEffect->openMusicFromFile(soundEffect->exploration, "audio/music/exploration.ogg");
    soundEffect->openMusicFromFile(soundEffect->enemyNearby, "audio/music/enemyNearby.ogg");
}

Game::Game(Machine &context, sf::RenderWindow &window, ResourceLoader &rl, GameSave save)
        : State(context, window),
          nestedMachine(context, window, *this),
          generator(time(nullptr)),
          rl(rl),
          fuelBar(rl),
          healthBar(rl),
          energyBar(rl),
          popupText(rl),
          vignette(rl)
{
    // Create and load spaceship
    entities.addEntity(std::make_shared<Spaceship>(*this));
    spaceship = entities.getSpaceship();
    spaceship->loadState(save.spaceshipState);

    // Load inventory
    spaceship->inventory.loadState(save.inventoryState);

    // Place and load planets
    placePlanets();
    for (auto it: save.planetPositions)
        entities.getPlanet(it.first)->sprite.setPosition(it.second.first, it.second.second);

    // Mark visited planets
    for (auto id: save.planetsVisited)
        entities.getPlanet(id)->visited = true;

    endGame = save.endGame;

    // Place enemies around planets
    for (int id: save.planetIDsWithEnemy)
    {
        entities.addEntity(EntityPtr(new Enemy(entities.getPlanet(id), *this)));
    }

    placeStaticEntities();

    background.setTexture(*rl.tex_background);
    background.setTextureRect(sf::IntRect(-50000, -50000, 100000, 100000));
    background.setOrigin(background.getLocalBounds().width/2, background.getLocalBounds().height/2);
}

void Game::placeStaticEntities()
{
    // Place spacestation
    entities.addEntity(std::make_shared<Spacestation>(*this));

    // Place a hostile meteorite belt around some planets
    RockBeltGenerator rockBeltGenerator(*this);
    rockBeltGenerator(entities.getPlanet(3));
    rockBeltGenerator(entities.getPlanet(4));
    rockBeltGenerator(entities.getPlanet(8));
    rockBeltGenerator(entities.getPlanet(10));
    rockBeltGenerator(entities.getPlanet(11));

    // Place 7 GasClouds
    entities.addEntity(EntityPtr(new GasCloud(sf::Vector2f(500000, 0), *this)));
    entities.addEntity(EntityPtr(new GasCloud(sf::Vector2f(500000, 0), *this)));
    entities.addEntity(EntityPtr(new GasCloud(sf::Vector2f(500000, 0), *this)));
    entities.addEntity(EntityPtr(new GasCloud(sf::Vector2f(500000, 0), *this)));
    entities.addEntity(EntityPtr(new GasCloud(sf::Vector2f(500000, 0), *this)));
    entities.addEntity(EntityPtr(new GasCloud(sf::Vector2f(500000, 0), *this)));
    entities.addEntity(EntityPtr(new GasCloud(sf::Vector2f(500000, 0), *this)));
    moveGasClouds(); // Put the gasclouds in random positions

    // Shield
    entities.addEntity(EntityPtr(new Shield(sf::Vector2f(spaceship->sprite.getPosition().x, spaceship->sprite.getPosition().y), *this)));
    shield = std::dynamic_pointer_cast<Shield>(entities.getEntityRange(Entity::SHIELD).begin()->second);
}

GameSave Game::getSave()
{
    GameSave save;

    // Save spaceship variables and inventory
    save.spaceshipState = spaceship->getState();
    save.inventoryState = spaceship->inventory.getState();

    // Save enemies
    for (auto it: entities.getEntityRange(Entity::ENEMY))
    {
        EnemyPtr enemy = std::dynamic_pointer_cast<Enemy>(it.second);
        save.planetIDsWithEnemy.push_back(enemy->planet->planetID);
    }

    // Save planet positions and planets visited
    for (auto it: entities.getEntityRange(Entity::PLANET))
    {
        PlanetPtr planet = std::dynamic_pointer_cast<Planet>(it.second);
        save.planetPositions[planet->planetID] = std::pair<float, float>(planet->sprite.getPosition().x, planet->sprite.getPosition().y);

        if (planet->visited)
            save.planetsVisited.push_back(planet->planetID);
    }

    save.endGame = endGame;

    return save;
}

void Game::enter()
{
    gameView.setSize(window.getDefaultView().getSize());
    gameView.zoom(1.25);

    clock.restart();

    soundEffect->exploration.play();

    soundEffect->exploration.setLoop(true);
    soundEffect->enemyNearby.setLoop(true);

    window.setMouseCursorVisible(false);
}

void Game::run()
{
    if (nestedMachine.getRunning())
    {
        soundEffect->stopSfx();
        nestedState();
        return;
    }

    handleEvents();

    updateAll();

    window.clear(backgroundColor);

    drawAll();

    window.display();
}

// Function that enables specific states to run on top of the game state
void Game::nestedState()
{
    // Restart/stop clock when entering child state
    clock.restart();

    // Set center of view back to spaceship
    window.setView(gameView);

    window.clear(backgroundColor);

    drawAll();

    nestedMachine.go();

    window.display();
}

void Game::exit()
{
    soundEffect->stopSfx();
}

void Game::handleEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                context.setState(context.EXIT);
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        nestedMachine.setState(NestedMachine::PAUSE);
                        nestedMachine.setRunning(true);
                        break;

                    /* Uncomment for testing/debugging
                    case sf::Keyboard::Subtract:
                        gameView.zoom(1.1);
                        break;
                    case sf::Keyboard::Add:
                        gameView.zoom(0.9);
                        break;
                    case sf::Keyboard::A:
                        entities.erase(entities.getEntityRange(Entity::ASTEROID));
                        break;
                    */

                    case sf::Keyboard::S:
                        if (!shield->upgrade)
                            break;

                        if (shield->health <= 0)
                            break;

                        if (!shield->activated)
                            soundEffect->sounds[SoundEffect::ACTIVATE_SHIELD].play();

                        shield->scale = 0.0f;
                        shield->timer.restart();
                        shield->activated = !shield->activated;
                        break;
                    default:
                        break;
                }
            default:
                break;
        }

        cheatCode(event);
    }
}

void Game::updateAll()
{
    // Get time since last update (in seconds)
    float delta = clock.restart().asSeconds();

    placeAsteroids();

    // Move the gasclouds once ever minute
    if (clockGasCloud.getElapsedTime().asSeconds() > 60)
    {
        clockGasCloud.restart();
        moveGasClouds();
    }

    for (auto it: entities) {
        EntityPtr entity = it.second;
        // Delete asteroids further than 20000 px away from the ship
        if (entity->TYPE == Entity::ASTEROID && getDistance(entity, entities.getSpaceship()) > 20000)
        {
            entities.deleteEntity(entity);
        }
        // Delete beams further than 1960 px away from the ship
            // TODO: Move this to Enemy.cpp or Beam.cpp? Erase beam after some time using sf::Clock instead?
        if (entity->TYPE == Entity::BEAM && getDistance(entities.getSpaceship(), entity) > 5000)
            entities.deleteEntity(entity);

        if (entity->TYPE == Entity::SPACESHIPBEAM && getDistance(entities.getSpaceship(), entity) > 5000)
            entities.deleteEntity(entity);
    }

    if (spaceship->fuel <= 0)
        context.setState(context.END);
    if (spaceship->health <= 0)
        context.setState(context.END);

    // Update all the entities
    entities.updateAll(delta);

    // Make the view follow the spaceship so it's always centered.
    float deltax = gameView.getCenter().x - spaceship->sprite.getPosition().x;
    float deltay = gameView.getCenter().y - spaceship->sprite.getPosition().y;
    if (std::abs(deltax) > 1 || std::abs(deltay) > 1)
    {
        gameView.setCenter(float(gameView.getCenter().x - 0.1 * deltax),float(gameView.getCenter().y - 0.1 * deltay));
    }

    window.setView(gameView);
}

void Game::drawAll()
{
    // Parallax scrolling on background
    background.setPosition(spaceship->sprite.getPosition().x/6, spaceship->sprite.getPosition().y/6);

    // Draw background
    window.draw(background);

    // Draw all our entities!
    entities.drawAll(window);

    // Change to the "UI camera/view", which doesnt move with the world
    window.setView(sf::View(sf::FloatRect(0,0,window.getSize().x,
                                          window.getSize().y)));

    vignette.draw(window);
    fuelBar.draw(window, spaceship);
    healthBar.draw(window, spaceship);
    energyBar.draw(window, spaceship);
    spaceship->inventory.draw(window);
    popupText.draw(window);
}

/***
 * Genereates planets at an increasing radius from map centre at a random angle.
 */
void Game::placePlanets()
{
    std::uniform_real_distribution<double> rngangle(0,2);

    int distance = 1500;

    for (int i = 1; i < 14; i++)
    {
        // To make sure the planets aren't very heavily concentrated on the north or south side of the map,
        // odd and even planets are placed on north and south side of the map, respectively.
        double rangle = -1;
        if (i % 2 == 0)
        {
            while(rangle < 1 || rangle < 0)
                rangle = rngangle(generator);
        }
        else
        {
            while(rangle >= 1 || rangle < 0)
                rangle = rngangle(generator);
        }

        // Convert point on circle with distance radius and rangle angle to a vector
        sf::Vector2f position(
                float(std::cos(rangle * M_PI) * distance),
                float(std::sin(rangle * M_PI) * distance));

        // Distance increases less with every new planet, to make sure the outermost planets aren't too far apart.
        distance += 3500 - i * 100;

        entities.addEntity(EntityPtr(new Planet(i, position, *this)));
    }
    LOG_INFO << "Generated planets.";
}

void Game::placeAsteroids()
{
    while (entities.count(Entity::ASTEROID) < 800)
    {
        double rangle = rngangle(generator);
        double radius = 0;
        while (radius < 1000)
           radius = 20000 - 8000*rngradius(generator);

        sf::Vector2f position(
                float(spaceship->sprite.getPosition().x + std::cos(rangle * M_PI) * radius),
                float(spaceship->sprite.getPosition().y + std::sin(rangle * M_PI) * radius));

        bool goodposition = true;

        for (auto it: entities)
        {
            EntityPtr entity = it.second;
            if (entity->sprite.getGlobalBounds().intersects(sf::FloatRect(position.x - 200, position.y - 200, 400, 400)))
            {
                goodposition = false;
                break;
            }
        }
        if (goodposition)
        {
            entities.addEntity(AsteroidPtr(new Asteroid(position, *this)));
        }
    }
}

void Game::setNestedState(NestedMachine::StateID state)
{
    nestedMachine.setState(state);
    nestedMachine.setRunning(true);
}

void Game::setPlanetView(int planetID)
{
    nestedMachine.resetState(NestedMachine::PLANETVIEW, new PlanetView(nestedMachine, context, window, planetID));
}

void Game::displayPopupText(std::string text)
{
    popupText.setText(std::move(text));
}

EntityCollection& Game::getEntityCollection()
{
    return entities;
}

ResourceLoader& Game::getResourceLoader()
{
    return rl;
}

double Game::getDistance(EntityPtr entity1, EntityPtr entity2)
{
    return sqrt(pow(entity1->sprite.getPosition().x - entity2->sprite.getPosition().x, 2) +
                pow(entity1->sprite.getPosition().y - entity2->sprite.getPosition().y, 2));
}

double Game::getDistance(const sf::Sprite &sprite1, const sf::Sprite &sprite2)
{
    return sqrt(pow(sprite1.getPosition().x - sprite2.getPosition().x, 2) +
                pow(sprite1.getPosition().y - sprite2.getPosition().y, 2));
}

void Game::flashVignette(sf::Color color)
{
    vignette.setColor(color);
}

void Game::endGameView()
{
    context.setState(Machine::GAME_FINISHED);
}

// Function for enabling different functionality in the game for debugging purposes
void Game::cheatCode(sf::Event& event)
{
    /* Uncomment to activate cheats for debugging and testing purposes.
     *
    switch (event.type)
    {
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Numpad1:
                    spaceship->health = spaceship->maxHealth;
                std::cout << "1. MAX HEALTH" << std::endl;
                    break;
                case sf::Keyboard::Numpad2:
                    spaceship->fuel = spaceship->maxFuel;
                    std::cout << "2. MAX FUEL" << std::endl;
                    break;
                case sf::Keyboard::Numpad3:
                    shield->upgrade = true;
                    std::cout << "3. SHIELD UPGRADE" << std::endl;
                    break;
                case sf::Keyboard::Numpad4:
                    spaceship->homeCompass = new Compass(*this, this->getEntityCollection().getSpacestation(), sf::Color(122,122,255,80));
                    spaceship->planetCompass = new Compass(*this, Entity::PLANET, sf::Color(122,255,122,80));
                    spaceship->enemyCompass = new Compass(*this, Entity::ENEMY, sf::Color(255,122,122,80));
                    std::cout << "4. ACTIVATE COMPASS" << std::endl;
                    break;
                case sf::Keyboard::Numpad5:
                    spaceship->speedLimit += 40;
                    std::cout << "5. INCREASE SPEED LIMIT" << std::endl;
                    break;
                case sf::Keyboard::Numpad6:
                    spaceship->inventory.addCash(50);
                    std::cout << "6. CHEAT +50$" << std::endl;
                    break;
                default:
                    break;
            }
        default:
            break;
    }
    */
}

void Game::moveGasClouds()
{
    std::uniform_real_distribution<float> rngangle(0, float(2 * M_PI));
    std::uniform_real_distribution<float> rngradius(3000, 35000);

    for(auto it: entities.getEntityRange(Entity::GASCLOUD))
    {
        std::shared_ptr<GasCloud> cloud = std::dynamic_pointer_cast<GasCloud>(it.second);

        // Don't move the gascloud if the spaceship is near
        if (getDistance(cloud->sprite, spaceship->sprite) < 3000)
            continue;

        bool goodPos = false;
        do {
            float angle = rngangle(generator);
            float radius = rngradius(generator);

            sf::Vector2f position(std::cos(angle) * radius, std::sin(angle) * radius);

            cloud->sprite.setPosition(position);

            goodPos = true;

            // Make sure we don't put it on top of a planet
            for (auto iter: entities.getEntityRange(Entity::PLANET))
            {
                if (getDistance(cloud->sprite, iter.second->sprite) < 1200)
                {
                    goodPos = false;
                    break;
                }
            }

            // Never place a gas cloud close enough to the spaceship that you can see it spawn
            if (getDistance(cloud->sprite, spaceship->sprite) < 2000)
                goodPos = false;
        }
        while (!goodPos);
    }
}
