// this code was written by Nate

#include "../include/ecs.hpp"
#include <cstdint>

// constructor
EntityComponentSystem::EntityComponentSystem() {
    Entity entity;
    // create an entity ID for every possible entity
    for (uint32_t i = 0; i < ENTITYMAX; i++) {
        // assigns current iterator value as entity ID
        entity.ID = i;
        // sets all component bits to zero (even though they should be already)
        entity.ComponentFlags.reset();
        // push the entity onto the queue
        AvailableEntityIDs.push(entity);
    }
}

EntityComponentSystem::Entity EntityComponentSystem::CreateEntity() {
    Entity entity;
    // gets entity from front of queue
    entity = AvailableEntityIDs.front();
    // remove entity from queue so no one else tries to take it
    AvailableEntityIDs.pop();
    return entity;
}

void EntityComponentSystem::DeleteEntity(Entity entity) {
    // set all component flags to zero
    entity.ComponentFlags.reset();
    // push entity back to end of queue
    AvailableEntityIDs.push(entity);
}

void EntityComponentSystem::AddComponent(Entity entity, uint8_t componentID) {
    // set the bit for the specified component ID
    entity.ComponentFlags.set(componentID, 1);
}

void EntityComponentSystem::RemoveComponent(Entity entity,
    uint8_t componentID) {
    // reset bit for specified component ID to zero
    entity.ComponentFlags.reset(componentID);
}

bool EntityComponentSystem::EntityHasComponent(Entity entity,
    uint8_t componentID) {
    return entity.ComponentFlags.test(componentID);
}