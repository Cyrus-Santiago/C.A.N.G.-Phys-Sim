/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

#include "../include/game.hpp"

SpriteRenderer * Renderer;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Width(width), Height(height) {

}

Game::~Game() {

}

void Game::Init() {
  // load in shader files and name them sprite
  ResourceManager::LoadShader("src/sprite.vs", "src/sprite.fs", "sprite");

  // set projection matrix based on dimensions of screen (that way we can provide
  // our coordinates in easy to decipher pixel coordinates)
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
    static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
  // tell the shader we're going to be providing an image
  ResourceManager::GetShader("sprite").Use().SetInt("image", 0);
  // provide the shader with the projection matrix
  ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
  Shader myShader;
  // retrieve the shader we loaded earlier from storage
  myShader = ResourceManager::GetShader("sprite");
  // call sprite renderer on our shader
  Renderer = new SpriteRenderer(myShader);
  // load our image as a texture
  ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
}

void Game::Update(float dt) {

}

void Game::Render() {
  Texture2D myTexture;
  // get our texture from storage
  myTexture = ResourceManager::GetTexture("face");
  // render the sprite
  Renderer->DrawSprite(myTexture, glm::vec2(20, 490),
    glm::vec2(90, 30), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    // use compiled shader
    //ourShader.Use();
    // bind to vertex array buffer
    //glBindVertexArray(VAO);
}