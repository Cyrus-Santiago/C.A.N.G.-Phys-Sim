#include "../include/collision.hpp"
#include "../include/factory.hpp"
#include <GLFW/glfw3.h>

#define GRAVITY 9.17

void Collision::collisionLoop(entt::registry &reg, float dt, int bottomBorder) {
    auto physical = reg.view<Physics>();
    
    for (auto entity : physical) {
        gravityCollision(reg, dt, bottomBorder, entity);
        liquidCollision(reg, dt, bottomBorder, entity);
    }
}

bool Collision::registerEntity(entt::registry &reg, entt::entity entity) {
    int xPos = reg.get<Renderable>(entity).xPos;
    int xSize = reg.get<Renderable>(entity).xSize;
    int yPos = reg.get<Renderable>(entity).yPos;
    int ySize = reg.get<Renderable>(entity).ySize;

    for (int x = xPos; x < (xPos + xSize); x++) {
        for (int y = yPos; y < (yPos + ySize); y++) {
            if (reg.valid(grid[x][y])) return false;
        }
    }
    for (int x = xPos; x < (xPos + xSize); x++) {
        for (int y = yPos; y < (yPos + ySize); y++) {
            grid[x][y] = entity;
        }
    }
    return true;
}

void Collision::gravityCollision(entt::registry &reg, float dt, int bottomBorder,
    entt::entity entity) {
    int xPos = reg.get<Renderable>(entity).xPos;
    int xSize = reg.get<Renderable>(entity).xSize;
    int yPos = reg.get<Renderable>(entity).yPos;
    int ySize = reg.get<Renderable>(entity).ySize;
    float gravity = dt * reg.get<Physics>(entity).mass * GRAVITY;

    reg.patch<Renderable>(entity, [&reg, gravity](auto &renderable) {
        renderable.yPos += gravity;
    });

    int i = 0;
    int newYPos = ((int)reg.get<Renderable>(entity).yPos + ySize);
    bool result = false;
    for (i = xPos; i < xPos + xSize; i++) {
        if (reg.valid(grid[i][newYPos]) || ((yPos + ySize) >= bottomBorder)) {
            result = true;
            break;
        }
    }
    
    int ySolid = 0;

    if (result) {
        if ((yPos + ySize) >= bottomBorder) {
            ySolid = bottomBorder - ySize;
        } else {
            for (int j = yPos + ySize; j < bottomBorder; j++) {
                if (grid[i][j] != entt::null) {
                    ySolid = j - ySize;
                    break;
                }
            }
        }
        reg.patch<Renderable>(entity, [&reg, ySolid, gravity](auto &renderable)
        {
            if (ySolid != 0) {
                renderable.yPos = ySolid;
            } else {
                renderable.yPos -= gravity;
            }
        });
    }
    for (int x = xPos; x < (xPos + xSize); x++) {
        for (int y = yPos; y < (reg.get<Renderable>(entity).yPos); y++) {
            grid[x][y] = entt::null;
        }
    }
    yPos = reg.get<Renderable>(entity).yPos;
    for (int x = xPos; x < (xPos + xSize); x++) {
        for (int y = yPos; y < (yPos + ySize); y++) {
            grid[x][y] = entity;
        }
    }
}

void Collision::liquidCascade(entt::registry &reg, entt::entity entt, float dt, bool left) {
}

void Collision::liquidCollision(entt::registry &reg, float dt, int bottomBorder,
    entt::entity entt) {
    bool above = false;
    auto enttR = reg.get<Renderable>(entt);
    for (int i = enttR.xPos; i < enttR.xPos + enttR.xSize; i++) {
        if (reg.valid(grid[i][(int)enttR.yPos - 1])) {
            above = true;
        }
    }
    if (above) {
        moveX(reg, entt, dt, true);
    }
}

void Collision::moveX(entt::registry &reg, entt::entity entt, float dt, bool right) {
    auto enttR = reg.get<Renderable>(entt);
    for (int i = enttR.yPos; i < (enttR.yPos + enttR.ySize); i++) {
        if (reg.valid(grid[(int)enttR.xPos + enttR.xSize + 1][i])) return;
    }
    for (int x = enttR.xPos; x < (enttR.xPos + enttR.xSize); x++) {
        for (int y = enttR.yPos; y < (enttR.yPos + enttR.ySize); y++) {
            grid[x][y] = entt::null;
        }
    }
    reg.patch<Renderable>(entt, [dt, right](auto &renderable) {
        if (right)
            renderable.xPos += dt * 50;
        else
            renderable.xPos -= dt * 50;;
    });
    enttR = reg.get<Renderable>(entt);
    for (int x = enttR.xPos; x < (enttR.xPos + enttR.xSize); x++) {
        for (int y = enttR.yPos; y < (enttR.yPos + enttR.ySize); y++) {
            grid[x][y] = entt;
        }
    }
}

void Collision::triangleCollision(entt::registry *reg, float dt) {
    bool insideFlag=false;
    auto view = reg->view<Physics>();
    // loop through each entity in the view
    for (auto triangleEnt : view) {
        for(auto entity : view){
            insideFlag=false;
            //If the entities being compared are not the same and the second from physics is not a triangle
            if((entity != triangleEnt) && (!reg->all_of<Triangle>(entity) && (reg->all_of<Triangle>(triangleEnt)))){
                //I declared variables so it is more human readable. These are bounds for the physics entity
                float xSize =reg->get<Renderable>(entity).xSize;
                float ySize =reg->get<Renderable>(entity).ySize;
                glm::vec2 lowerBound(reg->get<Renderable>(entity).xPos,reg->get<Renderable>(entity).yPos);
                glm::vec2 upperBound(reg->get<Renderable>(entity).xPos+xSize,reg->get<Renderable>(entity).yPos+ySize);
                //Iterate through each point in the triangle entity to see if it is inside a renderable
                
                for(auto point : reg->get<Triangle>(triangleEnt).points){
                
                    //If the point on a triangle is within some renderable shape.
                    if( (point.x >= lowerBound.x) && (point.x <= upperBound.x) &&
                        (point.y >= lowerBound.y) && (point.y <= upperBound.y) )    {
                        insideFlag=true;
                        reg->patch<Renderable>(triangleEnt, [dt, triangleEnt, &reg] (auto &renderable){
                            renderable.yPos-=dt*reg->get<Physics>(triangleEnt).mass * GRAVITY;
                        });
                        reg->patch<Triangle>(triangleEnt, [dt, triangleEnt, &reg](auto &triangle){
                            float deltaY=dt * reg->get<Physics>(triangleEnt).mass * GRAVITY;
                            //functional operator "map" to update each point position
                            std::transform(triangle.points.begin(), triangle.points.end(), triangle.points.begin(),[deltaY](glm::vec2 point){
                                point.y+=deltaY;
                                return(point);
                            });
                        });        
                    std::cout<<"AAAAABBBBBBBB"<<std::endl;

                        break;
                    }
                }
                
                //Need to run more collision calculations using slopes :/
                if(!insideFlag){
                    //Variables so it's more human readable. Gets each triangle vertex and the slopes
                    glm::vec2 leftPoint = reg->get<Triangle>(triangleEnt).points[0];
                    glm::vec2 rightPoint = reg->get<Triangle>(triangleEnt).points[1];
                    glm::vec2 topPoint = reg->get<Triangle>(triangleEnt).points[2];
                    float leftSlope= (topPoint.y - leftPoint.y) / (topPoint.x - leftPoint.x);
                    float rightSlope= (rightPoint.y - topPoint.y) / (rightPoint.x - topPoint.x);
                    float pointY=topPoint.y;
                    //Calculate collisions along the right edge of the triangle
                    //TODO std::cout<<"x "<<lowerBound.x<<"-"<<upperBound.x<<" y "<<lowerBound.y<<"-"<<upperBound.y<<std::endl;
                    for(float pointX=topPoint.x; pointX <= rightPoint.x; pointX++){
                        //If the point on a triangle is within some renderable shape.
                        if( (pointX >= lowerBound.x) && (pointX <= upperBound.x) &&
                            (pointY >= lowerBound.y) && (pointY <= upperBound.y) )  {
                            reg->patch<Renderable>(triangleEnt, [dt, triangleEnt, &reg] (auto &renderable){
                                renderable.yPos-=dt*reg->get<Physics>(triangleEnt).mass * GRAVITY;
                            }); 
                            std::cout<<"AAAAA"<<std::endl;
                            break;
                        }
                        //else, calculate a new point on the edge of the triangle with the slope.
                        else    {
                            //TODO std::cout<<"pointX "<<pointX<<" pointY "<<pointY<<std::endl;
                            pointY+=rightSlope;
                        }
                    }
                    
                    //std::cout<<"left "<<leftSlope<<" right "<<rightSlope<<std::endl;
                    //std::exit(0);
                }
                //TODO If the triangle entity is a forcewave. Actually maybe just make separate forcewave detection method
                if(reg->all_of<Triangle>(triangleEnt)){

                }
            }
            //TODO else, if the second entity IS a triangle (ugh)
        }
    }
}

// bool Collision::detector(entt::entity &obj1, entt::entity &obj2, entt::registry &reg){
//     colX = reg.get<Renderable>(obj1).xPos + reg.get<Renderable>(obj1).xSize >= 
//         reg.get<Renderable>(obj2).xPos && reg.get<Renderable>(obj2).xPos + 
//         reg.get<Renderable>(obj2).xSize >= reg.get<Renderable>(obj1).xPos;
    
//     colY = reg.get<Renderable>(obj1).yPos + reg.get<Renderable>(obj1).ySize >= 
//         reg.get<Renderable>(obj2).yPos && reg.get<Renderable>(obj2).yPos + 
//         reg.get<Renderable>(obj2).ySize >= reg.get<Renderable>(obj1).yPos;

//     return colX && colY;
// }

// bool Collision::checkCollision(entt::entity entity, entt::registry &reg){
//     auto view = reg.view<Renderable>();
//     for(auto Entity : view){
//         if(detector(entity, Entity, reg)){
//             return true;
//         }
//     }

//     return false;
// }
//Depcrecated(?), uses an old simulation object
/*
void Collision::collide(SimulationObject &obj){
    obj.Velocity = glm::vec2 (0.0f,0.0f);
}
*/