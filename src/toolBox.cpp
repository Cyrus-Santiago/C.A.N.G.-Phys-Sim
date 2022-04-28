#include "../include/toolBox.hpp"

/* Arguments: entt::registry, click newMouseClick
 * Returns:   entt::entity
 * Purpose:   Obtains an entity object that's within the play area
 *            based on the mouse click position. Returns entt::null if
 *            nothing valid is clicked. */
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

/* Arguments: entt::registry, entt::entity
 * Returns:   boolean
 * Purpose:   Simple function that checks whether an object is a border or not
 *            for the purposes of disallowing the user to disturb the border 
 *            with the toolbar functions. */
bool Tools::checkBorder(entt::registry *reg, entt::entity clickedObject){
    auto view2 = reg->view<Border>();
    for(auto object2 : view2){
        if(clickedObject == object2){
            return true; //Is a border
        }
    }
    return false; //Not a border
}

/* Arguments: entt::registry, click newMouseClick, collision engine pointer
 * Returns:   N/A
 * Purpose:   Moves object live by clearing space in collision grid, changing the
 *            rendered object's position and mapping the collision grid according to the
 *            new position. */
void Tools::moveObject(entt::registry &reg, Click newMouseClick, Collision *colEngine){
    entt::entity clickedObject = getObject(&reg, newMouseClick);
    if(!reg.valid(clickedObject)){ //Check if the user clicked a valid object (valid will return false if this is the case)
        return;
    }
    colEngine->entityUnclaim(reg,clickedObject,reg.get<Renderable>(clickedObject));
    reg.patch<Renderable>(clickedObject, [&reg, clickedObject,newMouseClick](auto &pos){
        pos.xPos = (int)newMouseClick.xPos-20, pos.yPos = (int)newMouseClick.yPos-20;
    });
    colEngine->entityClaim(reg,clickedObject,reg.get<Renderable>(clickedObject));
}

/* Arguments: entt::registry, click newMouseClick
 * Returns:   N/A
 * Purpose:   Changes and object's gravity vector and places a lock on the object
 *            that is used to determine what collisions to allow/disallow in the collision class. */
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

/* Arguments: entt::registry, click newMouseClick, collision engine pointer
 * Returns:   N/A
 * Purpose:   Resizes objects based on mouse clicks, adjusting the collision grid whilst the
 *            clicked object is resizes. */
void Tools::resizeObject(entt::registry &reg, Click newMouseClick, Collision *colEngine, bool growShrink){
    entt::entity clickedObject = getObject(&reg, newMouseClick);
    if(!reg.valid(clickedObject)){
        return;
    }

    /*Extract relevant info from the clickedObject to determine outline*/
    int objX = reg.get<Renderable>(clickedObject).xPos;
    int objY = reg.get<Renderable>(clickedObject).yPos;
    int sizeX = reg.get<Renderable>(clickedObject).xSize;
    int sizeY = reg.get<Renderable>(clickedObject).ySize;

    /*Find closest side clicked [DEPRECATED]
    int compareLine = (int)newMouseClick.yPos - objY; //Top Line
    int closest = 1;
    if(compareLine > (objY+sizeY) - (int)newMouseClick.yPos) closest = 2; //Bottom Line
    else if(compareLine > (int)newMouseClick.xPos - objX) closest = 3; //Left Line
    else if(compareLine > (objX+sizeX) - (int)newMouseClick.xPos) closest = 4; //Right Line
    */
    int resizeRate = 2;
    
    if(!growShrink){ //Left Click grows object
        colEngine->entityUnclaim(reg,clickedObject,reg.get<Renderable>(clickedObject));
        reg.patch<Renderable>(clickedObject, [&reg, clickedObject, resizeRate](auto &obj){
            obj.xPos-=(resizeRate);      obj.xSize+=resizeRate*2;
            obj.yPos-=(resizeRate);      obj.ySize+=resizeRate*2;
        });
        colEngine->entityClaim(reg,clickedObject,reg.get<Renderable>(clickedObject));
    }
    else{ //Right Click shrinks object
        colEngine->entityUnclaim(reg,clickedObject,reg.get<Renderable>(clickedObject));
        reg.patch<Renderable>(clickedObject, [&reg, clickedObject, resizeRate](auto &obj){
            obj.xPos+=(resizeRate);      obj.xSize-=resizeRate*2;
            obj.yPos+=(resizeRate);      obj.ySize-=resizeRate*2;
        });
        if(reg.get<Renderable>(clickedObject).yPos <= 2)
            reg.destroy(clickedObject);
        else
            colEngine->entityClaim(reg,clickedObject,reg.get<Renderable>(clickedObject));
    }
    /*Resize based on closest Line*/
    /*
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
    }*/

}

/* Arguments: entt::registry, click newMouseClick, string type
 * Returns:   N/A
 * Purpose:   Outlines object when resizing. DEPRECATED*/
/*
entt::entity* Tools::outlineObject(entt::registry &reg, Click newMouseClick, std::string type){
    glm::vec4 outlineColor = glm::vec4(1.0f,1.0f,0.0f,1.0f); //Outline should be yellow
    entt::entity clickedObject = getObject(&reg, newMouseClick); //Get the clicked object
    if(!reg.valid(clickedObject)){
        for(int i=0;i<4;i++){
            outline[i] = entt::null;
        }
        return outline;
    }

    //Extract relevant info from the clickedObject to draw outline
    int objX = reg.get<Renderable>(clickedObject).xPos;
    int objY = reg.get<Renderable>(clickedObject).yPos;
    int sizeX = reg.get<Renderable>(clickedObject).xSize;
    int sizeY = reg.get<Renderable>(clickedObject).ySize;
    
    //Build Outline
    auto topLine = reg.create();
    reg.emplace<Renderable>(topLine,"outline","button2",(int) objX+3,(int) objY,
    sizeX-6, 3, 1.0f, 1.0f, 0.0f, 1.0f);
    outline[0] = topLine;

    auto bottomLine = reg.create();
    reg.emplace<Renderable>(bottomLine,"outline","button2",(int) objX+3,(int) (objY+sizeY)-3,
    sizeX-6, 3, 1.0f, 1.0f, 0.0f, 1.0f);
    outline[1] = bottomLine;

    auto leftLine = reg.create();
    reg.emplace<Renderable>(leftLine,"outline","button2",(int) objX,(int) objY,
    3, sizeY, 1.0f, 1.0f, 0.0f, 1.0f);
    outline[2] = leftLine;

    auto rightLine = reg.create();
    reg.emplace<Renderable>(rightLine,"outline","button2",(int) (objX+sizeX)-3,(int) objY,
    3, sizeY, 1.0f, 1.0f, 0.0f, 1.0f);
    outline[3] = rightLine;

    return outline;
}
*/

/* Arguments: entt::registry, click newMouseClick
 * Returns:   N/A
 * Purpose:  Clears the outline around object. DEPRACATED*/
/*
void Tools::clearOutline(entt::registry &reg, entt::entity* outline){
    int arraySize = sizeof(outline) / sizeof(entt::entity);
    for(int i=0; i<arraySize; i++){
        reg.destroy(outline[i]);
    }
}
*/

/* Arguments: entt::registry, click newMouseClick
 * Returns:   N/A
 * Purpose:   Deletes an object from the simulation based on which object
 *            was clicked by the user. */
void Tools::deleteObject(entt::registry *reg, Click newMouseClick){
    entt::entity clickedObject = getObject(reg, newMouseClick);
    if(!reg->valid(clickedObject)){
        return;
    }
    reg->destroy(clickedObject);
}

/* Arguments: entt::registry, click newMouseClick
 * Returns:   N/A
 * Purpose:   Deletes all objects within the simulation. */
void Tools::clearAll(entt::registry *reg){
    auto view = reg->view<Renderable>();
    for(auto object : view){
        if(!checkBorder(reg,object)){
            reg->destroy(object);
        }
    }
}

/* Arguments: entity registry, and mouse click coordinates
 * Returns:   N/A
 * Purpose:   This method checks to see what type of entity is clicked
 *            and then will determine whether to glassify. Squares and 
 *            rectangles are valid entities to change material to glass.
 *            The user should be able to click on a shape while in 
 *            "glassify" mode and it will swap materials. Another click
 *            while in this mode will revert to a solid material. */
void Tools::glassify(entt::registry *reg, Click newMouseClick){
    entt::entity clickedObject = getObject(reg, newMouseClick);
    if(!reg->valid(clickedObject)){
        return;
    }
    /* If already glass texture, we can revert...no triangles yet */
    if(reg->any_of<Shape>(clickedObject) && !reg->any_of<Triangle>(clickedObject)){
        if(reg->any_of<Reflective>(clickedObject)){
        reg->patch<Renderable>(clickedObject, [reg, clickedObject, newMouseClick] (auto &material) {
            material.texture = "button2";
            material.colorR = 0.5f;
            material.colorG = 0.3f;
            material.colorB = 0.0f;
            material.colorA = 1.0f;
        });
        reg->erase<Reflective>(clickedObject);
        }
        else{ /* Otherwise, entity can be glassified */
            reg->patch<Renderable>(clickedObject, [reg, clickedObject, newMouseClick] (auto &material) {
            material.texture = "glass";
            material.colorR = 1.0f;
            material.colorG = 1.0f;
            material.colorB = 1.0f;
            material.colorA = 1.0f;
        });
        reg->emplace<Reflective>(clickedObject);
        }
    }
    else return;    
}