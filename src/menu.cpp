#include "../include/resourceManager.hpp"
#include "../include/menu.hpp"
#include "../include/input.hpp"

void Menu::Draw(SpriteRenderer &renderer) {
    Buttons = Input::giveButtonData();
    for (Button &title : this->Buttons)
        title.Draw(renderer);
}

void Menu::init(unsigned int menuWidth, unsigned int menuHeight,
                unsigned int scrWidth, unsigned int scrHeight) {
    for (int x = 0; x < menuWidth; ++x) {
        for (int y = 0; y < menuHeight; ++y) {
            glm::vec2 pos(20 + (x * ((scrWidth - 20) / menuWidth)), (scrHeight * 0.6) + (y * (scrHeight * 0.4) / menuHeight));
            glm::vec2 size((scrWidth / menuWidth) - 20, ((scrHeight * 0.4) / menuHeight) - 20);
            Button obj(pos, size, ResourceManager::GetTexture("button1"),
                       glm::vec3(1.0f, 1.0f, 1.0f));
            this->Buttons.push_back(obj);
        }
    }
}