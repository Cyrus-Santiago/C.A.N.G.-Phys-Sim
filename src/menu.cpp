#include "../include/menu.hpp"
#include "../include/resourceManager.hpp"

void Menu::Draw(SpriteRenderer &renderer) {
    for (Button &title : this->Buttons)
        title.Draw(renderer);
}

void Menu::init(unsigned int menuWidth, unsigned int menuHeight,
                unsigned int scrWidth, unsigned int scrHeight) {
    for (int x = 0; x < menuWidth; ++x) {
        for (int y = 0; y < menuHeight; ++y) {
            glm::vec2 pos(20 + (x * ((scrWidth - 20) / menuWidth)), (scrHeight * 0.6) + (y * (scrHeight * 0.4) / menuHeight));
            glm::vec2 size((scrWidth / menuWidth) - 20, ((scrHeight * 0.4) / menuHeight) - 20);
            Button obj(pos, size, ResourceManager::GetTexture("block_solid"),
                       glm::vec3(0.8f, 0.8f, 0.7f));
            this->Buttons.push_back(obj);
        }
    }
}