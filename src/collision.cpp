#include "../include/collision.hpp"
#include "../include/factory.hpp"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>

#define GRAVITY 9.17

void Collision::collisionLoop(entt::registry &reg, float dt, int bottomBorder) {
    auto physical = reg.view<Physics>();

    for (auto entity : physical) {
        gravityCollision(reg, dt, bottomBorder, entity);
        liquidCollision(reg, dt, bottomBorder, entity);
    }
}

bool Collision::registerEntity(entt::registry &reg, entt::entity entt) {
    auto enttR = reg.get<Renderable>(entt);

    if (reg.all_of<Border>(entt)) {
        auto enttB = reg.get<Border>(entt);
        if (enttB.position == "rightBorder") {
            for (int x = enttR.xPos; x < (enttR.xPos + 10); x++) {
                for (int y = enttR.yPos; y < (enttR.yPos + enttR.ySize); y++) {
                    grid[x][y] = entt;
                }
            }
        } else if (enttB.position == "leftBorder") {
            for (int x = enttR.xPos - 10; x < (enttR.xPos + enttR.xSize); x++) {
                for (int y = enttR.yPos; y < (enttR.yPos + enttR.ySize); y++) {
                    grid[x][y] = entt;
                }
            }
        }
    }

    for (int x = enttR.xPos - 1; x < (enttR.xPos + enttR.xSize) + 1; x++) {
        for (int y = enttR.yPos - 1; y < (enttR.yPos + enttR.ySize) + 1; y++) {
            if (reg.valid(grid[x][y])) return false;
        }
    }
    for (int x = enttR.xPos + 1; x < (enttR.xPos + enttR.xSize) - 1; x++) {
        for (int y = enttR.yPos + 1; y < (enttR.yPos + enttR.ySize) - 1; y++) {
            grid[x][y] = entt;
        }
    }
    return true;
}

void Collision::gravityCollision(entt::registry &reg, float dt, int bottomBorder,
    entt::entity entt) {

    auto enttR = reg.get<Renderable>(entt);

    float gravity = dt * reg.get<Physics>(entt).mass * GRAVITY;

    reg.patch<Renderable>(entt, [&reg, gravity](auto &renderable) {
        renderable.yPos += gravity;
    });

    auto newEnttR = reg.get<Renderable>(entt);
    int newY = 0;

    for (int x = newEnttR.xPos; x < newEnttR.xPos + newEnttR.xSize; x++) {
        for (int y = newEnttR.yPos; y < newEnttR.yPos + newEnttR.ySize; y++) {
            if (reg.valid(grid[x][y]) && (grid[x][y] != entt)) {
                newY = reg.get<Renderable>(grid[x][y]).yPos;
                if (newY == 43) newY = bottomBorder;
                reg.patch<Renderable>(entt, [newY](auto &renderable) {
                    renderable.yPos = newY - renderable.ySize;
                });
            }
        }
    }

    for (int x = enttR.xPos - 1; x < enttR.xPos + enttR.xSize + 1; x++) {
        for (int y = enttR.yPos - 1; y < enttR.yPos + enttR.ySize + 1; y++) {
            if (grid[x][y] == entt)
                grid[x][y] = entt::null;
        }
    }
    newEnttR = reg.get<Renderable>(entt);
    for (int x = newEnttR.xPos + 1; x < newEnttR.xPos + newEnttR.xSize - 1; x++) {
        for (int y = newEnttR.yPos + 1; y < newEnttR.yPos + newEnttR.ySize - 1; y++) {
            grid[x][y] = entt;
        }
    }
}

void Collision::liquidCollision(entt::registry &reg, float dt, int bottomBorder,
    entt::entity entt) {
    if (!reg.all_of<Liquid>(entt)) return;
    srand(time(0) * (uint)entt);

    auto enttR = reg.get<Renderable>(entt);
    bool direction = (rand() % 3 > 1) ? true : false;

    if (above(reg, entt) && !checkX(reg, entt, direction) && grounded(reg, entt, bottomBorder)) {
        moveX(reg, entt, dt, direction);
    } else if (checkX(reg, entt, true) && !checkX(reg, entt, false)) {
        moveX(reg, entt, dt, false);
    } else if(checkX(reg, entt, false) && !checkX(reg, entt, true)) {
        moveX(reg, entt, dt, true);
    }

}

bool Collision::above(entt::registry &reg, entt::entity entt) {
    auto enttR = reg.get<Renderable>(entt);
    int y = enttR.yPos - 1;
    for (int x = enttR.xPos; x < enttR.xPos + enttR.xSize; x++) {
        if (reg.valid(grid[x][y])) return true;
    }
    return false;
}

bool Collision::grounded(entt::registry &reg, entt::entity entt, int bottomBorder) {
    auto enttR = reg.get<Renderable>(entt);
    if ((int)enttR.yPos + enttR.ySize == bottomBorder) return true;
    int y = enttR.yPos + enttR.ySize + 1;
    for (int x = enttR.xPos; x < enttR.xPos + enttR.xSize; x++) {
        if (reg.valid(grid[x][y])) {
            return grounded(reg, grid[x][y], bottomBorder);
        }
    }
    return false;
}

bool Collision::checkX(entt::registry &reg, entt::entity entt, bool right) {
    auto enttR = reg.get<Renderable>(entt);
    int x = 0;
    if (right) x = enttR.xPos + enttR.xSize + 1;
    else x = enttR.xPos - 1;
    for (int y = enttR.yPos; y < (enttR.yPos + enttR.ySize); y++) {
        if (reg.valid(grid[x][y])) {
            return true;
        }
    }
    return false;
}

void Collision::moveX(entt::registry &reg, entt::entity entt, float dt, bool right) {
    auto enttR = reg.get<Renderable>(entt);
    reg.patch<Renderable>(entt, [dt, right](auto &renderable) {
        if (right)
            renderable.xPos += dt * 100;
        else
            renderable.xPos -= dt * 100;
    });
    auto newEnttR = reg.get<Renderable>(entt);
    
    for (int x = enttR.xPos - 2; x < (enttR.xPos + enttR.xSize) + 2; x++) {
        for (int y = enttR.yPos - 2; y < (enttR.yPos + enttR.ySize) + 2; y++) {
            if (entt == grid[x][y]) {
                grid[x][y] = entt::null;
            }
        }
    }
    for (int x = newEnttR.xPos + 1; x < (newEnttR.xPos + newEnttR.xSize) - 1; x++) {
        for (int y = newEnttR.yPos + 1; y < (newEnttR.yPos + newEnttR.ySize) - 1; y++) {
            grid[x][y] = entt;
        }
    }
}

void Collision::debugGrid(SpriteRenderer &spriteRenderer, entt::registry &reg) {
    for (int x = 33; x < 816; x++) {
        for (int y = 33; y < 398; y++) {
            if (reg.valid(grid[x][y])) {
                Texture2D texture = ResourceManager::GetTexture("button2");
                spriteRenderer.DrawSprite(texture, glm::vec2(x, y), glm::vec2(1.0f),
                0.0f, glm::vec4(1.0f, 0.0f, 0.0f, 0.4f));
            }
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