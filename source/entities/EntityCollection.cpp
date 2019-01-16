#include "EntityCollection.h"

void EntityCollection::addEntity(EntityPtr entity)
{
    entities.emplace(entity->TYPE, entity);
    entityCounts[entity->TYPE]++;
}

SpaceshipPtr EntityCollection::getSpaceship()
{
    return std::dynamic_pointer_cast<Spaceship>(entities.find(Entity::SPACESHIP)->second);
}

SpacestationPtr EntityCollection::getSpacestation()
{
    return std::dynamic_pointer_cast<Spacestation>(entities.find(Entity::SPACESTATION)->second);
}

PlanetPtr EntityCollection::getPlanet(int id)
{
    if (id > entities.count(Entity::PLANET))
        return nullptr;

    // Find the first Planet and iterate until we reach the requested one
    auto it = entities.find(Entity::PLANET);
    for (int i = 0; i < id - 1; ++i) {
        it++;
    }

    return std::dynamic_pointer_cast<Planet>(it->second);
}

Entities::iterator EntityCollection::begin()
{
    return entities.begin();
}

Entities::iterator EntityCollection::end()
{
    return entities.end();
}

void EntityCollection::deleteEntity(const Entity *entity)
{
    auto r = entities.equal_range(entity->TYPE);
    for (auto it = r.first; it != r.second; it++)
        if (entity == it->second.get())
            deleteEntity(it);
}

void EntityCollection::deleteEntity(EntityPtr entity)
{
    // Find the Entity and mark it to be deleted
    auto r = entities.equal_range(entity->TYPE);
    for (auto it = r.first; it != r.second; it++)
        if (entity == it->second)
            deleteEntity(it);
}

void EntityCollection::erase(EntityRange range)
{
    for (auto it = range.begin(); it != range.end(); it++)
        deleteEntity(it);
}

void EntityCollection::deleteEntity(Entities::iterator iter)
{
    if (toBeDeleted.find(iter->second) != toBeDeleted.end())
        return;

    // Mark this entity to be deleted
    toBeDeleted[iter->second] = iter;
    entityCounts[iter->first]--;
}

unsigned long long int EntityCollection::size()
{
    return entities.size();
}

void EntityCollection::updateAll(float delta)
{
    for (auto &e: entities)
        e.second->update(delta);

    // Delete all marked entities
    for (auto it: toBeDeleted) entities.erase(it.second);
    toBeDeleted.clear();
}

void EntityCollection::drawAll(sf::RenderWindow &window)
{
    for (auto &e: entities)
        e.second->draw(window);
}

EntityRange EntityCollection::getEntityRange(Entity::EntityID key_type)
{
    auto range = entities.equal_range(key_type);
    return {range.first, range.second};
}

const unsigned int EntityCollection::count(Entity::EntityID entityID)
{
    return entityCounts[entityID];
}

EntityRange::EntityRange(const Entities::iterator &_begin, const Entities::iterator &_end)
        : _begin(_begin),
          _end(_end) {}
