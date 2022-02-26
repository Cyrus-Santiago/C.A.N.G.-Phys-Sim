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
#include "../include/ecs.hpp"
#include <glm/detail/qualifier.hpp>
#include <iostream>
#include <glm/fwd.hpp>
#include <vector>
#include <memory>

playArea parea;
playBorder pborder;
SpriteRenderer * spriteRenderer;
Simulation simulation;
ECS ecs;
ECS::Entity entity1, entity2;
std::vector<Button> Buttons;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Width(width), Height(height) {
}

Game::~Game() {
  delete spriteRenderer;
}

void Game::Init() {
  ResourceManager::initializeResources(); /* This will load all textures and shaders */

  // set projection matrix based on dimensions of screen (that way we can provide
  // our coordinates in easy to decipher pixel coordinates)
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
    static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

  // provide the shader with the projection matrix
  ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
  Shader myShader;
  // retrieve the shader we loaded earlier from storage
  myShader = ResourceManager::GetShader("sprite");
  // call sprite renderer on our shader
  spriteRenderer = new SpriteRenderer(myShader);

  // initialize menu
  Menu::init(6, 5, Width*0.85, Height);
  parea.init(Width*0.85, Height);
  pborder.init(Width*0.85,Height);

  // retrieve button data
  Buttons = Menu::Buttons;
  // give the button data to input class
  Input::getButtonData(Buttons);
  // initialize the text renderer (actually manager)
  TextRenderer::Init();
  for (Button &button : Buttons) {
    TextRenderer::NewSentence(button.Type + " ", glm::vec2(40, 20), 20);
  }
  entity1 = ecs.CreateEntity();
  entity1 = ecs.AddComponent(entity1, DIMENSIONID);
  entity1 = ecs.AddComponent(entity1, GRAVITYID);
  entity2 = ecs.CreateEntity();
  entity2 = ecs.AddComponent(entity2, DIMENSIONID);
  ecs.EntityToComponents.at(entity2.ID).dimension.xPos = 100;
}
void Game::Update(float dt) {
  simulation.Update(dt);
  TextRenderer::Update(dt);
  if (ecs.EntityHasComponent(entity1, GRAVITYID)) {
    ecs.EntityToComponents.at(entity1.ID).dimension.yPos += dt * 45;
  }
}
void Game::Render() {
  Texture2D texture = ResourceManager::GetTexture("button2");
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

  if (ecs.EntityHasComponent(entity1, DIMENSIONID)) {
    spriteRenderer->DrawSprite(texture,
      glm::vec2(ecs.EntityToComponents.at(entity1.ID).dimension.xPos,
      ecs.EntityToComponents.at(entity1.ID).dimension.yPos));
  }
  if (ecs.EntityHasComponent(entity2, DIMENSIONID)) {
    spriteRenderer->DrawSprite(texture,
      glm::vec2(ecs.EntityToComponents.at(entity2.ID).dimension.xPos,
      ecs.EntityToComponents.at(entity2.ID).dimension.yPos));
  }
}
