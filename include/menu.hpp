// This code was written by Nate

#ifndef MENU_H
#define MENU_H

#include "../include/button.hpp"
#include <glm/fwd.hpp>
#include <ostream>
#include <string>
#include <utility>
#include <glm/fwd.hpp>
#include <vector>

#define ELEMENT_NUM 30
#define SHAPE_NUM 3
#define RAY_NUM 2
#define TOOLBAR_NUM 9
// this class is mainly here just to own the buttons. It figures out the layout
// based on screen width/height and maintains an array of all the buttons
class Menu {
public:
    // for storing array of buttons
    static std::vector<Button> Buttons;

    // used to map an element type string to it's color
    struct Element {
        glm::vec4 color;
    };

    static Element elementDataArray[ELEMENT_NUM];
    static Element shapesButtonColor[SHAPE_NUM];
    static Element rayButtonColor[RAY_NUM];
    static Element toolbarButtonColor[TOOLBAR_NUM];
    static std::string rayTypes[RAY_NUM];
    static std::string shapesTypes[SHAPE_NUM];
    static std::string elementTypes[ELEMENT_NUM];
    static std::string toolbarTypes[TOOLBAR_NUM];

    // this is the elegant way I thought of storing the different elements
    static std::map<std::string, Element> Types;
        
    // constructor
    Menu() { }

    // destructor
    ~Menu();

    // rendering menu
    static void Draw(SpriteRenderer &renderer);

    // intialize level from button data
    static void init(unsigned int menuWidth, unsigned int menuHeight, unsigned int scrWidth, unsigned int scrHeight);
};

#endif
