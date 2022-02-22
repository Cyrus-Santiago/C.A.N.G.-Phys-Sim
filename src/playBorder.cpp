//This is Cyrus's code to draw the borders for the background :D
#include "../include/resourceManager.hpp"
#include "../include/playBorder.hpp"
#include "../include/glfw3.h"

void PlayBorder::Draw(SpriteRenderer &renderer){
    createLine( (scrWidth*0.05)+1, scrHeight*0.05, areaWidth-2, 3); //Top Line
    createLine( (scrWidth*0.05)+1, (scrHeight*0.05)+areaHeight, areaWidth-2, 3); //Bottom Line
    createLine( (scrWidth*0.05)-1, scrHeight*0.05, 3, areaHeight+3); //Left Line
    createLine( (scrWidth*0.05+areaWidth)-1, scrHeight*0.05, 3, areaHeight+3); //Right Line
    //Draw all Border Lines
    for(Background &lines : this->Border)
        lines.Draw(renderer);
}

void PlayBorder::createLine(unsigned int PosX, unsigned int PosY, unsigned int sizeX, unsigned int sizeY){
    glm::vec2 pos(PosX, PosY);
    glm::vec2 size(sizeX, sizeY);
    Background obj(pos, size, ResourceManager::GetTexture("button1"), glm::vec3(1.0f, 1.0f, 1.0f));
    this->Border.push_back(obj);
}