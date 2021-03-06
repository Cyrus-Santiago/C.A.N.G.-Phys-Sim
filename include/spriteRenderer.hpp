/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/shader.hpp"
#include "../include/texture.hpp"

class SpriteRenderer {
public:
    // constructor and destructor
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();

    // method that lets us specify texture, position, size, rotation and color of a sprite
    void DrawSprite(Texture2D &texture, glm::vec2 position,
        glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
        glm::vec4 color = glm::vec4(1.0f), glm::vec2 texZoom = glm::vec2(1.0f),
        glm::vec2 texPos = glm::vec2(0.0f));

private:
    Shader shader;
    unsigned int quadVAO;

    // used to declare vertices and do some dense opengl stuff
    void initRenderData();
};

#endif