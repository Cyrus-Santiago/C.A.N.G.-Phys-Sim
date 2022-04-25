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
    
    /*Check if object clicked is the border, and return null is that's the case*/
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