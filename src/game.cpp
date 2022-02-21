/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

#include "../include/game.hpp"
#include "../include/input.hpp"
#include <glm/fwd.hpp>
#include <ostream>

Menu menu;
SpriteRenderer * spriteRenderer;
TextRenderer textRenderer;

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
  spriteRenderer = new SpriteRenderer(myShader);

  menu.init(6, 5, Width, Height);
  Input::getButtonData(menu.Buttons);
  textRenderer.Init();
}

void Game::Update(float dt) {

}

void Game::Render() {
  menu.Draw(*spriteRenderer);
  textRenderer.Draw(*spriteRenderer, "LONG AGO IN A GALAXY", glm::vec2(20, 20));
  textRenderer.Draw(*spriteRenderer, "FAR FAR AWAY...", glm::vec2(20, 50));
  /*Texture2D texture = ResourceManager::GetTexture("font");
  spriteRenderer->DrawSprite(texture, glm::vec2(0,   120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(0.05f, 0.05f)); // A
  spriteRenderer->DrawSprite(texture, glm::vec2(40,  120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(1.17f, 0.05f)); // B
  spriteRenderer->DrawSprite(texture, glm::vec2(80,  120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(2.305f,0.05f)); // C
  spriteRenderer->DrawSprite(texture, glm::vec2(120, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(3.37f, 0.05f)); // D
  spriteRenderer->DrawSprite(texture, glm::vec2(160, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(4.47f, 0.05f)); // E
  spriteRenderer->DrawSprite(texture, glm::vec2(200, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(5.57f, 0.05f)); // F
  spriteRenderer->DrawSprite(texture, glm::vec2(240, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(6.67f, 0.05f)); // G
  spriteRenderer->DrawSprite(texture, glm::vec2(280, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(7.77f, 0.05f)); // H
  spriteRenderer->DrawSprite(texture, glm::vec2(320, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(8.87f, 0.05f)); // I
  spriteRenderer->DrawSprite(texture, glm::vec2(360, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(0.07f, 1.09f)); // J
  spriteRenderer->DrawSprite(texture, glm::vec2(400, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(1.17f, 1.05f)); // K
  spriteRenderer->DrawSprite(texture, glm::vec2(440, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(2.27f, 1.09f)); // L
  spriteRenderer->DrawSprite(texture, glm::vec2(480, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(4.47f, 1.09f)); // M
  spriteRenderer->DrawSprite(texture, glm::vec2(520, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(5.57f, 1.09f)); // N
  spriteRenderer->DrawSprite(texture, glm::vec2(560, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(6.67f, 1.09f)); // O
  spriteRenderer->DrawSprite(texture, glm::vec2(600, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(7.77f, 1.09f)); // P
  spriteRenderer->DrawSprite(texture, glm::vec2(640, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(8.87f, 1.09f)); // Q
  spriteRenderer->DrawSprite(texture, glm::vec2(680, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(0.07f, 2.09f)); // R
  spriteRenderer->DrawSprite(texture, glm::vec2(720, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(1.17f, 2.09f)); // S
  spriteRenderer->DrawSprite(texture, glm::vec2(760, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(2.27f, 2.09f)); // T
  spriteRenderer->DrawSprite(texture, glm::vec2(800, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(3.37f, 2.09f)); // U
  spriteRenderer->DrawSprite(texture, glm::vec2(840, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(4.47f, 2.09f)); // V
  spriteRenderer->DrawSprite(texture, glm::vec2(880, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(5.57f, 2.09f)); // W
  spriteRenderer->DrawSprite(texture, glm::vec2(920, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(6.67f, 2.09f)); // X
  spriteRenderer->DrawSprite(texture, glm::vec2(960, 120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(7.77f, 2.09f)); // Y
  spriteRenderer->DrawSprite(texture, glm::vec2(1000,120), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f), glm::vec2(0.1f), glm::vec2(8.87f, 2.09f)); // Z*/
}