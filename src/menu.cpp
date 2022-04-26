// This code was written by Nate

#include "../include/resourceManager.hpp"
#include "../include/menu.hpp"
#include "../include/input.hpp"


std::vector<Button> Menu::Buttons;

struct Element {
    glm::vec4 color;
};

Menu::Element Menu::elementDataArray[ELEMENT_NUM];

std::string Menu::elementTypes[ELEMENT_NUM];

std::map<std::string, Menu::Element> Menu::Types;

// destructor
Menu::~Menu() { }

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

    // stores the color data of the elements
    Element elementDataArray[] = {
        glm::vec4(0.2f, 0.2f, 1.0f, 1.0f), glm::vec4(0.8f, 0.8f, 1.0f, 0.6f),
        glm::vec4(0.6f, 0.6f, 1.0f, 1.0f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0.2f, 0.2f, 1.0f),
        glm::vec4(1.0f, 0.6f, 0.0f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f),
        glm::vec4(0.8f, 0.8f, 0.2f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f),
        glm::vec4(0.9f, 0.9f, 0.9f, 0.6f), glm::vec4(0.85f, 0.8f, 0.8f,1.0f),
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

    // stores the element names
    std::string elementTypes[] = {
        "WATER", "ICE", "HYDROGEN", "HELIUM", "OXYGEN", "FIRE", "LAVA",
        "STONE", "NITROGEN", "CHLORINE", "STEAM", "GOLD", "TIN", "TITANIUM",
        "CALCIUM", "NICKEL", "LEAD", "IODINE", "RADIUM", "LITHIUM", "SODIUM",
        "NEON", "FLUORINE", "ARGON", "MAGNESIUM", "PLATINUM", "TUNGSTEN",
        "BROMINE", "BISMUTH", "URANIUM"
    };

    // merges the element names and colors together in the map
    for (int i = 0; i < ELEMENT_NUM; i++) {
        Types.insert(std::pair<std::string, Element>(elementTypes[i],
            elementDataArray[i]));
    }

    /* Written by Griffen */
    Element shapesButtonColor[SHAPE_NUM-1];
    std::string shapeTypes[] = { "SQUARE", "TRIANGLE", "RECTANGLE" };
    for (int i=0; i<SHAPE_NUM; i++) {
        shapesButtonColor[i].color = glm::vec4(0.5f, 0.3f, 0.0f, 1.0f);
        Types.insert(std::pair<std::string, Element>(shapeTypes[i],
        shapesButtonColor[i]));
    }
    /* Written by Griffen */
    Element toolbarButtonColor[TOOLBAR_NUM];
    /* If you need to add another button, add new toolbarTypes and increase TOOLBAR_NUM in menu.hpp*/
    std::string toolbarTypes[] = { "BOOM!", "GLASSIFY!", "MOVE", "RESIZE", "DELETE", "CLEAR ALL"};
    for (int i=0; i<TOOLBAR_NUM; i++)   {
        toolbarButtonColor[i].color = glm::vec4(1.0f,0.2f,0.1f,1.0f);
        Types.insert(std::pair<std::string, Element>(toolbarTypes[i],
        toolbarButtonColor[i]));
    }

    /* Written by Amethyst */
    Element rayButtonColor[RAY_NUM-1];
    std::string rayTypes[] = {"RAY", "BEAM"};
    for (int i=0; i<RAY_NUM; i++) {
        rayButtonColor[i].color = glm::vec4(0.6f, 0.1f, 0.9f, 1.0f);
        Types.insert(std::pair<std::string, Element>(rayTypes[i],
        rayButtonColor[i]));
    } /* */

    int i = 0;
    int idCounter=0;
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
            Button obj(pos,buttonSize, "button2",
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), elementTypes[i],false, idCounter);
            i++;
            idCounter++;
            // add button object to Buttons array
            Buttons.push_back(obj);
        }
    }
    //For loop of initializing shape buttons - This was written by Griffen
    for(int y=0; y<SHAPE_NUM; ++y){
        //May change shrunkScrHeight to full screen height if need more buttons
        glm::vec2 boxPos(scrWidth, shrunkScrHeight + (y * scrHeightGap));
        Button shapeObj(boxPos,buttonSize, "button2",
            glm::vec4(1.0f, 1.0f,1.0f,1.0f), shapeTypes[y], false, idCounter);
        Buttons.push_back(shapeObj);
        idCounter++;
    }
    /* Initializing Ray Buttons - This was written by Amethyst */
    for(int z = 0; z < RAY_NUM; ++z){
        glm::vec2 boxPos(scrWidth, shrunkScrHeight + ((z+3) * scrHeightGap));
        Button rayObj(boxPos,buttonSize, "button2",
            glm::vec4(1.0f, 1.0f,1.0f,1.0f), rayTypes[z], false, idCounter);
        Buttons.push_back(rayObj);
        idCounter++;
    }
    /* Initializing the Toolbar Buttons */
    for(int x = 0; x < TOOLBAR_NUM; ++x)    {
        glm::vec2 boxPos(edgeGap + (x * widthGapBetweenBoxes),
            shrunkScrHeight-scrHeightGap);
        Button toolbarObj(boxPos,buttonSize, "button2",
            glm::vec4(1.0f,1.0f,1.0f,1.0f), toolbarTypes[x], false, idCounter);
        Buttons.push_back(toolbarObj);
        idCounter++;
    }
}
