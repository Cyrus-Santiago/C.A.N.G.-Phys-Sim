//Cyrus here, this is to render a backgroud and border for the play area :)
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <glm/glm.hpp>
#include "../include/spriteRenderer.hpp"

class Background{
public:
    glm::vec2 Position, Size;
    glm::vec3 Color;
    Texture2D bgSprite;

    Background(glm::vec2 Position, glm::vec2 Size, Texture2D bgSprite, glm::vec3 Color = glm::vec3(1.0f));
    virtual void Draw(SpriteRenderer &renderer);
};

#endif