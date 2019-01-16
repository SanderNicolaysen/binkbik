#ifndef BINKBIK_SPACESTATION_H
#define BINKBIK_SPACESTATION_H

#include <SFML/Graphics.hpp>
#include "Entity.h"


class Spacestation : public Entity
{
public:
    explicit Spacestation(Game &game);

    void draw(sf::RenderWindow &window) override;

    void update(float delta) override;

protected:
    sf::Vector2f position;
};
typedef std::shared_ptr<Spacestation> SpacestationPtr;


#endif //BINKBIK_SPACESTATION_H
