#ifndef BINKBIK_SPACEROCK_H
#define BINKBIK_SPACEROCK_H


#include "Entity.h"
#include "Planet.h"

class Spacerock : public Entity
{
public:
    Spacerock(Game &game, PlanetPtr planet, float radius, float speed, float scale, float startPos);

    void draw(sf::RenderWindow &window) override;

    void update(float delta) override;

    time_t lastImpact = time(NULL);

protected:
    PlanetPtr planet;

    float angle; // Current angle/position

    float V_a; // Angular velocity

    float radius; // Orbit radius
};

typedef std::shared_ptr<Spacerock> SpacerockPtr;

#endif //BINKBIK_SPACEROCK_H
