/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

#include "../include/game.hpp"
#include "../include/input.hpp"
#include <glm/fwd.hpp>
#include <ostream>

Menu menu;
SpriteRenderer * Renderer;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Width(width), Height(height) {

}

Game::~Game() {

}

void Game::Init() {
  // load in shader files and name them sprite
  ResourceManager::LoadShader("src/shaders/sprite.vs", "src/shaders/sprite.fs", "sprite");

  // load our image as a texture
  ResourceManager::LoadTexture("textures/button2.png", false, "button2");
  ResourceManager::LoadTexture("textures/button1.jpg", false, "button1");
  ResourceManager::LoadTexture("textures/font.png", true, "font");

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

  menu.init(6, 5, Width, Height);
  Input::getButtonData(menu.Buttons);
}

void Game::Update(float dt) {

}

void Game::Render() {
  menu.Draw(*Renderer);
  Texture2D texture = ResourceManager::GetTexture("font");
  Renderer->DrawSprite(texture, glm::vec2(100, 100), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.09f), glm::vec2(5.55f, 3.8f)); // H
  Renderer->DrawSprite(texture, glm::vec2(120, 100), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.09f), glm::vec2(3.2f, 3.8f));  // E
  Renderer->DrawSprite(texture, glm::vec2(140, 100), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.09f), glm::vec2(8.5f, 3.8f));  // L
  Renderer->DrawSprite(texture, glm::vec2(160, 100), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.09f), glm::vec2(8.5f, 3.8f));  // L
  Renderer->DrawSprite(texture, glm::vec2(180, 100), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.09f), glm::vec2(0.8f, 5.02f)); // O

  Renderer->DrawSprite(texture, glm::vec2(220, 100), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.09f), glm::vec2(7.55f, 5.02f));// W
  Renderer->DrawSprite(texture, glm::vec2(240, 100), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.09f), glm::vec2(0.8f, 5.02f)); // O
  Renderer->DrawSprite(texture, glm::vec2(260, 100), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.09f), glm::vec2(3.36f, 5.02f));// R
  Renderer->DrawSprite(texture, glm::vec2(280, 100), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.09f), glm::vec2(8.5f, 3.8f));  // L
  Renderer->DrawSprite(texture, glm::vec2(300, 100), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.09f), glm::vec2(2.35f, 3.8f)); // D
}