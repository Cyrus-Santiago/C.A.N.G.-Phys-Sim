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
#include "../include/explosion.hpp"
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

#define GRAVITY 9.17

Menu menu;
PlayArea parea;
//PlayBorder pborder;
SpriteRenderer * spriteRenderer;
Simulation simulation;
Click newMouseClick;
Input input;
Factory factory;
entt::registry * reg;

std::vector<Button> Buttons;
//std::vector<SimulationObject> Border;
entt::entity entity;
ECS::Entity entity1, entity2, entity3, entity4, entity5;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Width(width), Height(height) {
}

Game::~Game() {
  delete spriteRenderer;
  delete reg;
}

void Game::Init() {
  ResourceManager::initializeResources(); /* This will load all textures and shaders */

  // set projection matrix based on dimensions of screen (that way we can provide
  // our coordinates in easy to decipher pixel coordinates)
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
    static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

  // provide the shader with the projection matrix
  ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
  // retrieve the shader we loaded earlier from storage
  Shader myShader = ResourceManager::GetShader("sprite");
  // call sprite renderer on our shader
  spriteRenderer = new SpriteRenderer(myShader);
  reg = new entt::registry();

  // initialize menu, play area, play border, and input dimensions
  Menu::init(6, 5, Width*0.85, Height);
  parea.init(Width*0.85, Height);
  //pborder.init(Width*0.85,Height);
  Input::screenWidth=Width*0.85;
  Input::screenHeight=Height;
  // retrieve button data
  Buttons = Menu::Buttons;
  // retrieve border data
  //Border = pborder.Border;
  factory.makeBorder(*reg,Width*0.85,Height);
  // give the button data to input class
  Input::getButtonData(Buttons);
  //simulation.getBorder(Border);
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

  //entity = factory.makeParticle(* reg, glm::vec2(50, 50), glm::vec4(1.0f));
}

void Game::Update(float dt) {
  simulation.Update(dt);
  TextRenderer::Update(dt);
  newMouseClick = input.getLastMouseClickPos();
    //If there is a new mouse click
  if(Input::mousePressed) {
    switch(Input::validClick){
      case 0:   //Button Press
        //Alters the game state based on button pressed
        State=Game::determineGameState();
        break;
      case 1:   //Mouse click on play area
        //reg->replace<Renderable>(entity, (int) newMouseClick.xPos, (int) newMouseClick.yPos);
        //Determine color of particle based on button being pressed.
        std::vector<Button> pressedButtonVector=Input::getButtonPressed();
        if(pressedButtonVector.size() != 0)  {
          Button pressedButton=pressedButtonVector[0];
          glm::vec4 buttonColor=Menu::Types.at(pressedButton.Type).color;
          //Determines what to do based on the game state
          switch(int(State)) {
            case GAME_DRAW_ELEMENT:
              factory.makeParticle(* reg, glm::vec2((int) newMouseClick.xPos,
                (int) newMouseClick.yPos), buttonColor);
              break;
            case GAME_DRAW_SHAPE:
              factory.makeShape( *reg, glm::vec2((int) newMouseClick.xPos,
                (int)newMouseClick.yPos), buttonColor);
              break;
            case GAME_DRAW_RAY:
              factory.makeRay( *reg, glm::vec2((int) newMouseClick.xPos,
                (int)newMouseClick.yPos), glm::vec4(0.9f, 0.9f, 0.1f, 1.0f));
              break;
            case GAME_DRAW_BEAM:
              factory.makeRay( *reg, glm::vec2((int) newMouseClick.xPos,
                (int)newMouseClick.yPos), glm::vec4(0.9f, 0.9f, 0.1f, 1.0f));
              break;
            case GAME_DRAW_EXPLOSION:
              for(int i=0; i<8; i++)  {
                factory.makeForceVector(*reg, glm::vec2((int) newMouseClick.xPos,
                  (int)newMouseClick.yPos), Explosion::rotation[i], buttonColor, 
                  glm::vec2(Explosion::velocityArrayX[i], Explosion::velocityArrayY[i]));
              }
              break;
          }
        }
        break;
    }//Needed so that multiple clicks are not registered in one spot
    Input::resetValidClick();
  }
  Explosion::updateForcePositions(reg, dt);
  // create a view containing all the entities with the physics component
  auto view = reg->view<Physics>();
  // loop through each entity in the view
  for (auto entity : view) {
    // patch each entities Renderable component with a new y position to
    // simulate gravity
    reg->patch<Renderable>(entity, [dt, entity](auto &renderable) {
      renderable.yPos += dt * reg->get<Physics>(entity).mass * GRAVITY;
      //renderable.xPos += dt
    });
    
  }
}

void Game::Render() {
  Texture2D texture = ResourceManager::GetTexture("button2");
  Buttons = Input::giveButtonData();
  parea.Draw(*spriteRenderer);
  //pborder.Draw(*spriteRenderer);
  // draws all the buttons
  Menu::Draw(*spriteRenderer);
  // draws every simulation object
  simulation.Draw(*spriteRenderer);
  // for each button, calls the text renderer to display the button type upon
  // the button being pressed
  for (Button &button : Buttons) {
    if (button.Pressed) {
      // pass button type to textRenderer so it can be drawn
      TextRenderer::Draw(*spriteRenderer, button.Type + " ",
        Menu::Types.at(button.Type).color);
    } else {
      // turns the opacity of text to zero
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

  // creates view of all entities with the renderable component
  auto view = reg->view<Renderable>();
  // loops through each entity in the view we just created
  for (auto entity : view) {
    // calls on the factory to draw the entity
    factory.draw(* reg, entity, * spriteRenderer);
  }
}

//This function tells the game class which button is being pressed. The
//game state is changed based on that
GameState Game::determineGameState()  {
    std::vector<Button> pressedButton=Input::getButtonPressed();
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
      else if(pressedButton[0].ID > 32 && pressedButton[0].ID < 34) {
          std::cout<<"ray mode"<<std::endl;
          return GAME_DRAW_RAY;
      }
      else if(pressedButton[0].ID > 33 && pressedButton[0].ID < 35) {
          std::cout<<"beam mode"<<std::endl;
          return GAME_DRAW_BEAM;
      }
      else if(pressedButton[0].ID ==35) {
          std::cout<<"explosion mode"<<std::endl;
          return GAME_DRAW_EXPLOSION;
      }
    }
    std::cout<<"idle mode" <<std::endl;
    return GAME_ACTIVE;
}
