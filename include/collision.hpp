#ifndef COLLISION_H
#define COLLISION_H

#include "../include/simulationObject.hpp"
#include "../include/background.hpp"

class Collision{
public:
    //constructor
    Collision(){};
    //Calculates whether a collision occured
    bool detector(SimulationObject &obj1, SimulationObject &obj2);
    bool borderDet(SimulationObject &obj, Background &border);

    void collide(SimulationObject &obj);

private:
    bool colX, colY;
};

#endif 