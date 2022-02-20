// This code was written by Nate

#ifndef MENU_H
#define MENU_H

#include "../include/button.hpp"
#include <vector>

class Menu {
public:
    // for storing array of buttons
    std::vector<Button> Buttons;
    // constructor
    Menu() { }
    // rendering menu
    void Draw(SpriteRenderer &renderer);

    // intialize level from button data
    void init(unsigned int menuWidth, unsigned int menuHeight, unsigned int scrWidth, unsigned int scrHeight);
};

#endif