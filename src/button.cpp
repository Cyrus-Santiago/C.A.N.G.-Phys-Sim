// This code was written by Nate

#include "../include/button.hpp"
#include <glm/fwd.hpp>
#include <string>
#include <iostream>

// constructor
Button::Button(glm::vec2 position, glm::vec2 size, Texture2D sprite,
                     glm::vec3 color, std::string type, bool pressed) :
                     Position(position), Size(size), Type(type),
                     Color(color), Pressed(pressed), Sprite(sprite) { }

// call to make drawing buttons simpler
void Button::Draw(SpriteRenderer &spriteRenderer, TextRenderer textRenderer, glm::vec3 color) {
    if (this->Type == "WATER") color = glm::vec3(0.4f, 0.4f, 1.0f);
    else if (this->Type == "MERCURY") color = glm::vec3(0.8f, 0.8f, 0.8f);
    else if (this->Type == "COBALT") color = glm::vec3(0.6f, 0.6f, 1.0f);
    else if (this->Type == "NICKEL") color = glm::vec3(0.6f, 0.6f, 0.6f);
    else if (this->Type == "SODIUM") color = glm::vec3(1.0f, 1.0f, 1.0f);
    else if (this->Type == "PLATINUM") color = glm::vec3(0.5f, 0.5f, 0.2f);
    else if (this->Type == "POTASSIUM") color = glm::vec3(0.8f, 0.8f, 1.0f);
    else if (this->Type == "MERCURY") color = glm::vec3(0.8f, 0.8f, 0.8f);
    else if (this->Type == "IRON") color = glm::vec3(0.9f, 0.9f, 0.9f);
    else if (this->Type == "GOLD") color = glm::vec3(0.8f, 0.8f, 0.2f);
    else if (this->Type == "LEAD") color = glm::vec3(0.4f, 0.4f, 0.4f);
    else if (this->Type == "NEON") color = glm::vec3(1.0f, 0.7f, 0.7f);
    else if (this->Type == "TUNGSTEN") color = glm::vec3(0.85f, 0.8f, 0.8f);
    else if (this->Type == "HYDROGEN") color = glm::vec3(0.4f, 0.4f, 0.8f);
    else if (this->Type == "CARBON") color = glm::vec3(0.3f, 0.3f, 0.3f);
    else if (this->Type == "TIN") color = glm::vec3(0.95f, 0.95f, 0.95f);
    else if (this->Type == "IODINE") color = glm::vec3(0.5f, 0.4f, 0.4f);
    else if (this->Type == "FLUORINE") color = glm::vec3(1.0f, 0.4f, 0.4f);
    else if (this->Type == "BROMINE") color = glm::vec3(0.5f, 0.3f, 0.3f);
    else if (this->Type == "HELIUM") color = glm::vec3(0.8f, 0.4f, 0.8f);
    else if (this->Type == "NITROGEN") color = glm::vec3(0.2f, 0.2f, 1.0f);
    else if (this->Type == "TITANIUM") color = glm::vec3(0.4f, 0.3f, 0.4f);
    else if (this->Type == "RADIUM") color = glm::vec3(0.2f, 0.8f, 0.2f);
    else if (this->Type == "MERCURY") color = glm::vec3(0.8f, 0.8f, 0.8f);
    else if (this->Type == "ARGON") color = glm::vec3(0.8f, 0.3f, 0.8f);
    else if (this->Type == "BISMUTH") color = glm::vec3(0.8f, 0.5f, 0.8f);
    else if (this->Type == "OXYGEN") color = glm::vec3(0.1f, 0.1f, 0.8f);
    else if (this->Type == "CHLORINE") color = glm::vec3(0.8f, 0.8f, 0.6f);
    else if (this->Type == "CALCIUM") color = glm::vec3(1.0f, 1.0f, 1.0f);
    else if (this->Type == "LITHIUM") color = glm::vec3(0.4f, 0.35f, 0.4f);
    else if (this->Type == "MAGNESIUM") color = glm::vec3(0.8f, 0.8f, 0.8f);
    else if (this->Type == "URANIUM") color = glm::vec3(1.0f, 1.0f, 0.0f);
    if (this->Pressed) color *= glm::vec3(0.5f);
    spriteRenderer.DrawSprite(this->Sprite, this->Position, this->Size, 0.0f, color);
    textRenderer.Draw(spriteRenderer, this->Type, glm::vec2(this->Position.x + 7, this->Position.y + 16), 14);
}