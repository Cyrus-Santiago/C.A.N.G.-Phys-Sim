// this code was written by Nate

#ifndef ENTITYCOMPONENTSYSTEM_H
#define ENTITYCOMPONENTSYSTEM_H

#include <queue>
#include <cstdint>
#include <bitset>

#define ENTITYMAX 5000
#define COMPONENTMAX 32

class EntityComponentSystem {
public:

    typedef struct {
        std::uint32_t ID;
        std::bitset<COMPONENTMAX> ComponentFlags;
    } Entity;

    // list of usable entity IDs
    static std::queue<Entity> AvailableEntityIDs;

    // initializes entire EntityID array
    EntityComponentSystem();
    
    // creates a new entity, returns the ID
    Entity CreateEntity();

    // deletes an entity
    void DeleteEntity(Entity entity);

    // adds a component to an entity
    void AddComponent(Entity entity, uint8_t componentID);    

    // removes a component
    void RemoveComponent(Entity entity, uint8_t componentID);

    // checks if an entity has a component
    bool EntityHasComponent(Entity entity, uint8_t componentID);
};

#endif