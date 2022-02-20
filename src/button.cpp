// This code was written by Nate

#include "../include/button.hpp"

// constructor
Button::Button(glm::vec2 position, glm::vec2 size, Texture2D sprite,
                     glm::vec3 color, bool pressed) :
                     Position(position), Size(size),
                     Color(color), Pressed(pressed), Sprite(sprite) { }

// call to make drawing buttons simpler
void Button::Draw(SpriteRenderer &renderer, glm::vec3 color) {
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, 0.0f, color);
}