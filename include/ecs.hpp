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
#define COMPONENTMAX 3
#define DIMENSIONID 1
#define GRAVITYID 2
#define GROWID 3

class ECS {
public: 

    typedef struct {
        std::uint32_t ID;
        std::bitset<COMPONENTMAX> ComponentBitMask;
    } Entity;

    typedef struct {
        float xPos;
        float yPos;
        float xSize;
        float ySize;
        std::string texture;
    } Dimension;

    typedef struct {
        float mass;
    } Physics;

    typedef struct {
        Dimension dimension;
        Physics physics;
    } Components;

    // list of usable entity IDs
    std::queue<Entity> AvailableEntityIDs;

    std::map<uint32_t, Components> EntityToComponents;

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