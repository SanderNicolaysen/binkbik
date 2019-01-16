#ifndef BINKBIK_METEORITEBELT_H
#define BINKBIK_METEORITEBELT_H


#include <random>
#include <chrono>
#include "Entity.h"
#include "Spacerock.h"
#include "state/Game.h"

class RockBeltGenerator
{
public:
    explicit RockBeltGenerator(Game &game) : game(game)
    {
        // Seed the generator
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        g.seed(seed);
    }

    void operator()(PlanetPtr planet)
    {
        radius.reset();
        speed.reset();
        scale.reset();
        angle.reset();

        for (int i = 0; i < 200; ++i) {
            SpacerockPtr rock(new Spacerock(game, planet, radius(g), speed(g), scale(g), angle(g)));
            game.getEntityCollection().addEntity(rock);
        }
    }

protected:
    Game &game;

    std::default_random_engine g;

    std::uniform_real_distribution<float> radius{4000, 4400};
    std::uniform_real_distribution<float> speed{float(M_PI/10), float(M_PI_4)};
    std::uniform_real_distribution<float> scale{0.5f, 1.5f};
    std::uniform_real_distribution<float> angle{0, float(2*M_PI)};
};


#endif //BINKBIK_METEORITEBELT_H
