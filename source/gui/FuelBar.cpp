#include <iostream>
#include <plog/include/plog/Log.h>

#include "FuelBar.h"

FuelBar::FuelBar(ResourceLoader &rl)
{
    defaultPositionFuelBar.x = 60.0f;
    defaultPositionFuelBar.y = 50.0f;

    defaultPositionFuelCan.x = 5.0f;
    defaultPositionFuelCan.y = 50.0f;

    fuelCan.setTexture(*rl.tex_fuelcan);

    fuelCan.setScale(0.05f, 0.04f);

    fuelBar.setSize(sf::Vector2f(250.0f, 50.0f));

    fuelBar.setFillColor(sf::Color::Blue);

    fuelLeft.setFont(*rl.font);

    fuelLeft.setCharacterSize(20);

    fuelLeft.setFillColor(sf::Color::White);

    defaultPositonFuelLeft.x = defaultPositionFuelBar.x + fuelBar.getSize().x / 2;

    defaultPositonFuelLeft.y = defaultPositionFuelBar.y + fuelBar.getSize().y / 2 - fuelLeft.getCharacterSize() / 2;

    init();
    LOGD << "Created new fuel bar and fuel can";
}

void FuelBar::init()
{
    fuelBar.setPosition(defaultPositionFuelBar);
    fuelCan.setPosition(defaultPositionFuelCan);
    fuelLeft.setPosition(defaultPositonFuelLeft);
}

void FuelBar::draw(sf::RenderWindow &window, SpaceshipPtr spaceship)
{
    // Scale the width of the fuel bar relative to fuelLeft
    fuelBar.setScale(spaceship->fuel/spaceship->maxFuel, 1.0f);

    fuelLeft.setString(std::to_string((int)spaceship->fuel) + "/" + std::to_string((int)spaceship->maxFuel));
    defaultPositonFuelLeft.x = defaultPositionFuelBar.x + fuelBar.getSize().x / 2 - fuelLeft.getLocalBounds().width / 2;
    fuelLeft.setPosition(defaultPositonFuelLeft);

    window.draw(fuelBar);
    window.draw(fuelCan);
    window.draw(fuelLeft);
}