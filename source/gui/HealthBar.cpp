#include <iostream>
#include <plog/include/plog/Log.h>

#include "HealthBar.h"

HealthBar::HealthBar(ResourceLoader &rl)
{
    defaultPositionHealthBar.x = 60.0f;
    defaultPositionHealthBar.y = 115.0f;

    defaultPositionHealthHeart.x = 5.0f;
    defaultPositionHealthHeart.y = 115.0f;

    healthHeart.setTexture(*rl.tex_healthHeart);

    healthHeart.setScale(0.08f, 0.09f);

    healthBar.setSize(sf::Vector2f(250.0f, 50.0f));

    healthBar.setFillColor(sf::Color::Red);

    healthLeft.setFont(*rl.font);

    healthLeft.setCharacterSize(20);

    healthLeft.setFillColor(sf::Color::White);

    defaultPositionHealthLeft.x = defaultPositionHealthBar.x + healthBar.getSize().x / 2;

    defaultPositionHealthLeft.y = defaultPositionHealthBar.y + healthBar.getSize().y / 2 - healthLeft.getCharacterSize() / 2;

    init();
    LOGD << "Created new health bar and health heart";
}

void HealthBar::init()
{
    healthBar.setPosition(defaultPositionHealthBar);
    healthHeart.setPosition(defaultPositionHealthHeart);
    healthLeft.setPosition(defaultPositionHealthLeft);
}

void HealthBar::draw(sf::RenderWindow &window, SpaceshipPtr spaceship)
{
    // Scale the width of the health bar relative to healthLeft
    healthBar.setScale(spaceship->health/spaceship->maxHealth, 1.0f);

    healthLeft.setString(std::to_string((int)spaceship->health) + "/" + std::to_string((int)spaceship->maxHealth));
    defaultPositionHealthLeft.x = defaultPositionHealthBar.x + healthBar.getSize().x / 2 - healthLeft.getLocalBounds().width / 2;
    healthLeft.setPosition(defaultPositionHealthLeft);

    window.draw(healthBar);
    window.draw(healthHeart);
    window.draw(healthLeft);
}