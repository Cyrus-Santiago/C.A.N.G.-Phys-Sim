// This code was written by Nate

#include "../include/simulation.hpp"
#include <iostream>

Collision col;

void Simulation::Draw(SpriteRenderer &spriteRenderer) {
    // loops through each element in Sim Obj array and draws it
    // if it isn't destroyed
    for (int i = 0; i < SimulationObjects.size(); ++i) {
        if (!SimulationObjects[i].Destroyed) {
            SimulationObjects[i].Draw(spriteRenderer);
        }
    }
}


SimulationObject Simulation::Create(glm::vec2 position, glm::vec3 color,
                        glm::vec2 size, Texture2D texture,
                        glm::vec2 velocity) {
    // flag used to let us know if we found a spot with a destroyed object we
    // can use for our new object
    bool failure = true;
    // construct our new object
    SimulationObject simulationObject(position, color, size,
                                          texture, velocity);
    // loops through each simulation object                    
    for (SimulationObject &simObj : this->SimulationObjects) {
        // if we find a destroyed one, our new object takes it's place, we
        // set out flag to say we found one, and we break
        if (simObj.Destroyed) {
            failure = false;
            simObj = simulationObject;
            break;
        }
    }
    // if we didn't find a spot for our new object, we make a new spot in the array
    if (failure)
        SimulationObjects.push_back(simulationObject);

    // debug statement to make sure the simulation isn't growing
    // the array when there's destroyed objects
    std::cout << SimulationObjects.size() << std::endl;

    // here we return our newly minted object
    return simulationObject;
}

void Simulation::Destroy(SimulationObject simulationObject) {
    // here we loop through our array and look for the object we're destroying
    for (SimulationObject &simObj : this->SimulationObjects) {
        if (simulationObject.Position == simObj.Position) {
            // it's marked as destroyed now, and a new object will take it's
            // place
            simObj.Destroyed = true;
        }
    }
}

void Simulation::Update(float dt) {
    // we loop through our object array and call move on each one
    // and provide it with the delta frame so movement is consistent
    // on any machine
    for (SimulationObject &simObj : this->SimulationObjects) {
        if (!simObj.Destroyed) {
            simObj.Move(dt);
            //if(col.borderDet(simObj,)){ I'm going to put border collision detection somewhere else - Cyrus
            //    col.collide(simObj);
            //}
        }
    }
}

void Simulation::getBorder(std::vector<Background> border){
    Border = border;
}