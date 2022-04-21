#include "../include/toolBox.hpp"

entt::entity Tools::getObject(entt::registry *reg, Click newMouseClick){
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

void Tools::moveObject(entt::registry *reg, Click newMouseClick){
    entt::entity clickedObject = getObject(reg, newMouseClick);
    if(!reg->valid(clickedObject)){ //Check if the user clicked a valid object (valid will return false if this is the case)
        return;
    }
    reg->patch<Renderable>(clickedObject, [reg, clickedObject,newMouseClick](auto &pos){
        pos.xPos = (int)newMouseClick.xPos-20, pos.yPos = (int)newMouseClick.yPos-20;
    });
}

entt::entity* Tools::outlineObject(entt::registry *reg, glm::vec2 shapeDimensions, Click newMouseClick, std::string type){
    glm::vec4 outlineColor = glm::vec4(1.0f,1.0f,0.0f,1.0f); //Outline should be yellow
    /*int xLeft = reg->get<Renderable>(obj).xPos;
    int xRight = xLeft + reg->get<Renderable>(obj).xSize;
    int yTop = reg->get<Renderable>(obj).yPos;
    int yBottom = yTop + reg->get<Renderable>(obj).ySize;*/
    factory.makeShape( *reg, glm::vec2((int) newMouseClick.xPos-20,
                (int)newMouseClick.yPos-20), outlineColor,shapeDimensions, type);
    return outline;
}

void Tools::deleteObject(entt::registry *reg, Click newMouseClick){
    entt::entity clickedObject = getObject(reg, newMouseClick);
    if(!reg->valid(clickedObject)){
        return;
    }
    reg->destroy(clickedObject);
}