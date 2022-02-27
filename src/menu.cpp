// This code was written by Nate

#include "../include/resourceManager.hpp"
#include "../include/menu.hpp"
#include "../include/input.hpp"
#include <glm/fwd.hpp>
#include <ostream>
#include <string>
#include <utility>

std::vector<Button> Menu::Buttons;

struct Element {
    glm::vec4 color;
};

Menu::Element Menu::elementDataArray[ELEMENT_NUM];

std::string Menu::elementTypes[ELEMENT_NUM];

std::map<std::string, Menu::Element> Menu::Types;

// destructor
Menu::~Menu() {
    Menu::Buttons.clear();
    Menu::Types.clear();
}

void Menu::Draw(SpriteRenderer &spriteRenderer) {
    // gets button data from input (in case the color changed or sum)
    Buttons = Input::giveButtonData();
    // draws every single button
    for (Button &button : Buttons) {
        TextRenderer::NewSentence(button.Type, glm::vec2(button.Position.x + 10,
            button.Position.y + 18), 14);
        button.Draw(spriteRenderer);
    }
}

void Menu::init(unsigned int menuWidth, unsigned int menuHeight,
                unsigned int scrWidth, unsigned int scrHeight) {

    Element elementDataArray[] = {
        glm::vec4(0.4f, 0.4f, 1.0f, 1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
        glm::vec4(0.6f, 0.6f, 1.0f, 1.0f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.5f, 0.5f, 0.2f, 1.0f),
        glm::vec4(0.8f, 0.8f, 1.0f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f),
        glm::vec4(0.8f, 0.8f, 0.2f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f),
        glm::vec4(1.0f, 0.7f, 0.7f, 1.0f), glm::vec4(0.85f, 0.8f, 0.8f,1.0f),
        glm::vec4(0.4f, 0.4f, 0.8f, 1.0f), glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
        glm::vec4(0.95f,0.95f,0.95f,1.0f), glm::vec4(0.5f, 0.4f, 0.4f, 1.0f),
        glm::vec4(1.0f, 0.4f, 0.4f, 1.0f), glm::vec4(0.5f, 0.3f, 0.3f, 1.0f),
        glm::vec4(0.8f, 0.4f, 0.8f, 1.0f), glm::vec4(0.2f, 0.2f, 1.0f, 1.0f),
        glm::vec4(0.4f, 0.3f, 0.4f, 1.0f), glm::vec4(0.2f, 0.8f, 0.2f, 1.0f),
        glm::vec4(0.8f, 0.3f, 0.8f, 1.0f), glm::vec4(0.8f, 0.5f, 0.8f, 1.0f),
        glm::vec4(0.1f, 0.1f, 0.8f, 1.0f), glm::vec4(0.8f, 0.8f, 0.6f, 1.0f),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.4f, 0.35f, 0.4f,1.0f),
        glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)
    };
    std::string elementTypes[] = {
        "WATER", "POTASSIUM", "HYDROGEN", "HELIUM", "OXYGEN", "MERCURY", "IRON",
        "CARBON", "NITROGEN", "CHLORINE", "COBALT", "GOLD", "TIN", "TITANIUM",
        "CALCIUM", "NICKEL", "LEAD", "IODINE", "RADIUM", "LITHIUM", "SODIUM",
        "NEON", "FLUORINE", "ARGON", "MAGNESIUM", "PLATINUM", "TUNGSTEN",
        "BROMINE", "BISMUTH", "URANIUM"
    };
    for (int i = 0; i < ELEMENT_NUM; i++) {
        Types.insert(std::pair<std::string, Element>(elementTypes[i],
            elementDataArray[i]));
    }
    /* Written by Griffen */
    Element shapesButtonColor[] = {glm::vec4(0.5f, 0.3f, 0.0f, 1.0f),
        glm::vec4(0.5f, 0.3f, 0.0f, 1.0f),glm::vec4(0.5f, 0.3f, 0.0f, 1.0f)};
    std::string shapeTypes[] = { "SQUARE", "TRIANGLE", "RECTANGLE" };
    for (int i=0; i<SHAPE_NUM; i++) {
        Types.insert(std::pair<std::string, Element>(shapeTypes[i],
        shapesButtonColor[i]));
    } /* */
    /* Written by Amethyst */
    Element rayButtonColor[] = {glm::vec4(0.6f, 0.1f, 0.9f, 1.0f),glm::vec4 (0.6f, 0.1f, 0.9f, 1.0f)};
    std::string rayTypes[] = {"RAY", "BEAM"};
    for (int i=0; i<RAY_NUM; i++) {
        Types.insert(std::pair<std::string, Element>(rayTypes[i],
        rayButtonColor[i]));
    } /* */

    int i = 0;
    // very messy and stupid calculations that makes buttons evenly spaced in
    // a specific area
    float edgeGap=0.025*scrWidth;  //About 20 pixels for a width of 1000
    float scrHeightGap=((scrHeight * 0.4) / menuHeight);
    float widthGapBetweenBoxes=((scrWidth - edgeGap) / menuWidth);
    float shrunkScrHeight=scrHeight * 0.6;
    glm::vec2 buttonSize(int(scrWidth / menuWidth) - edgeGap,
                scrHeightGap - edgeGap);
    //For loop of initializing element buttons
    for (int x = 0; x < menuWidth; ++x) {
        for (int y = 0; y < menuHeight; ++y) {
            glm::vec2 pos(edgeGap + (x * widthGapBetweenBoxes),
                shrunkScrHeight + (y * scrHeightGap));
            assert(Menu::Types.find(elementTypes[i]) != Menu::Types.end());
            // load info into Button object
            Button obj(pos,buttonSize, ResourceManager::GetTexture("button2"),
                       glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), elementTypes[i]);
            i++;
            // add button object to Buttons array
            Buttons.push_back(obj);
        }
    }
    //For loop of initializing shape buttons - This was written by Griffen
    for(int y=0; y<SHAPE_NUM; ++y){
        //May change shrunkScrHeight to full screen height if need more buttons
        glm::vec2 boxPos(scrWidth, shrunkScrHeight + (y * scrHeightGap));
        Button boxObj(boxPos,buttonSize, ResourceManager::GetTexture("button2"),
            glm::vec4(1.0f, 1.0f,1.0f,1.0f), shapeTypes[y]);
        Buttons.push_back(boxObj);
    }
    /* Initializing Ray Buttons - This was written by Amethyst */
    for(int z = 0; z < RAY_NUM; ++z){
        glm::vec2 boxPos(scrWidth, shrunkScrHeight + ((z+3) * scrHeightGap));
        Button rayObj(boxPos,buttonSize, ResourceManager::GetTexture("button2"),
            glm::vec4(1.0f, 1.0f,1.0f,1.0f), rayTypes[z]);
        Buttons.push_back(rayObj);
    }
}
