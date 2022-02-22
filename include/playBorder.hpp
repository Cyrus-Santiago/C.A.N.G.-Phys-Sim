//Cyrus again :) This is the header for the borders around the play area
#ifndef PLAYBORDER_H
#define PLAYBORDER_H

#include "../include/background.hpp"
#include "../include/playArea.hpp"
#include <vector>

class PlayBorder : public PlayArea{
    public:
        //Constructor
        PlayBorder(){};
        //Draw each line in the border
        void Draw(SpriteRenderer &renderer);
        //Array of Borders for each side
        std::vector<Background> Border;
    private:
        int x=0; //for the purposes of counting 
        unsigned int PosX, PosY, sizeX, sizeY;
        void createLine(unsigned int PosX, unsigned int PosY, unsigned int sizeX, unsigned int sizeY);
};

#endif