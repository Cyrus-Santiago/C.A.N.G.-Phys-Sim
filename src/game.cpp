/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

#include "../include/game.hpp"
#include "../include/input.hpp"
#include "../include/playBorder.hpp"
#include "../include/simulationObject.hpp"
#include "../include/ray.hpp"
#include "../include/simulation.hpp"
#include <glm/detail/qualifier.hpp>
#include <iostream>
#include <glm/fwd.hpp>
#include <vector>
#include <memory>

playArea parea;
playBorder pborder;
SpriteRenderer * spriteRenderer;
Simulation simulation;
std::vector<Button> Buttons;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Width(width), Height(height) {

}

Game::~Game() {
  delete spriteRenderer;
}

void Game::Init() {
  // load in shader files and name them sprite
  ResourceManager::LoadShader("src/shaders/sprite.vs", "src/shaders/sprite.fs", "sprite");

  // load our image as a texture
  ResourceManager::LoadTexture("textures/button2.png", true, "button2");
  ResourceManager::LoadTexture("textures/button1.jpg", false, "button1");
  ResourceManager::LoadTexture("textures/skyBackground.jpg", false, "skyBackground");
  ResourceManager::LoadTexture("textures/laser.png", true, "laser");
  ResourceManager::LoadTexture("textures/font2.png", true, "font2");

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

  // initialize menu
  Menu::init(6, 5, Width, Height);
  parea.init(Width, Height);
  pborder.init(Width,Height);

  // retrieve button data
  Buttons = Menu::Buttons;
  // give the button data to input class
  Input::getButtonData(Buttons);

  // initialize the text renderer (actually manager)
  TextRenderer::Init();
  simulation.Create(glm::vec2(50, 100), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  simulation.Create(glm::vec2(100, 100), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  SimulationObject simObj1 = simulation.Create(glm::vec2(150, 100));
  SimulationObject simObj2 = simulation.Create(glm::vec2(200, 100));
  simulation.Create(glm::vec2(250, 100), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
  simulation.Destroy(simObj1);
  simulation.Destroy(simObj2);
  simulation.Create(glm::vec2(150, 100), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  simulation.Create(glm::vec2(200, 100), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

  for (Button &button : Buttons) {
    TextRenderer::NewSentence(button.Type + " ", glm::vec2(40, 20), 20);
  }
}

void Game::Update(float dt) {
  simulation.Update(dt);
  TextRenderer::Update(dt);
}

void Game::Render() {
  Ray ray({100,100});
  simulation.Create(ray);
  Buttons = Input::giveButtonData();
  parea.Draw(*spriteRenderer);
  pborder.Draw(*spriteRenderer);
  // draws all the buttons
  Menu::Draw(*spriteRenderer);
  simulation.Draw(*spriteRenderer);
  for (Button &button : Buttons) {
    if (button.Pressed) {
      TextRenderer::Draw(*spriteRenderer, button.Type + " ",
        Menu::Types.at(button.Type).color);
    } else {
      TextRenderer::Hide(*spriteRenderer, button.Type + " ");
    }
  }
}
