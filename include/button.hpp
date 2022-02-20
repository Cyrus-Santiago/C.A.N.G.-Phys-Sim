/* This code was written by Nate */

#ifndef BUTTON_H
#define BUTTON_H

#include <glm/glm.hpp>
#include "../include/spriteRenderer.hpp"

class Button {
public:
    glm::vec2 Position, Size;
    glm::vec3 Color;
    bool Pressed;
    Texture2D Sprite;

    Button(glm::vec2 position, glm::vec2 size, Texture2D sprite,
              glm::vec3 color = glm::vec3(1.0f), bool pressed = false);

    virtual void Draw(SpriteRenderer &renderer, glm::vec3 color);
};

#endif