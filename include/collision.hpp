#ifndef COLLISION_H
#define COLLISION_H

//#include "../include/simulationObject.hpp"
#include "../include/entt.hpp"
#include <vector>
#include "../include/spriteRenderer.hpp"
#include "../include/factory.hpp"

class Collision {
public:
    // constructor
    Collision(entt::registry &reg){
        for (int i = 0; i < 806; i++) {
            for (int j = 0; j < 388; j++) {
                grid[i][j] = entt::null;
            }
        }
    };

    // Calculates whether a collision occured

    bool registerEntity(entt::registry &reg, entt::entity entity);
    
    void collisionLoop(entt::registry &reg, float dt, int bottomBorder, int topBorder);

    void debugGrid(SpriteRenderer &spriteRenderer, entt::registry &reg);

    entt::entity entityAtLoc(int x, int y);
    void destroyEnttAtLoc(entt::registry &reg, int x, int y);

//    void triangleCollision(entt::registry *reg,float dt);

    bool detector(entt::entity &obj1, entt::entity &obj2, entt::registry &reg);
    bool checkCollision(entt::entity entity, entt::registry &reg);
    // void collide(SimulationObject &obj);

private:
    // 43, 806 x coords
    // 43, 387 y coords
    entt::entity grid[820][400];
    bool registerTriangleEntity(entt::registry &reg, entt::entity entt);

    void triangleGravityCollision(entt::registry &reg, float dt, int bottomBorder, entt::entity entt);

    void forcewaveCollision(entt::registry &reg, entt::entity entt,float dt);

    void gravityCollision(entt::registry &reg, float dt, int bottomBorder,
        entt::entity entity);

    void liquidCollision(entt::registry &reg, float dt, int bottomBorder,
        entt::entity entity);

    void gasCollision(entt::registry &reg, float dt, int topBorder,
        entt::entity entity);

    void moveX(entt::registry &reg, entt::entity entt, float dt, int direction, float magnitude);

    void moveY(entt::registry &reg, entt::entity entt, float dt, int direction, float magnitude);

    bool checkX(entt::registry &reg, entt::entity entt, int direction);

    bool grounded(entt::registry &reg, entt::entity entt, int bottomBorder);

    bool above(entt::registry &reg, entt::entity entt);
};

#endif 