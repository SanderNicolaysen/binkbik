#ifndef BINKBIK_GAMESTATE_H
#define BINKBIK_GAMESTATE_H


#include "entities/Asteroid.h"

class GameSave {
public:
    struct SpaceshipState
    {
        int speedLimit, acceleration, rotationSpeed;

        float rotation, fuel, maxFuel, health, maxHealth, speed;

        bool rocketUpgrade;

        unsigned int drillUpgrade;

        int missilecount, guidedmissilecount;

        std::pair<float, float> position;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(speedLimit,
                    acceleration,
                    rotationSpeed,
                    rotation,
                    fuel,
                    maxFuel,
                    health,
                    maxHealth,
                    speed,
                    rocketUpgrade,
                    drillUpgrade,
                    missilecount,
                    guidedmissilecount,
                    position
            );
        }
    };

    struct InventoryState
    {
        int cash;

        int maxCargo;

        std::map<Asteroid::ResourceID, int> resources;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive(cash, maxCargo, resources);
        }
    };


    std::map<int, std::pair<int, int>> planetPositions;

    std::vector<int> planetsVisited;

    bool endGame;

    std::vector<int> planetIDsWithEnemy;

    SpaceshipState spaceshipState;

    InventoryState inventoryState;

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive & archive)
    {
        // serialize things by passing them to the archive
        archive(spaceshipState, inventoryState, planetPositions, planetsVisited, endGame, planetIDsWithEnemy);
    }
};


#endif //BINKBIK_GAMESTATE_H
