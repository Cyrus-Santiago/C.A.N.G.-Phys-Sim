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

void Tools::moveObject(entt::registry &reg, Click newMouseClick, Collision colEngine){
    entt::entity clickedObject = getObject(&reg, newMouseClick);
    if(!reg.valid(clickedObject)){ //Check if the user clicked a valid object (valid will return false if this is the case)
        return;
    }
    colEngine.destroyEnttAtLoc(reg, reg.get<Renderable>(clickedObject).xPos, reg.get<Renderable>(clickedObject).yPos);
    //colEngine.cleanBeforeMove(reg,clickedObject);
    reg.patch<Renderable>(clickedObject, [&reg, clickedObject,newMouseClick](auto &pos){
        pos.xPos = (int)newMouseClick.xPos-20, pos.yPos = (int)newMouseClick.yPos-20;
    });
    colEngine.registerEntity(reg, clickedObject);
    //colEngine.updateAfterMove(reg,clickedObject);
}

void Tools::lockObject(entt::registry *reg, Click newMouseClick){
    entt::entity clickedObject = getObject(reg, newMouseClick);
    if(!reg->valid(clickedObject)){ //Check if the user clicked a valid object (valid will return false if this is the case)
        return;
    }
    /*If it's already locked, unlock it*/
    if(reg->get<Physics>(clickedObject).lock)
        reg->patch<Physics>(clickedObject, [reg,clickedObject](auto &obj){
            obj.mass = 30.0f;
            obj.lock = false;
        });
    else //Otherwise you lock it in place
        reg->patch<Physics>(clickedObject, [reg,clickedObject](auto &obj){
            obj.mass = 0.0f;
            obj.lock = true;
        });
    
}

void Tools::resizeObject(entt::registry *reg, Click newMouseClick){
    entt::entity clickedObject = getObject(reg, newMouseClick);
    if(!reg->valid(clickedObject)){
        return;
    }

    /*Extract relevant info from the clickedObject to determine outline*/
    int objX = reg->get<Renderable>(clickedObject).xPos;
    int objY = reg->get<Renderable>(clickedObject).yPos;
    int sizeX = reg->get<Renderable>(clickedObject).xSize;
    int sizeY = reg->get<Renderable>(clickedObject).ySize;

    /*Find closest side clicked*/
    int compareLine = (int)newMouseClick.yPos - objY; //Top Line
    int closest = 1;
    if(compareLine > (objY+sizeY) - (int)newMouseClick.yPos) closest = 2; //Bottom Line
    else if(compareLine > (int)newMouseClick.xPos - objX) closest = 3; //Left Line
    else if(compareLine > (objX+sizeX) - (int)newMouseClick.xPos) closest = 4; //Right Line

    /*Resize based on closest Line*/
    switch(closest){
        case 1: //Top Line
            reg->patch<Renderable>(clickedObject, [reg, clickedObject,newMouseClick, objY](auto &size){
                size.yPos = size.ySize + ((int)newMouseClick.yPos-objY);
                size.ySize = 2*(size.ySize + ((int)newMouseClick.yPos-objY));
            });
        break;
        
        case 2: //Bottom Line
            reg->patch<Renderable>(clickedObject, [reg, clickedObject,newMouseClick, objY,sizeY](auto &size){
                size.ySize = size.ySize + ((objY+sizeY)-(int)newMouseClick.yPos);
            });
        break;
        
        case 3: //Left Line
            reg->patch<Renderable>(clickedObject, [reg, clickedObject,newMouseClick, objX](auto &size){
                size.xPos = size.xPos + ((int)newMouseClick.yPos-objX);
                size.xSize = 2*(size.xSize + ((int)newMouseClick.yPos-objX));
            });
        break;

        case 4: //Right Line
            reg->patch<Renderable>(clickedObject, [reg, clickedObject,newMouseClick, objX, sizeX](auto &size){
                size.xSize = size.xSize + ((objX+sizeX)-(int)newMouseClick.xPos);
            });
        break;
    }

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