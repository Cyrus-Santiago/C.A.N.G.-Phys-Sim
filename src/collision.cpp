#include "../include/collision.hpp"
#include "../include/factory.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/fwd.hpp>
#include "../include/fire.hpp"
#include "../include/lava.hpp"
#include "../include/ice.hpp"

#define GRAVITY     9.17

/* Arguments: entity registry, delta frame time, position of the bottom border.
 * Returns:   N/A
 * Purpose:   Loops through every entity and calls different collisons functions.
 *            It's more efficient than having individual loops for every type of
 *            collision. */
void Collision::collisionLoop(entt::registry &reg, float dt, int bottomBorder, int topBorder) {

    // auto renderable = reg.group<Renderable>(entt::get<Renderable>);

    auto renderable = reg.view<Renderable>();

    // loops through each entity with the physics component
    for (auto entity : renderable) {
        if (reg.any_of<Physics>(entity)) {
            // enacts gravity and bottom border collision
            gravityCollision(reg, dt, bottomBorder, entity);
            // simulates liquid movement and collision
            liquidCollision(reg, dt, bottomBorder, entity);
        }

        if (reg.any_of<Ice>(entity))
            iceWaterCollision(reg, entity, dt, * this);

        if (reg.any_of<Lava>(entity)) {
            lavaWaterCollision(reg, entity, dt, * this);
            lavaStoneCollision(reg, entity, dt, * this);
            lavaIceCollision(reg, entity, dt, * this);
        }

        if (reg.any_of<Gas>(entity))
            gasCollision(reg, dt, topBorder, entity);

        if (reg.any_of<Fire>(entity))
            if (burn(reg, entity, dt, * this)) continue;
        //Need the valid since the entity might be deleted
        if(reg.valid(entity)){
            if (reg.any_of<Forcewave>(entity))  forcewaveCollision(reg,entity,dt);
        }
        if(reg.any_of<Light>(entity)){
            rayCollision(reg, entity);
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
        } else if (enttB.position == "bottomBorder") {
            for (int x = enttR.xPos - 10; x < (enttR.xPos + enttR.xSize + 10); x++) {
                for (int y = enttR.yPos; y < (enttR.yPos + 11); y++) {
                    // store the border entity ID in the grid locations
                    this->grid[x][y] = entt;
                }
            }
        } else if (enttB.position == "topBorder") {
            for (int x = enttR.xPos - 10; x < (enttR.xPos + enttR.xSize + 10); x++) {
                for (int y = enttR.yPos - 10; y < (enttR.yPos); y++) {
                    // store the border entity ID in the grid locations
                    this->grid[x][y] = entt;
                }
            }
        }
        // success
        return true;
    }
    //Checks if entity has the triangle component. Does NOT include explosion entities
    if(reg.all_of<Triangle>(entt) && !reg.all_of<Forcewave>(entt)){
        return(registerTriangleEntity(reg,entt));
    }

    // checks if an entity exists where we're trying to place one
    if (entityExists(reg, entt, enttR, IN_PLACE) != entt::null) return false;
    
    entityClaim(reg, entt, enttR);
    return true;
}

entt::entity Collision::entityExists(entt::registry &reg, entt::entity entt,
                            Renderable &enttR, int where, bool excludeEntt) {
    int x = 0, y = 0;
    switch (where) {
        case IN_PLACE:
            break;
        case UP:
            y = enttR.yPos - 1;
            break;
        case DOWN:
            y = enttR.yPos + enttR.ySize + 1;
            break;
        case LEFT:
            x = enttR.xPos - 1;
            break;
        case RIGHT:
            x = enttR.xPos + enttR.xSize + 1;
            break;
        }
    if (where == LEFT || where == RIGHT) {
        // now we scan along the length of the entity at that x position
        for (int y = enttR.yPos; y < (enttR.yPos + enttR.ySize); y++) {
            // if we find something, we return true
            if (reg.valid(this->grid[x][y])) {
                return this->grid[x][y];
            }
        }
        // there's nothing there
        return entt::null;
    } else if (where == UP || where == DOWN) {
        // now we scan along the length of the entity at that x position
        for (int x = enttR.xPos; x < (enttR.xPos + enttR.xSize); x++) {
            // if we find something, we return true
            if (reg.valid(this->grid[x][y])) {
                return this->grid[x][y];
            }
        }
        // there's nothing there
        return entt::null;
    } else {
        // checks if an entity exists where we're trying to place one
        for (int x = enttR.xPos - 1; x < (enttR.xPos + enttR.xSize) + 1; x++) {
            for (int y = enttR.yPos - 1; y < (enttR.yPos + enttR.ySize) + 1; y++) {

            // if there is an entity there, we return false and the entity
            // should be deleted and not drawn
            if (excludeEntt) {
                if (reg.valid(this->grid[x][y]))
                    return this->grid[x][y];
            } else {
                if ((reg.valid(this->grid[x][y])) && (this->grid[x][y] != entt))
                    return this->grid[x][y];
                }
            }
        }
        return entt::null;
    }
}

void Collision::entityClaim(entt::registry &reg, entt::entity entt,
    Renderable & enttR) {

    // the grid location is 1 pixel smaller all around to make things more flush
    // visually.
    for (int x = enttR.xPos + 1; x < (enttR.xPos + enttR.xSize) - 1; x++) {
        for (int y = enttR.yPos + 1; y < (enttR.yPos + enttR.ySize) - 1; y++) {

            // set all the grid locations for the entity to the entity ID
            this->grid[x][y] = entt;
        }
    }
}

void Collision::entityUnclaim(entt::registry &reg, entt::entity entt, Renderable & enttR) {

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
    float gravity = reg.get<Physics>(entt).mass * GRAVITY;

    auto enttBelow = entityExists(reg, entt, enttR, DOWN);
    if (reg.valid(enttBelow)) {
        if (reg.any_of<Border>(enttBelow)) {
            moveY(reg, entt, dt, 2, gravity);
        }
    } else {
        moveY(reg, entt, dt, 2, gravity);
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
    if (entityExists(reg, entt, enttR, (direction % 2 == 0) ? RIGHT : LEFT) == entt::null)
        moveX(reg, entt, dt, direction, 1, true);
    
    // auto enttAbove = entityExists(reg, entt, enttR, UP);
    // while (reg.valid(enttAbove)) {
    //     if (reg.any_of<Liquid, Gas>(enttAbove)) break;
    //     moveY(reg, entt, dt, 1, 1, true);
    //     enttAbove = entityExists(reg, entt, enttR, UP);
    // }
}

void Collision::gasCollision(entt::registry &reg, float dt, int topBorder,
    entt::entity entt) {

    auto enttR = reg.get<Renderable>(entt);
    enttR.yPos = enttR.yPos - enttR.ySize;
    auto enttAbove = entityExists(reg, entt, enttR, UP);
    
    if (!reg.valid(enttAbove)) {
        auto enttR = reg.get<Renderable>(entt);
        // choose a direction to move randomly even is right, odd is left
        int direction = ((uint)entt) % 3;
        direction = (direction % 2 == 0) ? direction : direction * -1;

        reg.patch<Renderable>(entt, [dt, direction, entt](auto &renderable) {
            renderable.xPos += dt * direction;
            renderable.yPos -= dt * 30;
        });

    } else if (reg.any_of<Border>(enttAbove)) {
        srand(time(0) * (uint) entt);
        if (rand() % 4 == 0) {
            reg.erase<Gas>(entt);
            reg.emplace<Liquid>(entt);
            reg.emplace<Water>(entt);
            reg.emplace<Physics>(entt, 10.0f);
            reg.replace<Renderable>(entt, "particle", "solid", enttR.xPos, enttR.yPos,
                5, 5, 0.0f, 0.2f, 0.2f, 1.0f, 1.0f);
            registerEntity(reg, entt);
        }
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
    int y = enttR.yPos + enttR.ySize;

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

/* Arguments: entity registry, entity, delta frame time, direction int (even is right, odd is left)
 *            magnitude float number
 * Returns:   N/A
 * Purpose:   Facilitates movement of entity in a given x direction */
void Collision::moveX(entt::registry &reg, entt::entity entt, float dt, int direction, float magnitude, bool skip) {
    // get renderable component of entity
    auto enttR = reg.get<Renderable>(entt);
    if(!skip){
        auto gridEntt=entityExists(reg,entt,enttR,direction);
        if(reg.valid(gridEntt)){
            if(reg.all_of<Border>(gridEntt)){
                if(direction % 2 ==0)    return(moveX(reg,entt,dt,3,magnitude*2,true));
                else    return(moveX(reg,entt,dt,4,magnitude,true));
            }
        }
    }
    if (reg.any_of<Liquid>(entt)) {
        auto enttL = reg.get<Liquid>(entt);
        magnitude /= enttL.viscosity;
    }

    // change the x position of the entity based on the delta frame, and direction
    // we were told to move
    reg.patch<Renderable>(entt, [magnitude,dt, direction](auto &renderable) {
        if (direction % 2 == 0)
            renderable.xPos += dt * magnitude * direction;
        else
            renderable.xPos += dt * magnitude * direction * -1;
    });
    auto newEnttR = reg.get<Renderable>(entt);
    entityUnclaim(reg, entt, enttR);
    // register entity if it's a triangle
    if(reg.all_of<Triangle>(entt))  triangleEntityClaim(reg,entt,newEnttR);
    else    entityClaim(reg, entt, newEnttR);
}

/* Arguments: entity registry, entity, delta frame time, direction int (even is down, odd is up)
 *            magnitude float number
 * Returns:   N/A
 * Purpose:   Facilitates movement of entity in a given y direction */
void Collision::moveY(entt::registry &reg, entt::entity entt, float dt, int direction, float magnitude, bool ignoreCol) {
    // get renderable component of entity
    auto enttR = reg.get<Renderable>(entt);
    //This integer determines which direction to move. Up is positive, down is negative
    int upOrDown = 1;
    if (direction % 2 == 0) upOrDown = -1;

    reg.patch<Renderable>(entt, [upOrDown,magnitude,dt](auto &renderable) {
            renderable.yPos -= dt * magnitude * upOrDown;
    });

    if (!ignoreCol) {
        // get renderable component of future entity
        auto newEnttR = reg.get<Renderable>(entt);

    // we loop through the entire area of the new entity to check for an overlap
    // with anything else
    int x, y;
    for (x = newEnttR.xPos; x < newEnttR.xPos + newEnttR.xSize; x++) {
        for (y = newEnttR.yPos; y < newEnttR.yPos + newEnttR.ySize; y++) {
            // if we detect a valid entity
            if (reg.valid(this->grid[x][y]) && (this->grid[x][y] != entt))
                break;
        }
        if (reg.valid(this->grid[x][y]) && (this->grid[x][y] != entt)) {
            if (reg.all_of<Border>(this->grid[x][y])) {
                if (reg.get<Border>(this->grid[x][y]).position == "topBorder")
                    break;
            }
            reg.patch<Renderable>(entt, [y](auto &renderable) {
                renderable.yPos = y - renderable.ySize;
            });
            break;
        }
    }

    newEnttR = reg.get<Renderable>(entt);

    // erase past entity from the collision grid (using a 2 pixel buffer to ensure
    // we get it all)
    entityUnclaim(reg,entt,enttR);
    // register entity if it's a triangle
    if(reg.all_of<Triangle>(entt))  triangleEntityClaim(reg,entt,newEnttR);
    // register entity in new y position, with a 1 pixel buffer for better rendering
    else    entityClaim(reg,entt,newEnttR);
    }
}

/* Arguments: spriteRenderer, entity registry
 * Returns:   N/A
 * Purpose:   draws a translucent red filter over areas registered in the
 *            collision grid. */
void Collision::debugGrid(SpriteRenderer &spriteRenderer, entt::registry &reg) {

    //This only shows the spots of entities with their associated shape. It won't
    //show every single point on the play area where an "entity" might be (in the case of bugs)
    /*
    auto view = reg.view<Renderable>();
    for(auto ent : view){
        auto enttR=reg.get<Renderable>(ent);
        for(int x=enttR.xPos; x<(enttR.xPos +enttR.xSize); x++){
            for(int y=enttR.yPos; y<(enttR.yPos +enttR.ySize); y++){
                if(reg.valid(this->grid[x][y])){
                    Texture2D texture = ResourceManager::GetTexture("button2");
                    spriteRenderer.DrawSprite(texture, glm::vec2(x, y), glm::vec2(1.0f),
                    0.0f, glm::vec4(1.0f, 0.0f, 0.0f, 0.4f));
                }
            }
        }
    }
    */
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

/* 
 * Arguments: entity registry, a triangle entity to register
 * Returns:   true if successful, false if it's overlapping an existing entity.
 * Purpose:   registers a triangle entity in the collision grid, ensures that it doesn't
 *            overlap an existing entity. The grid prevents us from checking
 *            every existing entity on the screen for a collision. */
bool Collision::registerTriangleEntity(entt::registry &reg, entt::entity entt){
    //Variables to make this more readable
    auto enttR=reg.get<Renderable>(entt);
    if (entityExists(reg,entt,enttR,IN_PLACE) != entt::null)    return false;
    //If we leave this if statement, that means that there is no entity in the landing zone, meaning
    //we are free to make a new triangle entity
    triangleEntityClaim(reg,entt,enttR);
    //success
    return true;
}
/*
* Argumnents: entity registry, a triangle entity, and its renderable compeonent
* Returns: N/A
* Purpose: Claims an entity on the collision grid in the shape of a triangle.*/
void Collision::triangleEntityClaim(entt::registry &reg, entt::entity entt, 
    Renderable &newEnttR){
    int previousXLeft=0,    previousXRight=0;
    int xLeft=newEnttR.xPos,    xRight=newEnttR.xPos+newEnttR.xSize;
    //Descending for loop to register from the bottom-up of a triangle. 
    for(int y=(int)(newEnttR.yPos+newEnttR.ySize); y > (int)newEnttR.yPos; y-=2){
        previousXLeft=xLeft;
        previousXRight=xRight;
        for(xLeft;xLeft <= xRight; xLeft++){
            // set all the grid locations for the entity to the entity ID
            this->grid[xLeft][y]=entt;
            //I don't get why I can't decrement y by 1 in the loop conditional. If I do that, only 
            //half the triangle gets registered. So I have to do this statement as well.
            this->grid[xLeft][(y-1)]=entt;
        }
        //Update left and right points. They should come closer to each other
        xLeft=previousXLeft + 1;
        xRight=previousXRight - 1;
    }
}
/* 
*Arguments: entity registry, delta time frame, bottom border y-position, triangle entity
*Returns:   N/A
*Purpose:   Performs gravity calculations on a triangle shape. The grid entities 
*           are updated accordingly.*/
void Collision::triangleGravityCollision(entt::registry &reg, float dt, int bottomBorder, entt::entity entt){
    //Old copy of the triangle renderable
    auto enttR = reg.get<Renderable>(entt);
    float gravity = reg.get<Physics>(entt).mass * GRAVITY;
    //If there is no entity beneath the triangle
    if (entityExists(reg,entt,enttR,DOWN,false) == entt::null){
        entityUnclaim(reg,entt,enttR);
        moveY(reg, entt, dt, 2, gravity);
    }
}

entt::entity Collision::entityAtLoc(int x, int y) {
    return this->grid[x][y];
}

void Collision::destroyEnttAtLoc(entt::registry &reg, int x, int y) {
    auto entt = this->grid[x][y];
    if (!reg.valid(entt)) return;
    auto enttR = reg.get<Renderable>(entt);
    
    entityUnclaim(reg, entt, enttR);
}

/*
*Arguments: entity registry, forcewave entity, delta frame time
*Returns:   N/A
*Purpose:   Iterates through each point on a forcewave and determines if any
*           point intersects with a grid entity. The velocity of the forcewave
*           is added to the entity. If the forcewave touches a border, the
*           forcewave gets deleted. */
void Collision::forcewaveCollision(entt::registry &reg, entt::entity entt,float dt){
    //Get forcewave components of entity
    auto enttF=reg.get<Forcewave>(entt);
    entt::entity gridEntt;
    //Iterate through each triangle point in a forcewave
    for(auto point: reg.get<Triangle>(entt).points){
        gridEntt=entityAtLoc((int)point.x,(int)point.y);
        //If there is a grid entity at some triangle point
        if(reg.valid(gridEntt)){
            //If the grid entity is a border, delete the forcewave
            if(reg.all_of<Border>(gridEntt))  {
                reg.destroy(entt);
                return;
            }
            // auto enttR=reg.get<Renderable>(gridEntt);
            //If the grid entity has physics, push it in some direction
            int rotation=reg.get<Renderable>(entt).rotate;
            //If the explosion force wave goes to the right, push right
            if(rotation == 45 || rotation == 90 || rotation == 135){
                moveX(reg, gridEntt, dt, 1, enttF.xVel * (-1));
            }
            //If the explosion force wave goes to the left, push left
            if(rotation == 225 || rotation == 270 || rotation == 315){
                moveX(reg, gridEntt, dt, 2, enttF.xVel);
            }
            //If the explosion force wave goes up, push up
            if(rotation == 0 || rotation == 45 || rotation == 315){
                moveY(reg, gridEntt, dt, 1, enttF.yVel*(-15));
            }
            //If the explosion force wave goes down, push down
            if(rotation == 180 || rotation == 135 || rotation == 225){
                moveY(reg, gridEntt, dt, 2, enttF.yVel);
            }
            //Only perform the move for one point.
            return;
        }
    }
}
/* Arguments: 
 * Returns:   boolean
 * Purpose:   Checks if ray collides with an object.
              Depending on material of object, ray will reflect
              or path of light will halt. */
bool Collision::rayCollision(entt::registry &reg, entt::entity entt){
    auto ray = reg.get<Renderable>(entt);
    auto object = entityExists(reg, entt, ray, DOWN, true);
    if (reg.valid(object)){
        if(reg.any_of<Shape>(object)){
            //std::cout << "shape detected" << std::endl;
            return true;
        }
        else if(reg.any_of<Reflective>(object)){
            //std::cout << "glass detected" << std::endl;
            return true;
        }
        else if(reg.any_of<Liquid>(object)){
            //std::cout << "water detected" << std::endl;
            return true;
        }
    }
    /* check if ray entity meets solid entity */
    /* if yes, then shorten ray dimensions to stop at collision */
    /* check if ray entity meets reflective entity */
    /* if yes, then create incident ray */
    /* check if ray entity meets liquid entity */
    /* if yes, then create refractive ray */
    return false;
}
