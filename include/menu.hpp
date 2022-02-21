// This code was written by Nate

#ifndef MENU_H
#define MENU_H

#include "../include/button.hpp"
#include <vector>

class Menu {
public:
    // for storing array of buttons
    std::vector<Button> Buttons;

    std::vector<std::string> Types = {"WATER", "POTASSIUM", "HYDROGEN", "HELIUM", "OXYGEN", "MERCURY", "IRON", "CARBON", "NITROGEN", "CHLORINE",
                                      "COBALT", "GOLD", "TIN", "TITANIUM", "CALCIUM", "NICKEL", "LEAD", "IODINE", "RADIUM", "LITHIUM",
                                      "SODIUM", "NEON", "FLUORINE", "ARGON", "MAGNESIUM", "PLATINUM", "TUNGSTEN", "BROMINE", "BISMUTH", "URANIUM"};
    // constructor
    Menu() { }
    // rendering menu
    void Draw(SpriteRenderer &renderer, TextRenderer textRenderer);

    // intialize level from button data
    void init(unsigned int menuWidth, unsigned int menuHeight, unsigned int scrWidth, unsigned int scrHeight);
};

#endif