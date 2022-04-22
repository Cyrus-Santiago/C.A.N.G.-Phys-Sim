//Cyrus here, this one hurt emotionally, but it works now so yay!
#include "../include/playArea.hpp"

void PlayArea::Draw(SpriteRenderer &renderer){
    glm::vec2 pos(scrWidth*0.05, scrHeight*0.05);
    glm::vec2 size(areaWidth,areaHeight);
    Background obj(pos, size, ResourceManager::GetTexture("background"), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    obj.Draw(renderer);
}

void PlayArea::init(unsigned int Width, unsigned int Height){
    scrWidth=Width;
    scrHeight=Height;
    areaWidth=scrWidth*0.9;
    areaHeight=scrHeight*0.4;
}