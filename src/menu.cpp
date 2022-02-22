// This code was written by Nate

#include "../include/resourceManager.hpp"
#include "../include/menu.hpp"
#include "../include/input.hpp"

void Menu::Draw(SpriteRenderer &spriteRenderer, TextRenderer textRenderer) {
    // gets button data from input (in case the color changed or sum)
    Buttons = Input::giveButtonData();
    // draws every single button
    for (Button &button : this->Buttons)
        button.Draw(spriteRenderer, textRenderer, button.Color);
}

void Menu::init(unsigned int menuWidth, unsigned int menuHeight,
                unsigned int scrWidth, unsigned int scrHeight) {
    int i = 0;
    // very messy and stupid calculations that makes buttons evenly spaced in
    // a specific area
    for (int x = 0; x < menuWidth; ++x) {
        for (int y = 0; y < menuHeight; ++y) {
            glm::vec2 pos(20 + (x * ((scrWidth - 20) / menuWidth)), (scrHeight * 0.6) + (y * (scrHeight * 0.4) / menuHeight));
            glm::vec2 size((scrWidth / menuWidth) - 20, ((scrHeight * 0.4) / menuHeight) - 20);
            // load info into Button object
            Button obj(pos, size, ResourceManager::GetTexture("button2"),
                       glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Menu::Types[i]);
            i++;
            // add button object to Buttons array
            this->Buttons.push_back(obj);
        }
    }
}