#include "../include/collision.hpp"

bool Collision::detector(SimulationObject &obj1, SimulationObject &obj2){
    colX = obj1.Position.x + obj1.Size.x >= obj2.Position.x && 
        obj2.Position.x + obj2.Size.x >= obj1.Position.x;
    
    colY = obj1.Position.y + obj1.Size.y >= obj2.Position.y && 
        obj2.Position.y + obj2.Size.y >= obj1.Position.y;

    return colX && colY;
}

bool Collision::borderDet(SimulationObject &obj, Background &border){
    colX = obj.Position.x + obj.Size.x >= border.Position.x && 
        border.Position.x + border.Size.x >= obj.Position.x;
    
    colY = obj.Position.y + obj.Size.y >= border.Position.y && 
        border.Position.y + border.Size.y >= obj.Position.y;

    return colX && colY;
}

void Collision::collide(SimulationObject &obj){
    obj.Velocity = glm::vec2 (0.0f,0.0f);
}