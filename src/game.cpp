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
//#include "../include/entityMaestro.hpp"
#include "../include/ecs.hpp"
#include "../include/factory.hpp"
#include <cassert>
#include <cstddef>
#include <glm/detail/qualifier.hpp>
#include <iostream>
#include <glm/fwd.hpp>
#include <iterator>
#include <vector>
#include <memory>
#include <algorithm>
static int determineGameState();
Menu menu;
PlayArea parea;
PlayBorder pborder;
SpriteRenderer * spriteRenderer;
Simulation simulation;
Click newMouseClick, oldMouseClick;
Input input;
Factory factory;
entt::registry reg;

std::vector<Button> Buttons;
std::vector<SimulationObject> Border;
entt::entity entity;
ECS::Entity entity1, entity2, entity3, entity4, entity5;

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

  // initialize menu, play area, play border, and input dimensions
  Menu::init(6, 5, Width*0.85, Height);
  parea.init(Width*0.85, Height);
  pborder.init(Width*0.85,Height);
  Input::screenWidth=Width*0.85;
  Input::screenHeight=Height;
  // retrieve button data
  Buttons = Menu::Buttons;
  // retrieve border data
  Border = pborder.Border;
  // give the button data to input class
  Input::getButtonData(Buttons);
  simulation.getBorder(Border);
  // initialize the text renderer (actually manager)
  TextRenderer::Init();
  for (Button &button : Buttons) {
    TextRenderer::NewSentence(button.Type + " ", glm::vec2(40, 20), 20);
  }
  /*entity1 = registry.create();
  registry.emplace<dimensions>(entity1, 50, 50, 10, 10);
  registry.emplace<physics>(entity1);*/
  /*maestro.addComponent(entity1, );
  maestro.addComponent(entity1, "physics");
  maestro.addComponent(entity1,"renderable");*/

  entity = factory.makeParticle(reg, glm::vec2(50, 50), glm::vec4(1.0f));

}

void Game::Update(float dt) {
  simulation.Update(dt);
  TextRenderer::Update(dt);
  newMouseClick = input.getLastMouseClickPos();
  //If there is a new mouse click
  if((newMouseClick.xPos != oldMouseClick.xPos) || (newMouseClick.yPos != oldMouseClick.yPos))  {
    oldMouseClick=newMouseClick;
    switch(input.validClick){
      case 0:
        //Alters the game state based on button pressed
        State=Game::determineGameState();
        break;
      case 1:
        std::cout<<"pretend something just got drawn"<<std::endl;
        break;
    }
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
  /*auto group = registry.group<dimensions>(entt::get<physics>);
  for(auto entity : group){
    auto& [dims, phys] = group.get<dimensions,physics>(entity);

  }
  //maestro.setRenderable(entity1,texture);
  spriteRenderer->DrawSprite(maestro.registry.get(entity1).renderable.texture,
    maestro.registry.get(entity1).renderable.position,
    maestro.registry.get(entity1).renderable.size
  );*/

  factory.drawParticle(reg, entity, * spriteRenderer);
  int xPos = newMouseClick.xPos;
  int yPos = newMouseClick.yPos;
  reg.emplace_or_replace<Renderable>(entity, xPos, yPos, 10, 10, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f);
}

//This function tells the game class which button is being pressed. The
//game state is changed based on that
GameState Game::determineGameState()  {
    std::vector<Button> pressedButton;
    //Update button list
    Buttons=Input::giveButtonData();
    //Copies the button that was pressed
    std::copy_if(Buttons.begin(), Buttons.end(), std::back_inserter(pressedButton),[](Button buttons){
      return buttons.Pressed;   });
    //If a button WAS pressed
    if(pressedButton.size()!=0) {
      //If the button pressed is an element
      if(pressedButton[0].ID >= 0 && pressedButton[0].ID < 30)  {
        std::cout<<"element mode" <<std::endl;
        return GAME_DRAW_ELEMENT;
      }
      //If the button pressed is a shape
      else if(pressedButton[0].ID > 29 && pressedButton[0].ID < 33)  {
        std::cout<<"shape mode"<<std::endl;
          return GAME_DRAW_SHAPE;
      }
        //If the button pressed is light feature
      else if(pressedButton[0].ID < 35) { 
          std::cout<<"light mode"<<std::endl;
          return GAME_DRAW_LIGHT;
      }
    }
    std::cout<<"idle mode" <<std::endl;
    return GAME_ACTIVE;
}