// This code was written by Nate

#ifndef SIMULATION_H
#define SIMULATION_H

#include "../include/simulationObject.hpp"
#include <vector>

// this class keeps track of all the objects in the game. It draws them, destroys them,
// creates them, and updates them.
class Simulation {
private:
    // array of every object in the simulation
    std::vector<SimulationObject> SimulationObjects;
public:
    // constructor
    Simulation() { }

    // destuctor
    ~Simulation();

    // used to draw each individual simulation object
    void Draw(SpriteRenderer &renderer);

    // used to create a simulation object and add it to the array
    SimulationObject Create(glm::vec2 position, glm::vec4 color = glm::vec4(1.0f),
        glm::vec2 size = glm::vec2(5),
        Texture2D texture = ResourceManager::GetTexture("button2"),
        glm::vec2 velocity = glm::vec2(0.0f));
            
    SimulationObject Create(SimulationObject simObj);
    // used to mark a simulation object as destroyed, so it's storage space can be
    // reused by a new object
    void Destroy(SimulationObject simObj);

    // used to give time to simulation objects so they can move
    void Update(float dt);
};

#endif