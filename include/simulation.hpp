// This code was written by Nate

#ifndef SIMULATION_H
#define SIMULATION_H

#include "../include/simulationObject.hpp"
#include <vector>

class Simulation {
private:
    std::vector<SimulationObject> SimulationObjects;
public:
    Simulation() { }

    void Draw(SpriteRenderer &renderer);

    SimulationObject Create(glm::vec2 position, glm::vec3 color = glm::vec3(1.0f),
                glm::vec2 size = glm::vec2(5),
                Texture2D texture = ResourceManager::GetTexture("button1"),
                glm::vec2 velocity = glm::vec2(0.0f));
            
    void Destroy(SimulationObject simObj);

    void init();

};

#endif