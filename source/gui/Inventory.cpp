#include <plog/include/plog/Log.h>
#include <iostream>
#include "Inventory.h"

Inventory::Inventory(ResourceLoader &rl)
{
    sprite.setTexture(*rl.tex_resources);

    // Initialize resources map
    resources[Asteroid::ROCK] = std::make_tuple(0, sf::Text("0", *rl.font, 20), sf::Sprite(*rl.tex_resources, sf::IntRect(0, 0, 64, 64)), 1);
    resources[Asteroid::IRON] = std::make_tuple(0, sf::Text("0", *rl.font, 20), sf::Sprite(*rl.tex_resources, sf::IntRect(64, 0, 64, 64)), 2);
    resources[Asteroid::SILVER] = std::make_tuple(0, sf::Text("0", *rl.font, 20), sf::Sprite(*rl.tex_resources, sf::IntRect(128, 0, 64, 64)), 3);
    resources[Asteroid::IRIDIUM] = std::make_tuple(0, sf::Text("0", *rl.font, 20), sf::Sprite(*rl.tex_resources, sf::IntRect(192, 0, 64, 64)), 4);
    resources[Asteroid::GOLD] = std::make_tuple(0, sf::Text("0", *rl.font, 20), sf::Sprite(*rl.tex_resources, sf::IntRect(256, 0, 64, 64)), 5);
    resources[Asteroid::PLATINUM] = std::make_tuple(0, sf::Text("0", *rl.font, 20), sf::Sprite(*rl.tex_resources, sf::IntRect(320, 0, 64, 64)), 6);
    resources[Asteroid::OGRIUM] = std::make_tuple(0, sf::Text("0", *rl.font, 20), sf::Sprite(*rl.tex_resources, sf::IntRect(384, 0, 64, 64)), 10);
    resources[Asteroid::SPACEIUM] = std::make_tuple(0, sf::Text("0", *rl.font, 20), sf::Sprite(*rl.tex_resources, sf::IntRect(448, 0, 64, 64)), 15);

    textCargo.setFont(*rl.font);

    textCargo.setCharacterSize(20);

    textCash.setFont(*rl.font);
    textCash.setCharacterSize(24);
}

void Inventory::loadState(GameSave::InventoryState state)
{
    cash = state.cash;
    maxCargo = state.maxCargo;

    for (auto it: state.resources)
    {
        std::get<0>(resources[it.first]) = it.second;
    }
}

GameSave::InventoryState Inventory::getState()
{
    GameSave::InventoryState state;
    state.cash = cash;
    state.maxCargo = maxCargo;

    for (auto it: resources)
    {
        state.resources[it.first] = std::get<0>(resources[it.first]);
    }

    return state;
}

void Inventory::draw(sf::RenderWindow &window)
{
    // Right and top bounds for the inventory items
    int rightBound = window.getSize().x - 30;
    int topBound = 25;

    // Draw currency
    textCash.setString(std::to_string(cash) + "$");
    textCash.setPosition(rightBound - textCash.getGlobalBounds().width, topBound);
    window.draw(textCash);

    topBound += textCash.getGlobalBounds().height + 15;

    // Draw capacity
    textCargo.setString("Cargo: " + std::to_string(countAll()) + "/" + std::to_string(maxCargo));
    textCargo.setPosition(rightBound - textCargo.getGlobalBounds().width, topBound);
    window.draw(textCargo);

    int yPos = topBound + int(textCargo.getGlobalBounds().height) + 15;

    int count, value;
    sf::Text text;
    sf::Sprite sprite;
    for (auto &it: resources)
    {
        // Unpack the tuple into count, text, sprite and value
        std::tie(count, text, sprite, value) = it.second;

        // Only draw resources that we have
        if (count > 0)
        {
            sprite.setPosition(rightBound - sprite.getGlobalBounds().width, yPos - 19);
            text.setString(std::to_string(count));
            text.setPosition(sprite.getGlobalBounds().left - text.getGlobalBounds().width, yPos);

            window.draw(sprite);
            window.draw(text);

            yPos += 35;
        }
    }
}

void Inventory::addResource(Asteroid::ResourceID id, int count)
{
    if (countAll() >= maxCargo)
        return;

    std::get<0>(resources[id]) += count;
}

bool Inventory::useResource(Asteroid::ResourceID id, int count)
{
    if (std::get<0>(resources[id]) < count)
        return false;

    std::get<0>(resources[id]) -= count;
}

int Inventory::count(Asteroid::ResourceID id)
{
    return std::get<0>(resources[id]);
}

int Inventory::countAll()
{
    int count = 0;
    for (auto &it: resources)
        count += this->count(it.first);
    return count;
}

void Inventory::addCash(int amount)
{
    cash += amount;
}

bool Inventory::spendCash(int amount)
{
    if (cash < amount)
        return false;

    cash -= amount;
    return true;
}

int Inventory::value(Asteroid::ResourceID id)
{
    return std::get<3>(resources[id]);
}

void Inventory::sellResources()
{
    for (auto &it : resources)
    {
        int resourceQuantity = count(it.first);
        int resourceValue = value(it.first);

        useResource(it.first, resourceQuantity);

        addCash(resourceQuantity * resourceValue);
    }
}