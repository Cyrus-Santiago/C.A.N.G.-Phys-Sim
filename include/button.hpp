/* This code was written by Nate */

#ifndef BUTTON_H
#define BUTTON_H

#include <glm/glm.hpp>
#include <string>
#include "../include/spriteRenderer.hpp"
#include "game.hpp"

// holds data about buttons like; what is it for? where is it? how big is it?
// what texture does it use? was it pressed? and also draws buttons.
class Button {
public:
    glm::vec2 Position, Size;
    glm::vec4 Color;
    std::string Type;
    bool Pressed;
    std::string Texture;
    int ID;

    // constructor! by default the color is the same as the original texture,
    // it's for a chemcial and it's not pressed
    Button(glm::vec2 position, glm::vec2 size, std::string sprite,
              glm::vec4 color = glm::vec4(1.0f), std::string type = "CHEMICAL", 
              bool pressed = false,  int ID =0);
    //Placeholder Button
    Button(){}
    // here we set the color, add the text, and pass the button to the sprite renderer
    virtual void Draw(SpriteRenderer &spriteRenderer);
};

#endif