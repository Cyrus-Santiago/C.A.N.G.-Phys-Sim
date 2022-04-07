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
    void gravityCollision(entt::registry &reg, float dt, int bottomBorder);
    void liquidCollision(entt::registry &reg, float dt);
    void triangleCollision(entt::registry *reg,float dt);

    // void collide(SimulationObject &obj);

private:
    bool colX, colY;

    // if A right overlaps B
    bool rightOverlap(entt::registry &reg, entt::entity A, entt::entity B);
    // if A left overlaps B
    bool leftOverlap(entt::registry &reg, entt::entity A, entt::entity B);
    // if A top overlaps B
    bool topOverlap(entt::registry &reg, entt::entity A, entt::entity B);
    // if A bottom overlaps B
    bool bottomOverlap(entt::registry &reg, entt::entity A, entt::entity B);

    // if A left overlaps OR right overlaps B
    bool xOverlap(entt::registry &reg, entt::entity A, entt::entity B);
    // if A top overlaps OR bottom overlaps B
    bool yOverlap(entt::registry &reg, entt::entity A, entt::entity B);
};

#endif 