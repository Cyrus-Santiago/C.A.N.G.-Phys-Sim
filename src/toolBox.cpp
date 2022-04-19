#include "../include/toolBox.hpp"
#include "../include/factory.hpp"

void Move::moveObject(entt::registry *reg, Click newMouseClick){
    entt::entity clickedObject = getObject(reg, newMouseClick);
    if(!reg->valid(clickedObject)){ //Check if the user clicked a valid object (valid will return false if this is the case)
        return;
    }
    reg->patch<Renderable>(clickedObject, [reg, clickedObject,newMouseClick](auto &pos){
        pos.xPos = (int)newMouseClick.xPos-20, pos.yPos = (int)newMouseClick.yPos-20;
    });
}

entt::entity Move::getObject(entt::registry *reg, Click newMouseClick){
    entt::entity clickedObject; //Used to update object
    auto view = reg->view<Renderable>();
    for(auto object : view){ //Find object that was clicked
        int objX = reg->get<Renderable>(object).xPos;
        int objY = reg->get<Renderable>(object).yPos;
        if((int)newMouseClick.xPos >= objX && (int)newMouseClick.xPos <= objX+reg->get<Renderable>(object).xSize){
            if((int)newMouseClick.yPos >= objY && (int)newMouseClick.yPos <= objY+reg->get<Renderable>(object).ySize){
                clickedObject = object; //Identify object that was clicked
                break; //Break out of for loop
            }
        }
    }
    
    return clickedObject;
}

void Resize::outlineObject(entt::registry *reg, entt::entity obj){
    int xLeft = reg->get<Renderable>(obj).xPos;
    int xRight = xLeft + reg->get<Renderable>(obj).xSize;
    int yTop = reg->get<Renderable>(obj).yPos;
    int yBottom = yTop + reg->get<Renderable>(obj).ySize;

    
}