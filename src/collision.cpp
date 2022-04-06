#include "../include/collision.hpp"
#include "../include/factory.hpp"

#define GRAVITY 9.17

void Collision::gravityCollision(entt::registry &reg, float dt,
    int bottomBorder) {
    // create a view containing all the entities with the physics component
    auto view = reg.view<Physics>();

    glm::vec2 xBoundsA, yBoundsA, xBoundsB, yBoundsB;

    // loop through each entity in the view
    for (auto entityA : view) {

        // left and right dimensions of entity A (save them for collisions later on)
        xBoundsA = glm::vec2(reg.get<Renderable>(entityA).xPos,
                            reg.get<Renderable>(entityA).xPos +
                            (float)reg.get<Renderable>(entityA).xSize);
        // top and bottom dimensions of entity A (save them for collisions later on)
        yBoundsA = glm::vec2(reg.get<Renderable>(entityA).yPos,
                            reg.get<Renderable>(entityA).yPos +
                            (float)reg.get<Renderable>(entityA).ySize);

        // loop through each entity with the physics component that's above the
        // bottom border
        if (reg.get<Renderable>(entityA).yPos + reg.get<Renderable>(entityA).ySize
            <= bottomBorder) {    
            // calculate distance moved by gravity for the object and add it to
            // the entity's y component to simulate gravity
            reg.patch<Renderable>(entityA, [dt, entityA, &reg](auto &renderable)
            {
                renderable.yPos += dt * reg.get<Physics>(entityA).mass * GRAVITY;
            });
        }

        // now we loop through every entity again, so we can compare entities
        for (auto entityB : view) {
            ///There's a separate function for dealing with triangle collisions
            //because they're a pain to calculate unlike rectangle shapes.
            //So we skip any triangle entities (including forcewaves)
            if((!reg.all_of<Triangle>(entityA)) && (!reg.all_of<Triangle>(entityB))){
                // we check to make sure we don't compare an entity with itself,
                // since it'd always be colliding with itself
                if (entityA != entityB) {
                    // left and right dimensions of entity B
                    // (save them for collisions later on)
                    xBoundsB = glm::vec2(reg.get<Renderable>(entityB).xPos,
                                        reg.get<Renderable>(entityB).xPos +
                                        (float)reg.get<Renderable>(entityB).xSize);
                    // top and bottom dimensions of entity B
                    // (save them for collisions later on)
                    yBoundsB = glm::vec2(reg.get<Renderable>(entityB).yPos,
                                        reg.get<Renderable>(entityB).yPos +
                                        (float)reg.get<Renderable>(entityB).ySize);

                    /* This next part is really dense so I'm going to be very verbose */

                    /* 
                    * if entity B's left side is to the left of entity A's left side,
                    * AND entity B's right side is to the right of entity A's left side,
                    * 
                    * OR entity A's left side is to the left of entity B's right size,
                    * AND entity A's right side is to the right of entity B's right side,
                    */ 
                    if ((xBoundsB.x <= xBoundsA.x) && (xBoundsA.x <= xBoundsB.y) ||
                        (((xBoundsA.x <= xBoundsB.y) && (xBoundsB.y <= xBoundsA.y)))) {
                        /*
                        * if entity B's top is above entity A's bottom,
                        * AND entity B's bottom is below entity A's bottom
                        */
                        if ((yBoundsB.x <= yBoundsA.y) && (yBoundsA.y <= yBoundsB.y)) {
                            // we essentially undo the gravitational effect before it's
                            // even drawn.
                            reg.patch<Renderable>(entityA, [dt, entityA, &reg, yBoundsB, entityB]
                                (auto &renderable) {
                                renderable.yPos = yBoundsB.x - reg.get<Renderable>(entityA).ySize;
                            });
                        }
                        /* 
                        * if entity A's left side is to the left of entity B's left side,
                        * AND entity A's right side is to the right of entity B's left side,
                        * OR
                        * entity A's left side is to the left of entity B's left side,
                        * AND entity A's right side is to the right of entity B's right side,
                        */
                        if ((yBoundsA.x <= yBoundsB.y) && (yBoundsB.y <= yBoundsA.y)) {
                            // we essentially undo the gravitational effect before it's
                            // even drawn.
                            reg.patch<Renderable>(entityB, [dt, entityB, &reg, yBoundsA, entityA]
                                (auto &renderable) {
                                renderable.yPos = yBoundsA.x - reg.get<Renderable>(entityB).ySize;
                            });
                        }
                    }
                }
            }
        }
    }
}

void Collision::liquidCollision(entt::registry &reg, float dt) {
    auto view = reg.view<Liquid>();
    srand(time(0));
    for (auto entity : view) {
        reg.patch<Renderable>(entity, [dt, &reg, entity](auto &renderable) {
            renderable.xPos += (((rand() % 100 - 50) % 4) * dt) /
            reg.get<Liquid>(entity).viscosity;
        });
    }
    for (auto entityA : view) {
        glm::vec2 xBoundsA, xBoundsB;

        // left and right dimensions of entity A (save them for collisions later on)
        xBoundsA = glm::vec2(reg.get<Renderable>(entityA).xPos,
                            reg.get<Renderable>(entityA).xPos +
                            (float)reg.get<Renderable>(entityA).xSize);

        // now we loop through every entity again, so we can compare entities
        for (auto entityB : view) {
            // left and right dimensions of entity B (save them for collisions later on)
            xBoundsB = glm::vec2(reg.get<Renderable>(entityB).xPos,
                            reg.get<Renderable>(entityA).xPos +
                            (float)reg.get<Renderable>(entityB).xSize);

            // we check to make sure we don't compare an entity with itself,
            // since it'd always be colliding with itself
            if (entityA != entityB) {
                if (xBoundsB.x == xBoundsA.y) {
                    reg.patch<Renderable>(entityA, [&reg, entityA, xBoundsB]
                            (auto &renderable) {
                            renderable.xPos = xBoundsB.x + renderable.xSize;
                        });
                } else if (xBoundsA.x == xBoundsB.y) {
                    reg.patch<Renderable>(entityB, [&reg, entityB, xBoundsA]
                        (auto &renderable) {
                        renderable.xPos = xBoundsA.x + renderable.xSize;
                    });
                }
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
            //If the entities being compared are not the same and the second from rendreable is not a triangle
            if((entity != triangleEnt) && (!reg->all_of<Triangle>(entity))){
                std::cout<<"he"<<std::endl;
                //I declared variables so it is more human readable. These are bounds for the renderable entity
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
                    std::cout<<"lower "<<lowerBound.x<<" "<<lowerBound.y<<" upperBound "<<upperBound.x<<" "<<upperBound.y<<std::endl;
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
                            std::cout<<"pointX "<<pointX<<" pointY "<<pointY<<std::endl;
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

bool Collision::detector(entt::entity &obj1, entt::entity &obj2, entt::registry &reg){
    colX = reg.get<Renderable>(obj1).xPos + reg.get<Renderable>(obj1).xSize >= 
        reg.get<Renderable>(obj2).xPos && reg.get<Renderable>(obj2).xPos + 
        reg.get<Renderable>(obj2).xSize >= reg.get<Renderable>(obj1).xPos;
    
    colY = reg.get<Renderable>(obj1).yPos + reg.get<Renderable>(obj1).ySize >= 
        reg.get<Renderable>(obj2).yPos && reg.get<Renderable>(obj2).yPos + 
        reg.get<Renderable>(obj2).ySize >= reg.get<Renderable>(obj1).yPos;

    return colX && colY;
}

bool Collision::checkCollision(entt::entity entity, entt::registry &reg){
    auto view = reg.view<Renderable>();
    for(auto Entity : view){
        if(detector(entity, Entity, reg)){
            return true;
        }
    }

    return false;
}
//Depcrecated(?), uses an old simulation object
/*
void Collision::collide(SimulationObject &obj){
    obj.Velocity = glm::vec2 (0.0f,0.0f);
}
*/