#ifndef COLLISION_H
#define COLLISION_H

//#include "../include/simulationObject.hpp"
#include "../include/entt.hpp"
#include <vector>

class Collision{
public:
    //constructor
    Collision(){};
    //Calculates whether a collision occured
    bool detector(entt::entity &obj1, entt::entity &obj2, entt::registry &reg);
    bool checkCollision(entt::entity entity, entt::registry &reg);

//    void collide(SimulationObject &obj);

private:
    bool colX, colY;
};

#endif 