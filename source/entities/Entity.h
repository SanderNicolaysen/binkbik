#ifndef BINKBIK_ENTITY_H
#define BINKBIK_ENTITY_H


#include <SFML/Graphics.hpp>
#include <memory>
#include <cereal/types/polymorphic.hpp>

class Game;

class Entity
{
public:
    enum EntityID {
        SPACESHIP,
        ASTEROID,
        PLANET,
        SPACESTATION,
        ENEMY,
        BEAM,
        GASCLOUD,
        SPACEROCK,
        SPACESHIPBEAM,
        SHIELD,
        MISSILE,
        GUIDEDMISSILE
    };

    explicit Entity(Game &game) : game(game) {}

    virtual void draw(sf::RenderWindow &window) {};

    virtual void update(float delta) {};

    EntityID TYPE;

    sf::Sprite sprite;

    sf::Texture* texture;

protected:
    Game &game;
};

typedef std::shared_ptr<Entity> EntityPtr;

#endif //BINKBIK_ENTITY_H
