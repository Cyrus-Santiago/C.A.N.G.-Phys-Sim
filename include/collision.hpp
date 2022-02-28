#ifndef COLLISION_H
#define COLLISION_H

#include "../include/simulationObject.hpp"
#include <vector>

class Collision{
public:
    //constructor
    Collision(){};
    //Calculates whether a collision occured
    bool detector(SimulationObject &obj1, SimulationObject &obj2);
    bool checkCollision(SimulationObject &obj, std::vector<SimulationObject> Border);

    void collide(SimulationObject &obj);

private:
    bool colX, colY;
};

#endif 