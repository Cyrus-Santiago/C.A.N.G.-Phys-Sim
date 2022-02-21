#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include "../include/spriteRenderer.hpp"
#include "../include/resourceManager.hpp"
#include <iostream>

class Character {
public:
    char character;
    glm::vec2 location;

    Character(const char character, glm::vec2 location);

    void Draw(SpriteRenderer &renderer, int charPosition, glm::vec2 screenPosition);
};