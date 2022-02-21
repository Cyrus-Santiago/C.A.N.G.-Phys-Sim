#include "../include/resourceManager.hpp"
#include "../include/playArea.hpp"
#include "../include/background.hpp"

void playArea::Draw(SpriteRenderer &renderer, unsigned int scrHeight, unsigned int scrWidth){
    areaHeight=((scrHeight*0.5)-40);
    areaWidth=(scrWidth-(scrWidth*0.1));
    glm::vec2 pos((scrWidth*0.05),(scrHeight*0.05));
    glm::vec2 size(areaWidth,areaHeight);
    Background obj(pos, size, ResourceManager::GetTexture("button2"), glm::vec3(1.0f, 1.0f, 1.0f));
    obj.Draw(renderer);
}