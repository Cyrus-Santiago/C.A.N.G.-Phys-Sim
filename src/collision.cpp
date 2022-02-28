#include "../include/collision.hpp"

bool Collision::detector(SimulationObject &obj1, SimulationObject &obj2){
    colX = obj1.Position.x + obj1.Size.x >= obj2.Position.x && 
        obj2.Position.x + obj2.Size.x >= obj1.Position.x;
    
    colY = obj1.Position.y + obj1.Size.y >= obj2.Position.y && 
        obj2.Position.y + obj2.Size.y >= obj1.Position.y;

    return colX && colY;
}

bool Collision::checkCollision(SimulationObject &obj, std::vector<SimulationObject> Border){
    for(SimulationObject &border : Border){
        if(detector(obj,border)){
            return true;
        }
    }

    return false;
}

void Collision::collide(SimulationObject &obj){
    obj.Velocity = glm::vec2 (0.0f,0.0f);
}