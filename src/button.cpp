// This code was written by Nate

#include "../include/button.hpp"
#include "../include/menu.hpp"
#include <glm/fwd.hpp>
#include <string>
#include <iostream>

// constructor
Button::Button(glm::vec2 position, glm::vec2 size, std::string texture,
                     glm::vec4 color, std::string type, bool pressed,  int id):
                     Position(position), Size(size), Type(type),
                     Color(color), Pressed(pressed), Texture(texture),ID(id) { }

// call to make drawing buttons simpler
void Button::Draw(SpriteRenderer &spriteRenderer) {
    assert(Menu::Types.find(this->Type) != Menu::Types.end());
    glm::vec4 color = Menu::Types.at(this->Type).color;
    
    // this makes the button darker when it's pressed
    if (this->Pressed) {
        color *= glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    }
    // tell the sprite renderer what to draw
    Texture2D texture = ResourceManager::GetTexture(this->Texture);
    spriteRenderer.DrawSprite(texture, this->Position, this->Size, 0.0f,
        color);
    // tell the text renderer what to write
    TextRenderer::Draw(spriteRenderer, this->Type, color * 
        glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
}