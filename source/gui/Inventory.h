#ifndef BINKBIK_INVENTORY_H
#define BINKBIK_INVENTORY_H


#include <SFML/Graphics.hpp>
#include <GameSave.h>
#include "entities/Asteroid.h"
#include "ResourceLoader.h"

class Inventory
{
public:
    explicit Inventory(ResourceLoader &rl);

    void loadState(GameSave::InventoryState state);

    GameSave::InventoryState getState();

    void draw(sf::RenderWindow &window);

    void addResource(Asteroid::ResourceID id, int count = 1);

    bool useResource(Asteroid::ResourceID id, int count);

    int count(Asteroid::ResourceID id);

    int countAll();

    void addCash(int amount);

    bool spendCash(int amount);

    int value(Asteroid::ResourceID id);

    void sellResources();

    int maxCargo = 25;

    int cash = 0;

protected:
    std::map<Asteroid::ResourceID, std::tuple<int, sf::Text, sf::Sprite, int>> resources;

    sf::Texture texture;

    sf::Sprite sprite;

    sf::Font font;

    sf::Text textCargo;

    sf::Text textCash;
};


#endif //BINKBIK_INVENTORY_H
