#ifndef BINKBIK_GAME_H
#define BINKBIK_GAME_H

#include <cmath>
#include <random>
#include <gui/EnergyBar.h>
#include "State.h"
#include "entities/EntityCollection.h"
#include "nestedStates/NestedMachine.h"
#include "ResourceLoader.h"
#include "gui/FuelBar.h"
#include "gui/HealthBar.h"
#include "gui/PopupText.h"
#include "gui/Vignette.h"
#include "entities/Shield.h"

class Compass;

class Game : public State
{
public:
    Game(Machine &context, sf::RenderWindow &window, ResourceLoader &rl);

    Game(Machine &context, sf::RenderWindow &window, ResourceLoader &rl, GameSave save);

    void placeStaticEntities();

    GameSave getSave();

    void enter() override;

    void run() override;

    void exit() override;

    void handleEvents();

    void updateAll();

    void drawAll();

    void placePlanets();

    void nestedState();

    void setNestedState(NestedMachine::StateID state);

    void setPlanetView(int planetID);

    void displayPopupText(std::string text);

    void flashVignette(sf::Color color);

    void cheatCode(sf::Event& e);

    EntityCollection& getEntityCollection();

    ResourceLoader& getResourceLoader();

    double getDistance(EntityPtr entity1, EntityPtr entity2);

    double getDistance(const sf::Sprite &sprite1, const sf::Sprite &sprite2);

    std::default_random_engine generator;

    bool endGame = false;

    void endGameView();

protected:

    EntityCollection entities;

    SpaceshipPtr spaceship;

    sf::View gameView;

    NestedMachine nestedMachine;

    sf::Clock clock;

    sf::Color backgroundColor{0, 0, 0};

    sf::Sprite background;

    ResourceLoader &rl;

    FuelBar fuelBar{rl};

    HealthBar healthBar{rl};

    EnergyBar energyBar{rl};

    PopupText popupText;

    Vignette vignette;

    ShieldPtr shield;

    void placeAsteroids();

    sf::Clock clockGasCloud;

    void moveGasClouds();

    std::uniform_real_distribution<double> rngangle{0,2};

    std::exponential_distribution<double> rngradius{1};

    SoundEffect* soundEffect = SoundEffect::Instance();
};

#endif //BINKBIK_GAME_H
