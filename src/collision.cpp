#include "../include/collision.hpp"
#include "../include/factory.hpp"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include "../include/flame.hpp"

#define GRAVITY 9.17

Flame flame;

/* Arguments: entity registry, delta frame time, position of the bottom border.
 * Returns:   N/A
 * Purpose:   Loops through every entity and calls different collisons functions.
 *            It's more efficient than having individual loops for every type of
 *            collision. */
void Collision::collisionLoop(entt::registry &reg, float dt, int bottomBorder, int topBorder) {
    auto renderable = reg.view<Renderable>();

    // loops through each entity with the physics component
    for (auto entity : renderable) {
        if (reg.any_of<Physics>(entity)) {
            // enacts gravity and bottom border collision
            gravityCollision(reg, dt, bottomBorder, entity);
            // simulates liquid movement and collision
            liquidCollision(reg, dt, bottomBorder, entity);
        }

        if (reg.any_of<Gas>(entity)) {
            gasCollision(reg, dt, topBorder, entity);
        }

        if (reg.any_of<Fire>(entity)) {
            flame.burn(reg, entity, dt, * this);
        }
    }
}

/* Arguments: entity registry, an entity to register
 * Returns:   true if successful, false if it's overlapping an existing entity.
 * Purpose:   registers an entity in the collision grid, ensures that it doesn't
 *            overlap an existing entity. The grid prevents us from checking
 *            every existing entity on the screen for a collision. */
bool Collision::registerEntity(entt::registry &reg, entt::entity entt) {
    auto enttR = reg.get<Renderable>(entt);

    // checks if entity has the border component
    if (reg.all_of<Border>(entt)) {
        // gets the border component fields of the entity
        auto enttB = reg.get<Border>(entt);
        // if we have the right border we register a 10 pixel buffer to the right
        // of it to help with collision detection
        if (enttB.position == "rightBorder") {
            for (int x = enttR.xPos; x < (enttR.xPos + 10); x++) {
                for (int y = enttR.yPos; y < (enttR.yPos + enttR.ySize); y++) {
                    // store the border entity ID in the grid locations
                    this->grid[x][y] = entt;
                }
            }
        // if we have the left border we register the same buffer except to the left
        } else if (enttB.position == "leftBorder") {
            for (int x = enttR.xPos - 10; x < (enttR.xPos + enttR.xSize); x++) {
                for (int y = enttR.yPos; y < (enttR.yPos + enttR.ySize); y++) {
                    // store the border entity ID in the grid locations
                    this->grid[x][y] = entt;
                }
            }
        }
        // success
        return true;
    }

    // checks if an entity exists where we're trying to place one
    for (int x = enttR.xPos - 1; x < (enttR.xPos + enttR.xSize) + 1; x++) {
        for (int y = enttR.yPos - 1; y < (enttR.yPos + enttR.ySize) + 1; y++) {
            // if there is an entity there, we return false and the entity
            // should be deleted and not drawn
            if (reg.valid(this->grid[x][y])) return false;
        }
    }
    // since we're not on top of another entity, we register the location of the
    // new entity, the grid location is 1 pixel smaller all around to make things
    // more flush visually.
    for (int x = enttR.xPos + 1; x < (enttR.xPos + enttR.xSize) - 1; x++) {
        for (int y = enttR.yPos + 1; y < (enttR.yPos + enttR.ySize) - 1; y++) {
            // set all the grid locations for the entity to the entity ID
            this->grid[x][y] = entt;
        }
    }
    // success
    return true;
}

/* Arguments: entity registry, delta frame time, position of bottom border, entity
 * Returns:   N/A
 * Purpose:   Enacts gravity on a given entity, and ensure it stops accelerating
 *            down once it makes contact with the ground or another entity. */
void Collision::gravityCollision(entt::registry &reg, float dt, int bottomBorder,
    entt::entity entt) {

    // get the renderable component of the entity
    auto enttR = reg.get<Renderable>(entt);
    // calculate gravity based on the delta frame, the mass of the entity, and
    // the gravitational constant
    float gravity = dt * reg.get<Physics>(entt).mass * GRAVITY;

    // change the position of the entity by the calculated gravity
    reg.patch<Renderable>(entt, [&reg, gravity](auto &renderable) {
        renderable.yPos += gravity;
    });

    // the following code checks if we need to undo the above y position change
    // to account for a collision.

    // get the renderable component of the entt now that the y position is changed
    // we call it newEnttR because it's the renderable data of the entity's new pos
    auto newEnttR = reg.get<Renderable>(entt);
    int newY = 0;

    // we loop through the entire area of the new entity to check for an overlap
    // with anything else
    for (int x = newEnttR.xPos; x < newEnttR.xPos + newEnttR.xSize; x++) {
        for (int y = newEnttR.yPos; y < newEnttR.yPos + newEnttR.ySize; y++) {

            // if we detect a valid entity that is not the entity in question
            if (reg.valid(this->grid[x][y]) && (this->grid[x][y] != entt) &&
                ((reg.any_of<Physics>(this->grid[x][y])) ||
                reg.any_of<Border>(this->grid[x][y]))) {

                // we adjust the y component to the top edge of whatever it collided
                // with
                newY = reg.get<Renderable>(this->grid[x][y]).yPos;

                // for some reason, it wanted to put the entity on the top border
                // when it collided with the bottom border, this fixes that.
                if (newY == 43) newY = bottomBorder;

                // adjust the y component of our entity so that the bottom is
                // flush with what it collided into, we subtract the size to get
                // the new top position, since that's what yPos represents.
                reg.patch<Renderable>(entt, [newY](auto &renderable) {
                    renderable.yPos = newY - renderable.ySize + 1;
                });
            }
        }
    }

    // now we erase all the grid data based on the original renderable component
    // of the entity (before any movement) with a buffer to ensure we get it all
    for (int x = enttR.xPos - 1; x < enttR.xPos + enttR.xSize + 1; x++) {
        for (int y = enttR.yPos - 1; y < enttR.yPos + enttR.ySize + 1; y++) {
            // we ensure we're only erasing *this* component
            if (this->grid[x][y] == entt)
                this->grid[x][y] = entt::null;
        }
    }

    // now we get the renderable data for the new entity again, in case we made any
    // adjustments since the original fetch
    newEnttR = reg.get<Renderable>(entt);

    // now we claim everything one pixel withdrawn from what's rendered
    for (int x = newEnttR.xPos + 1; x < newEnttR.xPos + newEnttR.xSize - 1; x++) {
        for (int y = newEnttR.yPos + 1; y < newEnttR.yPos + newEnttR.ySize - 1; y++) {
            this->grid[x][y] = entt;
        }
    }
}

/* Arguments: entity registry, delta frame time, pos of bottom border, entity
 * Returns:   N/A
 * Purpose:   This implements an algorithm that tries to mimic the way water will
 *            fill the container it occupies. */
void Collision::liquidCollision(entt::registry &reg, float dt, int bottomBorder,
    entt::entity entt) {

    // checks to see if the entity has the liquid component, return if not
    if (!reg.all_of<Liquid>(entt)) return;
    // seed the random number generator based on time and entity ID
    srand(time(0) * (uint)entt);

    // get the renderable component of our entity
    auto enttR = reg.get<Renderable>(entt);
    // choose a direction to move randomly even is right, odd is left
    int direction = rand() % 19;

    // if there's nothing in the direction we chose, we move that way
    if (!checkX(reg, entt, direction)) {
        moveX(reg, entt, dt, direction);
    }
    while (above(reg, entt)) {
        moveUp(reg, entt, dt);
    }

}

void Collision::gasCollision(entt::registry &reg, float dt, int topBorder,
    entt::entity entt) {

    auto enttR = reg.get<Renderable>(entt);
    // choose a direction to move randomly even is right, odd is left
    int direction = ((uint)entt) % 3;
    direction = (direction % 2 == 0) ? direction : direction * -1;

    reg.patch<Renderable>(entt, [dt, direction, entt](auto &renderable) {
        renderable.xPos += dt * direction;
        renderable.yPos -= dt * 30;
    });

    enttR = reg.get<Renderable>(entt);
    
    if (enttR.yPos <= topBorder + 10) {
        reg.erase<Gas>(entt);
        reg.emplace<Liquid>(entt);
        reg.emplace<Physics>(entt, 10.0f);
        reg.replace<Renderable>(entt, "particle", "solid", enttR.xPos, enttR.yPos,
            5, 5, 0.0f, 0.2f, 0.2f, 1.0f, 1.0f);
        registerEntity(reg, entt);
    }
}

/* Arguments: entity registry, entity
 * Returns:   true if there's something above us, false otherwise
 * Purpose:   helps urge movement of liquid particles if there's something above. */
bool Collision::above(entt::registry &reg, entt::entity entt) {

    // get renderable component
    auto enttR = reg.get<Renderable>(entt);

    // get y position directly above this entity
    int y = enttR.yPos - 1;

    // check the pixel directly above along the width of the entity
    for (int x = enttR.xPos; x < enttR.xPos + enttR.xSize; x++) {

        // if there's a valid non-liquid entity, there's something above us
        if (reg.valid(this->grid[x][y]) &&
            !reg.all_of<Liquid>(this->grid[x][y]) &&
            reg.all_of<Physics>(this->grid[x][y])) { return true; }
    }

    // there's nothing above
    return false;
}

/* Arguments: entity registry, entity, bottom border position
 * Returns:   true if we're on the ground or there's something between us and the ground.
 * Purpose:   this helps ensure water particles don't fly around in the air as they fall. */
bool Collision::grounded(entt::registry &reg, entt::entity entt, int bottomBorder) {

    // get the renderable component of the entity
    auto enttR = reg.get<Renderable>(entt);

    // we we're on the ground directly, return true
    if ((int)enttR.yPos + enttR.ySize - 1 >= bottomBorder) return true;

    // get the y pos directly beneath this entity
    int y = enttR.yPos + enttR.ySize + 1;

    // scan along width of the entity, one pixel below
    for (int x = enttR.xPos; x < enttR.xPos + enttR.xSize; x++) {

        // if we find another entity, we recurse to see if it's on the ground.
        if (reg.valid(this->grid[x][y])) {
            // if it is, or it's touching another entity that's on the ground,
            // eventually this will return true
            return grounded(reg, this->grid[x][y], bottomBorder);
        }
    }
    // we're not grounded
    return false;
}

/* Arguments: entity registry, entity, right boolean (true is right, false is left)
 * Returns:   true if there's something there, false if there isn't
 * Purpose:   checks to see if there's an entity to the left or right of a given
 *            entity for safe movment. */
bool Collision::checkX(entt::registry &reg, entt::entity entt, int direction) {
    
    // get renderable component of entity
    auto enttR = reg.get<Renderable>(entt);
    // if we're moving right we set the x component to the pixel just right of 
    // the entity, otherwise we set x to the pixel just left
    int x = 0;
    if (direction % 2 == 0) x = enttR.xPos + enttR.xSize + 1;
    else x = enttR.xPos - 1;

    // now we scan along the length of the entity at that x position
    for (int y = enttR.yPos; y < (enttR.yPos + enttR.ySize); y++) {
        // if we find something, we return true
        if (reg.valid(this->grid[x][y])) {
            return true;
        }
    }
    // there's nothing there
    return false;
}

/* Arguments: entity registry, entity, delta frame time, right bool (true is right, false is left)
 * Returns:   N/A
 * Purpose:   Facilitates movement of entity in a given x direction */
void Collision::moveX(entt::registry &reg, entt::entity entt, float dt, int direction) {

    // get renderable component of entity
    auto enttR = reg.get<Renderable>(entt);

    // change the x position of the entity based on the delta frame, and direction
    // we were told to move
    reg.patch<Renderable>(entt, [dt, direction](auto &renderable) {
        if (direction % 2 == 0)
            renderable.xPos += dt * 3 * direction;
        else
            renderable.xPos += dt * 3 * direction * -1;
    });

    // get renderable component of future entity
    auto newEnttR = reg.get<Renderable>(entt);
    
    // erase past entity from the collision grid (using a 2 pixel buffer to ensure
    // we get it all)
    for (int x = enttR.xPos - 2; x < (enttR.xPos + enttR.xSize) + 2; x++) {
        for (int y = enttR.yPos - 2; y < (enttR.yPos + enttR.ySize) + 2; y++) {
            // ensure we only delete *this* entity
            if (entt == this->grid[x][y]) {
                this->grid[x][y] = entt::null;
            }
        }
    }
    
    // register entity in new x position, with a 1 pixel buffer for better rendering
    for (int x = newEnttR.xPos + 1; x < (newEnttR.xPos + newEnttR.xSize) - 1; x++) {
        for (int y = newEnttR.yPos + 1; y < (newEnttR.yPos + newEnttR.ySize) - 1; y++) {
            this->grid[x][y] = entt;
        }
    }
}

void Collision::moveUp(entt::registry &reg, entt::entity entt, float dt) {
    
    // get renderable component of entity
    auto enttR = reg.get<Renderable>(entt);
    
    // erase past entity from the collision grid (using a 2 pixel buffer to ensure
    // we get it all)
    for (int x = enttR.xPos - 2; x < (enttR.xPos + enttR.xSize) + 2; x++) {
        for (int y = enttR.yPos - 2; y < (enttR.yPos + enttR.ySize) + 2; y++) {
            // ensure we only delete *this* entity
            if (entt == this->grid[x][y]) {
                this->grid[x][y] = entt::null;
            }
        }
    }

    reg.patch<Renderable>(entt, [dt](auto &renderable) {
            renderable.yPos -= dt * 30;
    });

    // get renderable component of future entity
    auto newEnttR = reg.get<Renderable>(entt);

    // we loop through the entire area of the new entity to check for an overlap
    // with anything else
    for (int x = newEnttR.xPos; x < newEnttR.xPos + newEnttR.xSize; x++) {
        for (int y = newEnttR.yPos; y < newEnttR.yPos + newEnttR.ySize; y++) {
            // if we detect a valid entity
            if (reg.valid(this->grid[x][y]) && (this->grid[x][y] != entt)) return;
        }
    }

    // register entity in new x position, with a 1 pixel buffer for better rendering
    for (int x = newEnttR.xPos + 1; x < (newEnttR.xPos + newEnttR.xSize) - 1; x++) {
        for (int y = newEnttR.yPos + 1; y < (newEnttR.yPos + newEnttR.ySize) - 1; y++) {
            this->grid[x][y] = entt;
        }
    }
}

/* Arguments: spriteRenderer, entity registry
 * Returns:   N/A
 * Purpose:   draws a translucent red filter over areas registered in the
 *            collision grid. */
void Collision::debugGrid(SpriteRenderer &spriteRenderer, entt::registry &reg) {

    // sorry about the hardcoded values, loops around outside and inside of play
    // area, so you can clearly see borders and entities
    for (int x = 33; x < 816; x++) {
        for (int y = 33; y < 398; y++) {
            // if it's valid, we draw a filter over it.
            if (reg.valid(this->grid[x][y])) {
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

entt::entity Collision::entityAtLoc(int x, int y) {
    return this->grid[x][y];
}

void Collision::destroyEnttAtLoc(entt::registry &reg, int x, int y) {
    auto entt = this->grid[x][y];
    if (!reg.valid(entt)) return;
    auto enttR = reg.get<Renderable>(entt);
    
    // now we erase all the grid data based on the original renderable component
    // of the entity (before any movement) with a buffer to ensure we get it all
    for (int x = enttR.xPos - 1; x < enttR.xPos + enttR.xSize + 1; x++) {
        for (int y = enttR.yPos - 1; y < enttR.yPos + enttR.ySize + 1; y++) {
            // we ensure we're only erasing *this* component
            if (this->grid[x][y] == entt)
                this->grid[x][y] = entt::null;
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