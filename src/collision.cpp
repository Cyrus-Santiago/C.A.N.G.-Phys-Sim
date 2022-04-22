#include "../include/collision.hpp"
#include "../include/factory.hpp"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include "../include/flame.hpp"

#define GRAVITY     9.17


Flame flame;

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

        if (reg.any_of<Gas>(entity)) {
            gasCollision(reg, dt, topBorder, entity);
        }
        if (reg.any_of<Fire>(entity)) {
            flame.burn(reg, entity, dt, * this);
        }
    }
    //Separate view to loop through explosions because entities might
    //be deleted, causing in a valid entity assertion fail for other methods
    auto explosions = reg.view<Forcewave>();
    for(auto enttF : explosions){
        forcewaveCollision(reg,enttF,dt);
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

    if(reg.all_of<Triangle>(entt))
        return(triangleGravityCollision(reg, dt, bottomBorder,entt));

    // get the renderable component of the entity
    auto enttR = reg.get<Renderable>(entt);

    // calculate gravity based on the delta frame, the mass of the entity, and
    // the gravitational constant
    float gravity = reg.get<Physics>(entt).mass * GRAVITY;

    if (entityExists(reg, entt, enttR, DOWN) == entt::null)
        moveY(reg, entt, dt, 2, gravity);
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
        moveX(reg, entt, dt, direction, 1);
        
    // while (above(reg, entt))
    //     moveY(reg, entt, dt, 1,30.0f);

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
void Collision::moveX(entt::registry &reg, entt::entity entt, float dt, int direction, float magnitude) {
    // get renderable component of entity
    auto enttR = reg.get<Renderable>(entt);

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
    
    entityUnclaim(reg, entt, enttR);
    entityClaim(reg, entt, reg.get<Renderable>(entt));
}

/* Arguments: entity registry, entity, delta frame time, direction int (even is down, odd is up)
 *            magnitude float number
 * Returns:   N/A
 * Purpose:   Facilitates movement of entity in a given y direction */
void Collision::moveY(entt::registry &reg, entt::entity entt, float dt, int direction, float magnitude) {
    
    //This integer determines which direction to move. Up is positive, down is negative
    int upOrDown = 1;
    if (direction % 2 == 0) upOrDown = -1;

    // get renderable component of entity
    auto enttR = reg.get<Renderable>(entt);

    reg.patch<Renderable>(entt, [upOrDown,magnitude,dt](auto &renderable) {
            renderable.yPos -= dt * magnitude * upOrDown;
    });

    // get renderable component of future entity
    auto newEnttR = reg.get<Renderable>(entt);

    // we loop through the entire area of the new entity to check for an overlap
    // with anything else
    int x, y;
    for (x = newEnttR.xPos; x < newEnttR.xPos + newEnttR.xSize; x++) {
        for (y = newEnttR.yPos; y < newEnttR.yPos + newEnttR.ySize; y++) {
            // if we detect a valid entity
            if (reg.valid(this->grid[x][y]) && (this->grid[x][y] != entt)) break;
        }
        if (reg.valid(this->grid[x][y]) && (this->grid[x][y] != entt)) {
            reg.patch<Renderable>(entt, [y](auto &renderable) {
                renderable.yPos = y - renderable.ySize;
            });
            break;
        }
    }

    newEnttR = reg.get<Renderable>(entt);

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

    // register entity in new y position, with a 1 pixel buffer for better rendering
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
    auto enttT= reg.get<Triangle>(entt);
    //Variables to make this more readable
    glm::vec2 leftPoint=enttT.points[0]; glm::vec2 rightPoint=enttT.points[1];
    glm::vec2 topPoint=enttT.points[2];
    int xLeft=(int)leftPoint.x, xRight=(int)rightPoint.x;
    int previousXLeft, previousXRight;
    //Descending for loop to check if an entity exists where we're trying to place one
    for(int y=(int)(leftPoint.y); y >(int)topPoint.y ; y-=2){        
        previousXLeft=xLeft;
        previousXRight=xRight;
        //Checks from left to right on a certain y-line between two triangle points to check if an entity is there
        for(xLeft; xLeft <= xRight; xLeft++)    {
            // if there is an entity there, we return false and the entity
            // should be deleted and not drawn
            if(reg.valid(this->grid[xLeft][(y)]) ||reg.valid(this->grid[xLeft][(y-1)]) )   return false;
        }
        //Update left and right points. They should come closer to each other
        xLeft=previousXLeft +1;
        xRight=previousXRight -1;
    }
    //If we leave this loop, that means that there is no entity in the landing zone, meaning
    //we are free to make a new triangle entity
    //Reset points
    xLeft=(int)leftPoint.x; 
    xRight=(int)rightPoint.x;
    //Descending for loop to register from the bottom-up of a triangle. Same routine as before
    for(int y=(int)(leftPoint.y); y >(int)topPoint.y ; y-=2){
        previousXLeft=xLeft;
        previousXRight=xRight;
        for(xLeft; xLeft <= xRight; xLeft++)    {
            // set all the grid locations for the entity to the entity ID
            this->grid[xLeft][(y)]=entt;
            //I don't get why I can't decrement y by 1 in the loop conditional. If I do that, only 
            //half the triangle gets registered. So I have to do this statement as well.
            this->grid[xLeft][(y-1)]=entt;
        }
        //Update left and right points. They should come closer to each other
        xLeft=previousXLeft + 1;
        xRight=previousXRight - 1;
    }
    //success
    return true;
}
/* 
*Arguments: entity registry, delta time frame, bottom border y-position, triangle entity
*Returns:   N/A
*Purpose:   Performs gravity calculations on a triangle shape. The grid entities 
*           are updated accordingly. This function is buggy */
void Collision::triangleGravityCollision(entt::registry &reg, float dt, int bottomBorder, entt::entity entt){
    //Distance from top to bottom of the triangle
    float ySize=reg.get<Renderable>(entt).ySize;
    //If the triangle's position is at the bottom of the play area, adjust accordingly and return immediately
    if(reg.get<Renderable>(entt).yPos+reg.get<Renderable>(entt).ySize+1>bottomBorder) {
        reg.patch<Renderable>(entt,[bottomBorder,ySize](auto &renderable){
            renderable.yPos=bottomBorder-ySize+1;
        });
        return;
    }
    //This is where the fun begins
    bool breakFlag=false;
    bool slopeFlag=false;
    //entity that will be deleted
    auto enttTOld= reg.get<Triangle>(entt);
    int newY = 0;
    float gravity = dt * reg.get<Physics>(entt).mass * GRAVITY;
    //Update entity's y position and triangle point y positions based on the gravity
    reg.patch<Renderable>(entt, [gravity](auto &triangle){
        triangle.yPos+=gravity;
    });
    //functional operator "map" to update each point y-position for each triangle point
    reg.patch<Triangle>(entt, [gravity](auto &triangle){
        std::transform(triangle.points.begin(), triangle.points.end(), triangle.points.begin(),[gravity](glm::vec2 point){
            point.y+=gravity;
            return(point);
        });
    });
    auto enttTNew = reg.get<Triangle>(entt);
    glm::vec2 leftPoint=enttTNew.points[0]; glm::vec2 rightPoint=enttTNew.points[1];
    glm::vec2 topPoint=enttTNew.points[2];
    int xLeft=(int)leftPoint.x, xRight=(int)rightPoint.x;
    int previousXLeft=0, previousXRight=0;
    //Descending for loop to check if an entity exists where we're trying to place one
    for(int y=(int)(leftPoint.y); y >(int)topPoint.y ; y--){   
        if(breakFlag)   break;
        previousXLeft=xLeft;
        previousXRight=xRight;
        //Checks from left to right on a certain y-line between two triangle points to check if an entity is there
        for(xLeft; xLeft <= xRight; xLeft++)    {
            //Check if there is a valid entity and if the entity is not the same one.
            if(reg.valid(this->grid[xLeft][(y)]) && this->grid[xLeft][(y)]!= entt ){
                newY = reg.get<Renderable>(this->grid[xLeft][y]).yPos;
                // for some reason, it wanted to put the entity on the top border
                // when it collided with the bottom border, this fixes that.
                if (newY <= 43 || newY >= 348) newY = bottomBorder;
                //Update y-position of triangle
                reg.patch<Renderable>(entt, [bottomBorder,newY](auto &renderable) {
                    renderable.yPos = newY - renderable.ySize + 1;
                    if(renderable.yPos>=bottomBorder)   renderable.yPos=bottomBorder;
                });
                reg.patch<Triangle>(entt, [ySize,newY](auto &triangle){
                    triangle.points[0].y=newY-ySize+1;
                    triangle.points[1].y=newY-ySize+1;
                    triangle.points[2].y=newY+1;
                });
                //Only perform the operation once per detection
                breakFlag=true;
                break;
            }
            
        }
        //Update left and right points. They should come closer to each other every other iteration
        if(slopeFlag){
            xLeft=previousXLeft +1;
            xRight=previousXRight -1;
            slopeFlag=false;
        }
        else{
            slopeFlag=true;
        }
    }
    //Erase all points on grid of original triangle spot
    //Reset points
    glm::vec2 leftPointOld=enttTOld.points[0]; glm::vec2 rightPointOld=enttTOld.points[1];
    glm::vec2 topPointOld=enttTOld.points[2];
    xLeft=(int)leftPointOld.x; 
    xRight=(int)rightPointOld.x;
    for(int y=(int)(leftPointOld.y); y >(int)topPointOld.y ; y-=2){        
        previousXLeft=xLeft;
        previousXRight=xRight;
        //Removes the triangle pixel-by-pixel on a certain y-line
        for(xLeft; xLeft <= xRight; xLeft++)    {
            if(this->grid[xLeft][y] == entt)    this->grid[xLeft][y]=entt::null;
            if(this->grid[xLeft][(y-1)] == entt)    this->grid[xLeft][(y-1)]=entt::null;
        }
        //Update left and right points. They should come closer to each other
        xLeft=previousXLeft +1;
        xRight=previousXRight -1;
    }
    //Claim points on grid for new triangle position
    enttTNew=reg.get<Triangle>(entt);
    xLeft=(int)leftPoint.x; 
    xRight=(int)rightPoint.x;
    for(int y=(int)(leftPoint.y); y >(int)topPoint.y ; y-=2){        
        previousXLeft=xLeft;
        previousXRight=xRight;
        for(xLeft; xLeft <= xRight; xLeft++)    {
            this->grid[xLeft][y]=entt;
            this->grid[xLeft][(y-1)]=entt;
        }
        //Update left and right points. They should come closer to each other
        xLeft=previousXLeft +1;
        xRight=previousXRight -1;
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
                moveY(reg, gridEntt, dt, 11, enttF.yVel);
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

/* Depcrecated method for detecting triangle collisions
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
                            pointY+=rightSlope;
                        }
                    }
                }
                if(reg->all_of<Triangle>(triangleEnt)){
                }
            }
        }
    }
}

Deprecated gravity collision method
void Collision::gravityCollision(entt::registry &reg, float dt, int bottomBorder,
    entt::entity entt) {
    if(reg.all_of<Triangle>(entt))  {
        return(triangleGravityCollision(reg, dt, bottomBorder,entt));
    }
    // get the renderable component of the entity
    auto enttR = reg.get<Renderable>(entt);
    // calculate gravity based on the delta frame, the mass of the entity, and
    // the gravitational constant
    float gravity = dt * reg.get<Physics>(entt).mass * GRAVITY;

    // change the position of the entity by the calculated gravity
    reg.patch<Renderable>(entt, [&reg, gravity](auto &renderable) {
        if (gravity < 2)
            renderable.yPos += gravity;
        else
            renderable.yPos += 2;
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
                //If falling onto the side of a triangle, don't get the yPos from renderable.
                //Instead, get the yPos from the nearest surface layer
                if(reg.get<Renderable>(this->grid[x][y]).type=="TRIANGLE"){
                    int y1=y;
                    while( this->grid[x][y1] != entt::null){
                        newY=y1;
                        y1--;
                    }
                }
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
    //Move objects if there is something in the way. The main purpose is to 
    //Move when falling onto a triangle.
    if(!checkX(reg,entt,1) && checkX(reg,entt,2))   moveX(reg,entt,dt,1,5.0f);
    else if(!checkX(reg,entt,2) && checkX(reg,entt,1)) moveX(reg,entt,dt,2,5.0f);
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

*/
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