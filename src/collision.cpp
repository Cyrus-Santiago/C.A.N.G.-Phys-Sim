#include "../include/collision.hpp"
#include "../include/factory.hpp"

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

void Collision::collide(SimulationObject &obj){
    obj.Velocity = glm::vec2 (0.0f,0.0f);
}