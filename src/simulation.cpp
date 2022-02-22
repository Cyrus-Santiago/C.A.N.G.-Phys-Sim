// This code was written by Nate

#include "../include/simulation.hpp"
#include <iostream>

void Simulation::Draw(SpriteRenderer &spriteRenderer) {
    for (int i = 0; i < SimulationObjects.size(); ++i) {
        if (!SimulationObjects[i].Destroyed) {
            SimulationObjects[i].Draw(spriteRenderer);
        }
    }
}

SimulationObject Simulation::Create(glm::vec2 position, glm::vec3 color,
                        glm::vec2 size, Texture2D texture,
                        glm::vec2 velocity) {
    bool failure = true;
    SimulationObject simulationObject(position, color, size,
                                          texture, velocity);
    for (SimulationObject &simObj : this->SimulationObjects) {
        if (simObj.Destroyed) {
            failure = false;
            simObj = simulationObject;
            break;
        }
    }
    if (failure)
        SimulationObjects.push_back(simulationObject);

    // debug statement to make sure the simulation isn't growing
    // the array when there's destroyed objects
    std::cout << SimulationObjects.size() << std::endl;

    return simulationObject;
}

void Simulation::Destroy(SimulationObject simulationObject) {
    for (SimulationObject &simObj : this->SimulationObjects) {
        if (simulationObject.Position == simObj.Position) {
            simObj.Destroyed = true;
        }
    }
}
