// this code was written by Nate

#ifndef ENTITYCOMPONENTSYSTEM_H
#define ENTITYCOMPONENTSYSTEM_H

#include <list>
#include <queue>
#include <map>
#include <cstdint>
#include <bitset>
#include <type_traits>
#include <glm/glm.hpp>
#include "../include/texture.hpp"

#define ENTITYMAX 5000
#define COMPONENTMAX 2
#define DIMENSIONID 1
#define GRAVITYID 2

class ECS {
public: 

    typedef struct {
        std::uint32_t ID;
        std::bitset<COMPONENTMAX> ComponentBitMask;
    } Entity;

    struct Dimension {
        uint16_t xPos;
        uint16_t yPos;
        uint8_t xSize;
        uint8_t ySize;
        std::string texture;
    };

    struct Gravity {
        float gravity;
    };

    typedef struct {
        Dimension dimension;
        Gravity gravity;
    } Components;

    // list of usable entity IDs
    static std::queue<Entity> AvailableEntityIDs;

    static std::map<uint32_t, Components> EntityToComponents;

    // initializes entire EntityID array
    ECS();
    
    // creates a new entity, returns the ID
    Entity CreateEntity();

    // deletes an entity
    void DeleteEntity(Entity entity);

    // adds a component to an entity
    Entity AddComponent(Entity entity, uint8_t componentID);    

    // removes a component
    void RemoveComponent(Entity entity, uint8_t componentID);

    // checks if an entity has a component
    bool EntityHasComponent(Entity entity, uint8_t componentID);

};

#endif