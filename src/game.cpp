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
  Renderer->DrawSprite(texture, glm::vec2(0,   120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(0.0f, 0.0f)); // A
  Renderer->DrawSprite(texture, glm::vec2(20,  120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(1.1f, 0.0f)); // B
  Renderer->DrawSprite(texture, glm::vec2(40,  120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(2.2f, 0.0f)); // C
  Renderer->DrawSprite(texture, glm::vec2(60,  120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(3.3f, 0.0f)); // D
  Renderer->DrawSprite(texture, glm::vec2(80,  120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(4.4f, 0.0f)); // E
  Renderer->DrawSprite(texture, glm::vec2(100, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(5.5f, 0.0f)); // F
  Renderer->DrawSprite(texture, glm::vec2(120, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(6.6f, 0.0f)); // G
  Renderer->DrawSprite(texture, glm::vec2(140, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(7.7f, 0.0f)); // H
  Renderer->DrawSprite(texture, glm::vec2(160, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(8.8f, 0.0f)); // I
  Renderer->DrawSprite(texture, glm::vec2(180, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(0.0f, 1.04f)); // J
  Renderer->DrawSprite(texture, glm::vec2(200, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(1.1f, 1.04f)); // K
  Renderer->DrawSprite(texture, glm::vec2(220, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(2.2f, 1.04f)); // L
  Renderer->DrawSprite(texture, glm::vec2(240, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(4.4f, 1.04f)); // M
  Renderer->DrawSprite(texture, glm::vec2(260, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(5.5f, 1.04f)); // N
  Renderer->DrawSprite(texture, glm::vec2(280, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(6.6f, 1.04f)); // O
  Renderer->DrawSprite(texture, glm::vec2(300, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(7.7f, 1.04f)); // P
  Renderer->DrawSprite(texture, glm::vec2(320, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(8.8f, 1.04f)); // Q
  Renderer->DrawSprite(texture, glm::vec2(340, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(0.0f, 2.075f)); // R
  Renderer->DrawSprite(texture, glm::vec2(360, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(1.1f, 2.075f)); // S
  Renderer->DrawSprite(texture, glm::vec2(380, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(2.2f, 2.075f)); // T
  Renderer->DrawSprite(texture, glm::vec2(400, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(3.3f, 2.075f)); // U
  Renderer->DrawSprite(texture, glm::vec2(420, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(4.4f, 2.075f)); // V
  Renderer->DrawSprite(texture, glm::vec2(440, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(5.5f, 2.075f)); // W
  Renderer->DrawSprite(texture, glm::vec2(460, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(6.6f, 2.075f)); // X
  Renderer->DrawSprite(texture, glm::vec2(480, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(7.7f, 2.075f)); // Y
  Renderer->DrawSprite(texture, glm::vec2(500, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(8.8f, 2.075f)); // Z
}