#ifndef COLLISION_H
#define COLLISION_H

//#include "../include/simulationObject.hpp"
#include "../include/entt.hpp"
#include <vector>
#include <glm/fwd.hpp>

class Collision{
public:
    //constructor
    Collision(){
        for (int i = 0; i < 781; i++) {
            for (int j = 0; j < 358; j++) {
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
    entt::entity grid[781][358];

    void gravityCollision(entt::registry &reg, float dt, int bottomBorder,
        entt::entity entity);

    void liquidCollision(entt::registry &reg, float dt, int bottomBorder,
        entt::entity entity);

    void liquidCascade(entt::registry &reg, entt::entity entt, float dt, bool left);

    void moveX(entt::registry &reg, entt::entity entt, float dt, bool right);

    // if A left overlaps OR right overlaps B
    bool xOverlap(entt::registry &reg, entt::entity A, entt::entity B);
    // if A top overlaps OR bottom overlaps B
    bool yOverlap(entt::registry &reg, entt::entity A, entt::entity B);
};

#endif 