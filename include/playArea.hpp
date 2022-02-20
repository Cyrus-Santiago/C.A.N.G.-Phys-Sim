// This class was written by Cyrus
#ifndef PLAYAREA_H
#define PLAYAREA_H

#include "../include/background.hpp"
#include <vector>

class playArea{
    public:
        //Constructor for playArea
        playArea(){};
        //Renders the play area background
        void Draw(SpriteRenderer &renderer);
        //Initializes the area
        void init(unsigned int scrHeight, unsigned int scrWidth);

    private:
        unsigned int areaHeight;
        unsigned int areaWidth;

};

#endif