//Cyrus again :) This is the header for the borders around the play area
#ifndef PLAYBORDER_H
#define PLAYBORDER_H

#include "../include/background.hpp"
#include "../include/playArea.hpp"
#include <vector>

class playBorder : public playArea{
    public:
        //Constructor
        playBorder(){};
        //Draw each line in the border
        void Draw(SpriteRenderer &renderer);
        //Array of Borders for each side
        std::vector<Background> Border;
    private:
        int x=0; //for the purposes of counting 
        unsigned int Pos;
};

#endif