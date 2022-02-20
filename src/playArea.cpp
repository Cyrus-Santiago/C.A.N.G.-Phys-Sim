#include "../include/resourceManager.hpp"
#include "../include/playArea.hpp"
#include "../include/background.hpp"
#include "../include/input.hpp"

void playArea::Draw(SpriteRenderer &renderer){
    
}

void playArea::init(unsigned int scrHeight, unsigned int scrWidth){
    areaHeight=((scrHeight*0.5)-40);
    areaWidth=(scrWidth-40);
    glm::vec2 pos((scrHeight*0.75)-20,(scrWidth*0.5+20));
    glm::vec2 size(areaHeight,areaWidth);
    Background obj(pos, size, ResourceManager::GetTexture("button2"), glm::vec3(1.0f, 1.0f, 1.0f));
    
}