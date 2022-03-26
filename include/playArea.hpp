// This class was written by Cyrus
#ifndef PLAYAREA_H
#define PLAYAREA_H

#include "../include/background.hpp"
#include "../include/resourceManager.hpp"
#include <vector>

class PlayArea{
    public:
        //Constructor for playArea
        PlayArea(){};
        //Renders the play area background
        void Draw(SpriteRenderer &renderer);
        //Calculates local variables for rendering
        void init(unsigned int Width, unsigned int Height);

        unsigned int areaHeight;
        unsigned int areaWidth;
        unsigned int scrHeight;
        unsigned int scrWidth;

};

#endif