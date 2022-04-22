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
*/
