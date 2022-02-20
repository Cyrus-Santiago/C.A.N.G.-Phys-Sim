#include "../include/background.hpp"

Background::Background(glm::vec2 position, glm::vec2 size, Texture2D bgsprite, glm::vec3 color) :
                     Position(position), Size(size),
                     bgSprite(bgsprite), Color(color) {}

void Background::Draw(SpriteRenderer &renderer){
    renderer.DrawSprite(this->bgSprite, this->Position, this->Size);
}