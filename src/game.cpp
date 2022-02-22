/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

#include "../include/game.hpp"
#include "../include/input.hpp"
#include "../include/playBorder.hpp"
#include "../include/simulationObject.hpp"
#include "../include/simulation.hpp"
#include <ostream>
#include <glm/fwd.hpp>
#include <vector>

Menu menu;
playArea parea;
playBorder pborder;
SpriteRenderer * spriteRenderer;
TextRenderer textRenderer;
Simulation simulation;
std::vector<Button> Buttons;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Width(width), Height(height) {

}

Game::~Game() {

}

void Game::Init() {
  // load in shader files and name them sprite
  ResourceManager::LoadShader("src/shaders/sprite.vs", "src/shaders/sprite.fs", "sprite");

  // load our image as a texture
  ResourceManager::LoadTexture("textures/button2.png", true, "button2");
  ResourceManager::LoadTexture("textures/button1.jpg", false, "button1");
  ResourceManager::LoadTexture("textures/skyBackground.jpg", false, "skyBackground");
  ResourceManager::LoadTexture("textures/laser.png", true, "laser");
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

  // initialize menu
  menu.init(6, 5, Width, Height);
  parea.init(Width, Height);
  pborder.init(Width,Height);
  // give the button data to input class
  Input::getButtonData(menu.Buttons);
  // initialize the text renderer (actually manager)
  textRenderer.Init();
  simulation.Create(glm::vec2(50, 100), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  simulation.Create(glm::vec2(100, 100), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  SimulationObject simObj1 = simulation.Create(glm::vec2(150, 100));
  SimulationObject simObj2 = simulation.Create(glm::vec2(200, 100));
  simulation.Create(glm::vec2(250, 100), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
  simulation.Destroy(simObj1);
  simulation.Destroy(simObj2);
  simulation.Create(glm::vec2(150, 100), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  simulation.Create(glm::vec2(200, 100), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
}

void Game::Update(float dt) {
  simulation.Update(dt);
  textRenderer.Update(dt);
}

void Game::Render() {
  Buttons = Input::giveButtonData();
  parea.Draw(*spriteRenderer);
  pborder.Draw(*spriteRenderer);
  // draws all the buttons
  menu.Draw(*spriteRenderer, textRenderer);
  simulation.Draw(*spriteRenderer);
  if (Buttons[0].Pressed) {
    textRenderer.SetMessage(*spriteRenderer, Buttons[0].Type);
    textRenderer.FlashMessage(true);
  } else {
    textRenderer.SetMessage(*spriteRenderer, Buttons[0].Type);
    textRenderer.FlashMessage(false);
  }
}
