//This is Cyrus's code to draw the borders for the background :D
#include "../include/playBorder.hpp"

void PlayBorder::Draw(SpriteRenderer &renderer){
    createLine( (scrWidth*0.05)+1, scrHeight*0.05, areaWidth-2, 3); //Top Line
    createLine( (scrWidth*0.05)+1, (scrHeight*0.05)+areaHeight, areaWidth-2, 3); //Bottom Line
    createLine( (scrWidth*0.05)-1, scrHeight*0.05, 3, areaHeight+3); //Left Line
    createLine( (scrWidth*0.05+areaWidth)-1, scrHeight*0.05, 3, areaHeight+3); //Right Line
    //Draw all Border Lines
//    for(SimulationObject &lines : this->Border)
//        lines.Draw(renderer);
}

void PlayBorder::createLine(unsigned int PosX, unsigned int PosY, unsigned int sizeX, unsigned int sizeY){
    glm::vec2 pos(PosX, PosY);
    glm::vec2 size(sizeX, sizeY);
    glm::vec4 color(1.0f);
//    SimulationObject obj(pos, color, size, ResourceManager::GetTexture("button1"), glm::vec3(1.0f, 1.0f, 1.0f));
//    this->Border.push_back(obj);
}