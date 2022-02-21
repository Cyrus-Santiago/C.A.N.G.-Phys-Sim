/* This code was written by Nate */

#ifndef BUTTON_H
#define BUTTON_H

#include <glm/glm.hpp>
#include <string>
#include "../include/spriteRenderer.hpp"
#include "game.hpp"

class Button {
public:
    glm::vec2 Position, Size;
    glm::vec3 Color;
    std::string Type;
    bool Pressed;
    Texture2D Sprite;

    Button(glm::vec2 position, glm::vec2 size, Texture2D sprite,
              glm::vec3 color = glm::vec3(1.0f), std::string type = "CHEMICAL", bool pressed = false);

    virtual void Draw(SpriteRenderer &spriteRenderer, TextRenderer textRenderer, glm::vec3 color);
};

#endif