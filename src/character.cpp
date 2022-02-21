
#include "../include/character.hpp"

// constructor
Character::Character(const char character, glm::vec2 location) {
        this->character = character;
        this->location = location;
}

void Character::Draw(SpriteRenderer &renderer, int charPosition,
                     glm::vec2 screenPosition, unsigned int fontSize) {
    // get the font texture from our resource manager
    Texture2D fontTexture = ResourceManager::GetTexture("font");
    // hand off all data for this character to the spriteRenderer
    renderer.DrawSprite(fontTexture,
                        glm::vec2(screenPosition.x + ((fontSize * 0.80) * charPosition),
                        screenPosition.y), glm::vec2(fontSize, fontSize), 0.0f,
                        glm::vec3(1.0f), glm::vec2(0.1f), this->location);
}