/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

#include "../include/spriteRenderer.hpp"

// constructor
SpriteRenderer::SpriteRenderer(Shader &shader) {
    this->shader = shader;
    this->initRenderData();
}

// destructor
SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->quadVAO);
}

// initialize opengl data needed to render
void SpriteRenderer::initRenderData() {
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // provide our vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    // we want to enable element 0
    glEnableVertexAttribArray(0);
    // tells opengl how our vertex data is organized
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, 
  glm::vec2 size, float rotate, glm::vec4 color, glm::vec2 texZoom, glm::vec2 texPos)
{
    // prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    // move the sprite
    model = glm::translate(model, glm::vec3(position, 0.0f));  

    // move the origin of rotation
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
    // rotate the sprite
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
    // move the origin of rotation back
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    // scale the sprite
    model = glm::scale(model, glm::vec3(size, 1.0f)); 
  
    // provide this data to the shader
    this->shader.SetMatrix4("model", model);
    // provide color to the shader
    this->shader.SetVector4f("spriteColor", color);

    this->shader.SetVector2f("texPos", texPos);
    this->shader.SetVector2f("texZoom", texZoom);

    // set the texture
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    // tell GPU to draw it
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}