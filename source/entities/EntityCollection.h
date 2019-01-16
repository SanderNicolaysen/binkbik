#ifndef BINKBIK_ENTITYCOLLECTION_H
#define BINKBIK_ENTITYCOLLECTION_H


#include <map>
#include "Entity.h"
#include "Spaceship.h"
#include "Asteroid.h"
#include "Spacestation.h"
#include "Planet.h"

typedef std::multimap<Entity::EntityID, EntityPtr> Entities;

class EntityRange
{
public:
    EntityRange(const Entities::iterator  &_begin,
          const Entities::iterator  &_end);

    Entities::iterator begin() { return _begin; };

    Entities::iterator end() { return _end; };

protected:
    Entities::iterator _begin;

    Entities::iterator _end;
};

class EntityCollection
{
public:
    void addEntity(EntityPtr entity);

    void deleteEntity(const Entity *entity);

    void deleteEntity(EntityPtr entity);

    void deleteEntity(Entities::iterator iter);

    void erase(EntityRange range);

    unsigned long long int size();

    Entities::iterator begin();

    Entities::iterator end();

    SpaceshipPtr getSpaceship();

    SpacestationPtr getSpacestation();

    PlanetPtr getPlanet(int id);

    EntityRange getEntityRange(Entity::EntityID key_type);

    void updateAll(float delta);

    void drawAll(sf::RenderWindow &window);

    const unsigned int count(Entity::EntityID entityID);

protected:
    Entities entities;

    std::map<EntityPtr, Entities::iterator> toBeDeleted;

    std::map<Entity::EntityID, unsigned int> entityCounts;
};


#endif //BINKBIK_ENTITYCOLLECTION_H
