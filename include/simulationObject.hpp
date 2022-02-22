// This code was written by Nate

#ifndef SIMULATIONOBJECT_H
#define SIMULATIONOBJECT_H

#include "../include/spriteRenderer.hpp"
#include "../include/resourceManager.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

// this is the base class of every entity to be drawn on the screen. It contains
// information that every object in the game should have ie position, velocity, etc
class SimulationObject {
public:
    glm::vec2 Position, Size, Velocity;
    glm::vec3 Color;
    bool Destroyed;
    Texture2D Texture;

    // constructor
    SimulationObject(glm::vec2 position, glm::vec3 color = glm::vec3(1.0f),
                     glm::vec2 size = glm::vec2(5),
                     Texture2D texture = ResourceManager::GetTexture("button2"),
                     glm::vec2 velocity = glm::vec2(0.0f), bool Destroyed = false);
    
    // used so the simulation object can experience gravity
    void Move(float dt);

    // calls sprite renderer on simulation object
    virtual void Draw(SpriteRenderer &spriteRenderer);
};

#endif