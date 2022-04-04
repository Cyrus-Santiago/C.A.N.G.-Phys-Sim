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
                    if ((yBoundsB.x < yBoundsA.y) && (yBoundsA.y < yBoundsB.y)) {
                        // we essentially undo the gravitational effect before it's
                        // even drawn.
                        reg.patch<Renderable>(entityA, [dt, entityA, &reg, yBoundsB, entityB]
                            (auto &renderable) {
                            renderable.yPos = yBoundsB.x - reg.get<Renderable>(entityA).ySize;
                        });
                    }
                }
                /* 
                * if entity A's left side is to the left of entity B's left side,
                * AND entity A's right side is to the right of entity B's left side,
                * OR
                * entity A's left side is to the left of entity B's left side,
                * AND entity A's right side is to the right of entity B's right side,
                */
                if (((xBoundsA.x <= xBoundsB.x) && (xBoundsB.x <= xBoundsA.y)) ||
                    (((xBoundsA.x <= xBoundsB.y) && (xBoundsB.y <= xBoundsA.y)))) {
                    /*
                    * if entity A's top is above entity B's bottom,
                    * AND entity A's bottom is below entity B's bottom
                    */
                    if ((yBoundsA.x < yBoundsB.y) && (yBoundsB.y < yBoundsA.y)) {
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
