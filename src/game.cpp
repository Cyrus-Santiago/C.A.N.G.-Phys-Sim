
/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

#include "../include/game.hpp"
#include "../include/input.hpp"
#include "../include/playArea.hpp"
#include "../include/ray.hpp"
#include "../include/explosion.hpp"
#include "../include/factory.hpp"
#include "../include/audio.hpp"
#include "../include/collision.hpp"
#include "../include/toolBox.hpp"
#include "../include/animation.hpp"

//#include "../include/simulation.hpp"
//#include "../include/simulationObject.hpp"
//#include "../include/entityMaestro.hpp"
//#include "../include/ecs.hpp"


#define GRAVITY 9.17
GLFWwindow *Window;
Menu menu;
PlayArea parea;
Audio sfxAudio;
//PlayBorder pborder;
SpriteRenderer * spriteRenderer;
//Simulation simulation;
Click newMouseClick;
Tools tools;
Input input;
Factory factory;
Animation *animationEngine;
entt::registry * reg;
Collision * colEngine;

std::vector<Button> Buttons;
//std::vector<SimulationObject> Border;
entt::entity entity;
//ECS::Entity entity1, entity2, entity3, entity4, entity5;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Width(width), Height(height){
}

Game::~Game() {
  delete spriteRenderer;
  delete reg;
  delete colEngine;
}

void Game::Init(GLFWwindow *window) {
  assert(Width>0);
  assert(Height>0);
  Window=window;
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
  colEngine = new Collision(* reg);

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
  auto view = reg->view<Border>();
  for (auto border : view) {
    colEngine->registerEntity(* reg, border);
    if (reg->get<Border>(border).position == "bottomBorder") {
      bottomBorder = reg->get<Renderable>(border).yPos;
    }
    if (reg->get<Border>(border).position == "topBorder") {
      topBorder = reg->get<Renderable>(border).yPos;
    }
  }
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
//  simulation.Update(dt);
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
          //Default shape is a square. For some reason I'm not allowed to put this
          //declaration inside the shape case.
          glm::vec2 shapeDimensions(40);
          //Determines what to do based on the game state
          entt::entity entity;
          switch(int(State)) {
            
            case GAME_DRAW_ELEMENT:
              if(Input::mousePressed){
                Input::mousePressHeldDown(Window);
                newMouseClick=input.getLastMouseClickPos();
              }
              entity = factory.makeParticle(* reg, pressedButton.Type,
                glm::vec2((int) newMouseClick.xPos, (int) newMouseClick.yPos), buttonColor);
              if (reg->all_of<Physics>(entity)) {
                if (!colEngine->registerEntity(* reg, entity))
                  reg->destroy(entity);
              }
              break;

            case GAME_DRAW_SHAPE:
              //Double length if shape is a rectangle
              if(pressedButton.Type=="RECTANGLE")
                shapeDimensions.x*=2;
              entity=factory.makeShape( *reg, glm::vec2((int) newMouseClick.xPos,
                (int)newMouseClick.yPos), buttonColor,shapeDimensions, pressedButton.Type);
              if (!colEngine->registerEntity(* reg, entity))
                reg->destroy(entity);
              break;            

            case GAME_DRAW_RAY:
              sfxAudio.playAudio("audio/zap.wav");
              factory.makeRay( *reg, glm::vec2((int) newMouseClick.xPos,
                (int)newMouseClick.yPos), glm::vec4(0.9f, 0.9f, 0.1f, 0.6f));
              break;

            case GAME_DRAW_BEAM:
              sfxAudio.playAudio("audio/zap.wav");
              factory.makeBeam( *reg, glm::vec2((int) newMouseClick.xPos,
                (int)newMouseClick.yPos), glm::vec4(0.9f, 0.9f, 0.1f, 0.6f));
              break;

            case GAME_DRAW_EXPLOSION:
              sfxAudio.playAudio("audio/blast.wav");
              factory.makeAnimation(*reg, glm::vec2(newMouseClick.xPos-10, newMouseClick.yPos-10),
              glm::vec4(1.0f), glm::vec2(20.0f),"explosion","explosion",0.75f,106.66f);
              for(int i=0; i<8; i++)  {
                for(int j=0; j<10; j++){
                  entity = factory.makeParticle(* reg, "FIRE",glm::vec2((int) 
                    newMouseClick.xPos+i+j+15, (int) newMouseClick.yPos+i+j+15), buttonColor);
                  //if (!colEngine.registerEntity(* reg, entity)){  reg->destroy(entity);}
                  entity = factory.makeParticle(* reg, "FIRE",glm::vec2((int) 
                    newMouseClick.xPos-i-j, (int) newMouseClick.yPos-i-j), buttonColor);
                  //if (!colEngine.registerEntity(* reg, entity)){  reg->destroy(entity);}
                }
                factory.makeForceVector(*reg, glm::vec2((int) newMouseClick.xPos-10,
                  (int)newMouseClick.yPos-10), Explosion::rotation[i], buttonColor, 
                  glm::vec2(Explosion::velocityArrayX[i], Explosion::velocityArrayY[i]));
              }
              break;

            case GAME_GLASSIFY:
              tools.glassify(reg, newMouseClick);
              break;

            /*Allows user to move object around the play area*/
            case GAME_MOVE_OBJECT:
              if(Input::mousePressed){
                Input::mousePressHeldDown(Window);
                newMouseClick=input.getLastMouseClickPos();
              } 
              //reg->replace<Physics>(clickedObject, 0.0f); //Make object weightless for manipulation
              tools.moveObject(reg,newMouseClick);
              break;

            case GAME_RESIZE_OBJECT:
              if(Input::mousePressed){
                Input::mousePressHeldDown(Window);
                newMouseClick=input.getLastMouseClickPos();
              }
              tools.outlineObject(reg, shapeDimensions, newMouseClick, pressedButton.Type);
              break;

            case GAME_DELETE_OBJECT:
              tools.deleteObject(reg, newMouseClick);
              break;

            case GAME_CLEAR:
              tools.clearAll(reg);
              break;
          }
        }
        break;
    }
    
    //Needed so that multiple clicks are not registered in one spot
    if (int(State) != GAME_DRAW_ELEMENT && int(State) != GAME_MOVE_OBJECT && int(State) != GAME_RESIZE_OBJECT)
      Input::resetValidClick();
  }
  animationEngine->animationUpdate(*reg, dt);
  Explosion::updateForcePositions(reg, dt);

  colEngine->collisionLoop(* reg, dt, bottomBorder, topBorder);

}

void Game::Render() {
  Texture2D texture = ResourceManager::GetTexture("button2");
  Buttons = Input::giveButtonData();
  parea.Draw(*spriteRenderer);
  //pborder.Draw(*spriteRenderer);
  // draws all the buttons
  Menu::Draw(*spriteRenderer);
  // draws every simulation object
//  simulation.Draw(*spriteRenderer);
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
  //Debug method to highlight wherever a grid spot is filled.
  // colEngine->debugGrid(* spriteRenderer, * reg);
}

//This function tells the game class which button is being pressed. The
//game state is changed based on that
GameState Game::determineGameState()  {
    std::vector<Button> pressedButton=Input::getButtonPressed();
    //If a button WAS pressed
    if(pressedButton.size()!=0) {
      //If the button pressed is an element
      if(pressedButton[0].ID >= 0 && pressedButton[0].ID < 30)  {
        return GAME_DRAW_ELEMENT;
      }
      //If the button pressed is a shape
      else if(pressedButton[0].ID > 29 && pressedButton[0].ID < 33)  {
          return GAME_DRAW_SHAPE;
      }
        //If the button pressed is light feature
      else if(pressedButton[0].ID == 33) {
          return GAME_DRAW_RAY;
      }
      else if(pressedButton[0].ID == 34) {
          return GAME_DRAW_BEAM;
      }
      else if(pressedButton[0].ID ==35) {
          return GAME_DRAW_EXPLOSION;
      }
      else if (pressedButton[0].ID == 36) {
        return GAME_GLASSIFY;
      }
      else if(pressedButton[0].ID == 37) {
        return GAME_MOVE_OBJECT;
      }
      else if (pressedButton[0].ID == 38) {
        return GAME_RESIZE_OBJECT;
      }
      else if (pressedButton[0].ID == 39) {
        return GAME_DELETE_OBJECT;
      }
      else if (pressedButton[0].ID == 40) {
        return GAME_CLEAR;
      }
    }
    return GAME_ACTIVE;
}