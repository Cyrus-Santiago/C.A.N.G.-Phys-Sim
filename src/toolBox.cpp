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
    
    /*Check if object clicked is the border, and return null if that's the case*/
    if(checkBorder(reg, clickedObject)){
        return entt::null;
    }
    return clickedObject;
}

bool Tools::checkBorder(entt::registry *reg, entt::entity clickedObject){
    auto view2 = reg->view<Border>();
    for(auto object2 : view2){
        if(clickedObject == object2){
            return true; //Is a border
        }
    }
    return false; //Not a border
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

void Tools::lockObject(entt::registry *reg, Click newMouseClick){
    entt::entity clickedObject = getObject(reg, newMouseClick);
    if(!reg->valid(clickedObject)){ //Check if the user clicked a valid object (valid will return false if this is the case)
        return;
    }
    reg->patch<Physics>(clickedObject, [reg,clickedObject](auto &obj){
        obj.mass = 0.0f;
    });
    /*TO DO: Improve locking so that no other objects can move this object*/
}

void Tools::resizeObject(entt::registry *reg, Click newMouseClick){
    entt::entity clickedObject = getObject(reg, newMouseClick);
    if(!reg->valid(clickedObject)){
        return;
    }

    /*Extract relevant info from the clickedObject to draw outline*/
    int objX = reg->get<Renderable>(clickedObject).xPos;
    int objY = reg->get<Renderable>(clickedObject).yPos;
    int sizeX = reg->get<Renderable>(clickedObject).xSize;
    int sizeY = reg->get<Renderable>(clickedObject).ySize;

    /*Reformat into info about the edges of the box*/


}

entt::entity* Tools::outlineObject(entt::registry *reg, Click newMouseClick, std::string type){
    glm::vec4 outlineColor = glm::vec4(1.0f,1.0f,0.0f,1.0f); //Outline should be yellow
    entt::entity clickedObject = getObject(reg, newMouseClick); //Get the clicked object
    if(!reg->valid(clickedObject)){
        for(int i=0;i<4;i++){
            outline[i] = entt::null;
        }
        return outline;
    }

    /*Extract relevant info from the clickedObject to draw outline*/
    int objX = reg->get<Renderable>(clickedObject).xPos;
    int objY = reg->get<Renderable>(clickedObject).yPos;
    int sizeX = reg->get<Renderable>(clickedObject).xSize;
    int sizeY = reg->get<Renderable>(clickedObject).ySize;
    
    /*Build Outline*/
    auto topLine = reg->create();
    reg->emplace<Renderable>(topLine,"outline","button2",(float) objX+3,(float) objY,
    sizeX-6, 3, 1.0f, 1.0f, 0.0f, 1.0f);
    outline[0] = topLine;

    auto bottomLine = reg->create();
    reg->emplace<Renderable>(bottomLine,"outline","button2",(float) objX+3,(float) (objY+sizeY)-3,
    sizeX-6, 3, 1.0f, 1.0f, 0.0f, 1.0f);
    outline[1] = bottomLine;

    auto leftLine = reg->create();
    reg->emplace<Renderable>(leftLine,"outline","button2",(float) objX,(float) objY,
    3, sizeY, 1.0f, 1.0f, 0.0f, 1.0f);
    outline[2] = leftLine;

    auto rightLine = reg->create();
    reg->emplace<Renderable>(rightLine,"outline","button2",(float) (objX+sizeX)-3,(float) objY,
    3, sizeY, 1.0f, 1.0f, 0.0f, 1.0f);
    outline[3] = rightLine;

    return outline;
}

void Tools::clearOutline(entt::registry *reg, entt::entity* outline){
    int arraySize = sizeof(outline) / sizeof(entt::entity);
    for(int i=0; i<arraySize; i++){
        reg->destroy(outline[i]);
    }
}

void Tools::deleteObject(entt::registry *reg, Click newMouseClick){
    entt::entity clickedObject = getObject(reg, newMouseClick);
    if(!reg->valid(clickedObject)){
        return;
    }
    reg->destroy(clickedObject);
}

void Tools::clearAll(entt::registry *reg){
    auto view = reg->view<Renderable>();
    for(auto object : view){
        if(!checkBorder(reg,object)){
            reg->destroy(object);
        }
    }
}