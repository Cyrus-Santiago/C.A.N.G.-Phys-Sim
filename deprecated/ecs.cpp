/*
//DEPCRECATED, PLACE IN "src" DIRECTORY TO COMPILE
// this code was written by Nate

#include "../include/ecs.hpp"
#include "../include/resourceManager.hpp"
#include <cstdarg>
#include <cstdint>
#include <iostream>

// queue of available entity IDs
std::queue<ECS::Entity>
AvailableEntityIDs;

// map from entity ID to it's components
std::map<uint32_t, ECS::Components>
EntityToComponents;

// constructor
ECS::ECS() {

    ECS::Entity entity;
    // create an entity ID for every possible entity
    for (uint32_t i = 0; i < ENTITYMAX; i++) {
        // assigns current iterator value as entity ID
        entity.ID = i;
        // sets all component bits to zero (even though they should be already)
        entity.ComponentBitMask.reset();
        // push the entity onto the queue
        ECS::AvailableEntityIDs.push(entity);

        // intialize components for entity
        ECS::Dimension dimension = {50, 150, 10, 10, "button2"};
        ECS::Physics physics = {true};
        ECS::Components componentList = {dimension, physics};

        // map entity to it's components
        ECS::EntityToComponents.insert(
            std::pair<uint32_t,
            ECS::Components>(entity.ID, componentList));
    }
}

ECS::Entity ECS::CreateEntity() {
    Entity entity;
    // gets entity from front of queue
    entity = AvailableEntityIDs.front();
    // remove entity from queue so no one else tries to take it
    AvailableEntityIDs.pop();
    return entity;
}

void ECS::DeleteEntity(Entity entity) {
    // set all component flags to zero
    entity.ComponentBitMask.reset();
    // push entity back to end of queue
    AvailableEntityIDs.push(entity);
}

ECS::Entity ECS::AddComponent(
    Entity entity, uint8_t componentID) {
    // set the bit for the specified component ID
    entity.ComponentBitMask.set(componentID - 1, 1);
    return entity;
}

void ECS::RemoveComponent(Entity entity,
    uint8_t componentID) {
    // reset bit for specified component ID to zero
    entity.ComponentBitMask.reset(componentID);
}

bool ECS::EntityHasComponent(Entity entity,
    uint8_t componentID) {
    // test to see if the specified component bit is 1
    return entity.ComponentBitMask.test(componentID - 1);
}
*/