#ifndef BINKBIK_GASCLOUD_H
#define BINKBIK_GASCLOUD_H

#include "Entity.h"

class GasCloud : public Entity
{
public:
    GasCloud(sf::Vector2f position, Game &game);

    void draw(sf::RenderWindow &window) override;

    void update(float delta) override;

protected:
    sf::Vector2f position;
};

#endif //BINKBIK_GASCLOUD_H
