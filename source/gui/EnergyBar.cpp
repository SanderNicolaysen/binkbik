#include <iostream>
#include <plog/include/plog/Log.h>

#include "EnergyBar.h"

EnergyBar::EnergyBar(ResourceLoader &rl)
{
    defaultPositionEnergyBar.x = 60.0f;
    defaultPositionEnergyBar.y = 180.0f;

    defaultPositionLightning.x = 2.0f;
    defaultPositionLightning.y = 178.0f;

    lightning.setTexture(*rl.tex_energy);

    lightning.setScale(0.45f, 0.45f);

    energyBar.setSize(sf::Vector2f(250.0f, 50.0f));

    energyBar.setFillColor(sf::Color(200,200,0));

    energyLeft.setFont(*rl.font);

    energyLeft.setCharacterSize(20);

    energyLeft.setFillColor(sf::Color::White);

    defaultPositonEnergyLeft.x = defaultPositionEnergyBar.x + energyBar.getSize().x / 2;

    defaultPositonEnergyLeft.y = defaultPositionEnergyBar.y + energyBar.getSize().y / 2 - energyLeft.getCharacterSize() / 2;

    init();
    LOGD << "Created new energy bar";
}

void EnergyBar::init()
{
    energyBar.setPosition(defaultPositionEnergyBar);
    lightning.setPosition(defaultPositionLightning);
    energyLeft.setPosition(defaultPositonEnergyLeft);
}

void EnergyBar::draw(sf::RenderWindow &window, SpaceshipPtr spaceship)
{
    // Scale the width of the fuel bar relative to fuelLeft
    energyBar.setScale(spaceship->energy/100, 1.0f);

    energyLeft.setString(std::to_string((int)spaceship->energy) + "/" + "100");
    defaultPositonEnergyLeft.x = defaultPositionEnergyBar.x + energyBar.getSize().x / 2 - energyLeft.getLocalBounds().width / 2;
    energyLeft.setPosition(defaultPositonEnergyLeft);

    window.draw(energyBar);
    window.draw(lightning);
    window.draw(energyLeft);
}