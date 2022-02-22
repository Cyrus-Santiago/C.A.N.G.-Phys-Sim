#include <glm/detail/qualifier.hpp>
// This code was written by Nate

#ifndef CHARACTER_H
#define CHARACTER_H

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include "../include/spriteRenderer.hpp"
#include "../include/resourceManager.hpp"
#include <iostream>

// this holds a single character, like text character. Couldn't think of a
// better name sorry
class Character {
public:
    // holds the kind of character ex: 'A' and bitmap formatting coordinates
    char character;
    glm::vec2 location;
    double timeElapsed = 0.0f;

    // simple constructor
    Character(const char character, glm::vec2 location);

    // here we pass the data to the renderer. charPosition is a characters
    // position in a sentence or word screen position is where the char is
    // on the screen, and font size is obvious
    void Draw(SpriteRenderer &renderer, int charPosition,
              glm::vec2 screenPosition, unsigned int fontSize,
              bool fade = false, float dt = 0);
};

#endif