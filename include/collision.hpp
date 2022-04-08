#ifndef COLLISION_H
#define COLLISION_H

//#include "../include/simulationObject.hpp"
#include "../include/entt.hpp"
#include <vector>

class Collision{
public:
    //constructor
    Collision(){
        for (int i = 0; i < 764; i++) {
            for (int j = 0; j < 345; j++) {
                grid[i][j] = entt::null;
            }
        }
    };
    //Calculates whether a collision occured

    bool registerEntity(entt::registry &reg, entt::entity entity);
    
    void collisionLoop(entt::registry &reg, float dt, int bottomBorder);

    void triangleCollision(entt::registry *reg,float dt);

    bool detector(entt::entity &obj1, entt::entity &obj2, entt::registry &reg);
    bool checkCollision(entt::entity entity, entt::registry &reg);
    // void collide(SimulationObject &obj);

private:
    // 43, 806 x coords
    // 43, 387 y coords
    entt::entity grid[763][344];

    void gravityCollision(entt::registry &reg, float dt, int bottomBorder,
        entt::entity entity);

    void liquidCollision(entt::registry &reg, float dt, int bottomBorder,
        entt::entity entity);

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