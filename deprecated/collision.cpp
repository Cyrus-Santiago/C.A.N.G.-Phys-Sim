// DEPRECATED liquid collision code, it works but it's ideal
/*void Collision::liquidCollision(entt::registry &reg, float dt, int bottomBorder,
    entt::entity entity) {
    if (reg.any_of<Liquid>(entity)) {
        srand((unsigned int)entity);
        int xPos = reg.get<Renderable>(entity).xPos;
        int xSize = reg.get<Renderable>(entity).xSize;
        int yPos = reg.get<Renderable>(entity).yPos;
        int ySize = reg.get<Renderable>(entity).ySize;

        int modifier = 0;

        if (yPos + ySize == bottomBorder) {
            return;
        } else if (grid[xPos][yPos + ySize + 1] || grid[xPos + xSize - 1][yPos + ySize + 1]) {
            modifier = ((rand() % 3) >= 2) ? 1 : -1;
            reg.patch<Renderable>(entity, [dt, modifier](auto &renderable) {
                renderable.xPos += dt * 30 * modifier;
            });
            // I'm not sure what this has to do with liquid collisions, so I commented it out
            // //Update vertice locations on triangle shapes
            // if(reg.all_of<Triangle,Physics>(entity)){
            //     reg.patch<Triangle>(entity, [dt, entity, &reg](auto &triangle){
            //         float deltaY=dt * reg.get<Physics>(entity).mass * GRAVITY;
            //         //functional operator "map" to update each point position
            //         std::transform(triangle.points.begin(), triangle.points.end(), triangle.points.begin(),[deltaY](glm::vec2 point){
            //             point.y+=deltaY;
            //             return(point);
            //         });
            //     });
            // }
        }
        int newX = reg.get<Renderable>(entity).xPos;

        if (modifier > 0) {
            for (int x = xPos + xSize; x < newX + xSize; x++) {
                for (int y = yPos; y < yPos + ySize; y++) {
                    if (grid[x][y]) {
                        reg.patch<Renderable>(entity, [&reg, dt, xPos](auto &renderable) {
                            renderable.xPos = xPos;
                        });
                        return;
                    }
                }
            }
        } else {
            for (int x = xPos - 1; x >= newX; x--) {
                for (int y = yPos; y < yPos + ySize; y++) {
                    if (grid[x][y]) {
                        reg.patch<Renderable>(entity, [&reg, dt, xPos, x](auto &renderable) {
                            renderable.xPos = xPos;
                        });
                        return;
                    }
                }
            }
        }
        for (int x = xPos; x < (xPos + xSize); x++) {
            for (int y = yPos; y < yPos + ySize; y++) {
                grid[x][y] = false;
            }
        }
        yPos = reg.get<Renderable>(entity).yPos;
        xPos = reg.get<Renderable>(entity).xPos;
        for (int x = xPos; x < (xPos + xSize); x++) {
            for (int y = yPos; y < (yPos + ySize); y++) {
                grid[x][y] = true;
            }
        }
    }
}*/

// deprecated collision methods from when we tested every entity with every other entity
/*
// Entity A left overlaps Entity B
bool Collision::leftOverlap(entt::registry &reg, entt::entity A, entt::entity B) {
    float  leftA  = reg.get<Renderable>(A).xPos,
           rightA = reg.get<Renderable>(A).xPos + (float)reg.get<Renderable>(A).xSize,
           leftB  = reg.get<Renderable>(B).xPos,
           rightB = reg.get<Renderable>(B).xPos + (float)reg.get<Renderable>(B).xSize;

    return ((leftA <= leftB) && (leftB <= rightA)) ? true : false;
}

// Entity A right overlaps Entity B
bool Collision::rightOverlap(entt::registry &reg, entt::entity A, entt::entity B) {

    float  leftA  = reg.get<Renderable>(A).xPos,
           rightA = reg.get<Renderable>(A).xPos + (float)reg.get<Renderable>(A).xSize,
           leftB  = reg.get<Renderable>(B).xPos,
           rightB = reg.get<Renderable>(B).xPos + (float)reg.get<Renderable>(B).xSize;

    return ((leftB <= leftA) && (leftA <= rightB)) ? true : false;
}

// Entity A top overlaps Entity B
bool Collision::topOverlap(entt::registry &reg, entt::entity A, entt::entity B) {
    
    float  topA  = reg.get<Renderable>(A).yPos,
           bottomA = reg.get<Renderable>(A).yPos + (float)reg.get<Renderable>(A).ySize,
           topB  = reg.get<Renderable>(B).yPos,
           bottomB = reg.get<Renderable>(B).yPos + (float)reg.get<Renderable>(B).ySize;

    return ((topA <= bottomB) && (bottomB <= bottomA)) ? true : false;
}

// Entity A bottom overlaps Entity B
bool Collision::bottomOverlap(entt::registry &reg, entt::entity A, entt::entity B) {

    double topA  = reg.get<Renderable>(A).yPos,
           bottomA = reg.get<Renderable>(A).yPos + (float)reg.get<Renderable>(A).ySize,
           topB  = reg.get<Renderable>(B).yPos,
           bottomB = reg.get<Renderable>(B).yPos + (float)reg.get<Renderable>(B).ySize;

    return ((topB <= bottomA) && (bottomA <= bottomB)) ? true : false;
}

bool Collision::xOverlap(entt::registry &reg, entt::entity A, entt::entity B) {
    if (leftOverlap(reg, A, B))
        return true;
    else if (rightOverlap(reg, A, B))
        return true;
    else
        return false;
}

bool Collision::yOverlap(entt::registry &reg, entt::entity A, entt::entity B) {
    if (topOverlap(reg, A, B))
        return true;
    else if (bottomOverlap(reg, A, B))
        return true;
    else
        return false;
}

DEPRECATED COLLISION CODE. BEFORE WE USED THE entt.hpp ENTITY COMPONENT SYSTEM
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

DEPRECATED METHOD FOR DETECTING TRIANGLE COLLISIONS
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
*/
