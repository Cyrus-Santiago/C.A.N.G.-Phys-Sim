
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
#include <unistd.h>
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
    auto enttBR =reg->get<Renderable>(border);
    if (reg->get<Border>(border).position == "bottomBorder")
      borderThreshold[0] = enttBR.yPos;
    if (reg->get<Border>(border).position == "topBorder")
      borderThreshold[1] = enttBR.yPos;
    if (reg->get<Border>(border).position == "leftBorder")
      borderThreshold[2] = enttBR.xPos-enttBR.xSize;
    if (reg->get<Border>(border).position == "rightBorder")
      borderThreshold[3] = enttBR.xPos+enttBR.xSize;
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
              tools.moveObject(*reg,newMouseClick,colEngine);
              break;
            case GAME_STASIS:
              tools.lockObject(reg,newMouseClick);
              break;

            case GAME_RESIZE_OBJECT:
              if(Input::mousePressed){
                Input::mousePressHeldDown(Window);
                newMouseClick=input.getLastMouseClickPos();
              }
              tools.resizeObject(*reg,newMouseClick,colEngine);
              //tools.outlineObject(reg, newMouseClick, pressedButton.Type);
              break;

            case GAME_DELETE_OBJECT:
              tools.deleteObject(reg, newMouseClick);
              break;

            case GAME_CLEAR:
              tools.clearAll(reg);
              break;

            case GAME_CHAOS:
              Game::determineChaos();
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

  colEngine->collisionLoop(* reg, dt, borderThreshold);
  auto flammableView= reg->view<Flammable>();
  for(auto enttFL : flammableView){
    if(reg->all_of<Animated>(enttFL)){
      auto enttFLR=reg->get<Renderable>(enttFL);
      int xRand= rand() % (int)enttFLR.xSize;
      int yRand= rand() % 5;
      entity = factory.makeParticle(* reg, "FIRE",glm::vec2((int) 
              enttFLR.xPos+xRand, (int) enttFLR.yPos-yRand), glm::vec4(1.0f,0.2f,0.2f,1.0f));
      entity = factory.makeParticle(* reg, "FIRE",glm::vec2((int) 
              enttFLR.xPos+xRand, (int) enttFLR.yPos+enttFLR.ySize+yRand), glm::vec4(1.0f,0.2f,0.2f,1.0f));
      yRand= rand() % (int)enttFLR.ySize;
      entity = factory.makeParticle(* reg, "FIRE",glm::vec2((int) 
              enttFLR.xPos-5, (int) enttFLR.yPos+yRand), glm::vec4(1.0f,0.2f,0.2f,1.0f));
      yRand= rand() % (int)enttFLR.ySize;
      entity = factory.makeParticle(* reg, "FIRE",glm::vec2((int) 
              enttFLR.xPos+enttFLR.xSize+5, (int) enttFLR.yPos+yRand), glm::vec4(1.0f,0.2f,0.2f,1.0f));
      reg->patch<Renderable>(enttFL, [dt](auto &renderable){
        renderable.colorR+=dt/4;
      });
    }
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
   //colEngine->debugGrid(* spriteRenderer, * reg);
}
/*
*Arguments: N/A
*Returns:   GameState Enum
*Purpose:   Tells the game class which button is being pressed. 
*           The game state is changed based on that */
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
      else if(pressedButton[0].ID == 38) {
        return GAME_STASIS;
      }
      else if (pressedButton[0].ID == 39) {
        return GAME_RESIZE_OBJECT;
      }
      else if (pressedButton[0].ID == 40) {
        return GAME_DELETE_OBJECT;
      }
      else if (pressedButton[0].ID == 41) {
        return GAME_CLEAR;
      }
      else if (pressedButton[0].ID == 42) {
        return GAME_CHAOS;
      }
    }
    return GAME_ACTIVE;
}
/*
*Arguments: N/A
*Returns:   N/A
*Purpose:   A random effect happens whenever you click on the play area with the
*           "CHAOS" button pressed down. Honestly, I thought I would have fun and show off a sample
*           of everything (or close to it) that the program is capable of doing. There is also some
*           random humor thrown into here. This method is not meant to be taken seriously and is simply
*           for comedic relief.*/
void Game::determineChaos(){
  srand(rand()+ int(newMouseClick.xPos * newMouseClick.yPos));
  int outcome=rand() % 18;
  //std::cout<<outcome<<std::endl;
  auto view =reg->view<Renderable>();
  switch(outcome){
    //Makes Flaming birds (phoenix?)
    case 1:
      for(int i=0;i<20;i++){
        entity=entity = factory.makeParticle(* reg, "BIRD",glm::vec2((int) 
          newMouseClick.xPos+(2*i),(int) newMouseClick.yPos+(2*i)), glm::vec4(1.0f));
        reg->emplace<Flammable>(entity);
        reg->emplace<Animated>(entity,10000.0f,0.01f);
      }
      break;
    //Close the game
    case 2:
      std::cout<<"Oops! You got unlucky and the game intentionally closed."<<std::endl;
      glfwSetWindowShouldClose(Window,true);
      break;
    //Turn all current light,shapes, and physical entities to the color green
    case 3:
      for(auto entt : view){
        if(reg->any_of<Physics,Shape,Light>(entt)){
          reg->patch<Renderable>(entt, [](auto &renderable){
            renderable.colorR=0.0f;
            renderable.colorG=1.0f;
            renderable.colorB=0.0f;
          });
        }
      }
      break;
    //Delete all entities on screen like the clear all tool
    case 4:
      tools.deleteObject(reg, newMouseClick);
      break;
    //Make a single, weird explosion force vector
    case 5:
      factory.makeForceVector(*reg, glm::vec2((int) newMouseClick.xPos-10,
        (int)newMouseClick.yPos-10), 123, glm::vec4(1.0f,0.0f,0.0f,1.0f), 
        glm::vec2(Explosion::velocityArrayX[0], Explosion::velocityArrayY[0]));
      break;
    //Glassify the first shape found, if any
    case 6:
      for(auto entt : view){
        if(reg->any_of<Shape>(entt)){
          tools.glassify(reg, newMouseClick);
          break;
        }
      }
      break;
    //Turn off the chaos button and turn on a random button
    case 7:{
      srand(time(0) * (int)newMouseClick.xPos * (int)newMouseClick.yPos);
      int buttonNum= rand() % 42;
      Buttons[42].Pressed=false;
      Buttons[buttonNum].Pressed=true;
      break;
    }
    //Make one explosion, water, and beam entity
    case 8:
      sfxAudio.playAudio("audio/zap.wav");
      factory.makeRay( *reg, glm::vec2((int) newMouseClick.xPos,
        (int)newMouseClick.yPos), glm::vec4(0.9f, 0.9f, 0.1f, 0.6f));
      factory.makeForceVector(*reg, glm::vec2((int) newMouseClick.xPos-10,
        (int)newMouseClick.yPos-10), 275, glm::vec4(1.0f,0.0f,0.0f,1.0f), 
        glm::vec2(Explosion::velocityArrayX[1], Explosion::velocityArrayY[1]));
      entity = factory.makeParticle(* reg, "WATER", glm::vec2((int) newMouseClick.xPos, 
        (int) newMouseClick.yPos), glm::vec4(0.0f,0.0f,1.0f,1.0f));
      if (reg->all_of<Physics>(entity)) {
        if (!colEngine->registerEntity(* reg, entity))reg->destroy(entity);
      }
      break;
    //Make an "animation" with resizing and a random texture and noise
    case 9:{
      srand(time(0) * (int)newMouseClick.xPos * (int)newMouseClick.yPos);
      int randomTexture= rand() % 4;
      int randomNoise= rand() % 5;
      std::string textureName;
      switch(randomTexture){
        case 0:
          textureName="button2";
          break;
        case 1:
          textureName="glass";
          break;
        case 2:
          textureName="explosion";
          break;
        default:
          textureName="triangle";
          break;
      }
      switch(randomNoise){
        case 0:
          sfxAudio.playAudio("audio/zap.wav");
          break;
        case 1:
          sfxAudio.playAudio("audio/blast.wav");
          break;
        case 2:
          sfxAudio.playAudio("audio/electric.wav");
          break;
        case 3:
          sfxAudio.playAudio("audio/blop.wav");
          break;
      }
      factory.makeAnimation(*reg, glm::vec2(newMouseClick.xPos-10, newMouseClick.yPos-10),
        glm::vec4(1.0f), glm::vec2(20.0f),textureName,"random",1.0f,106.66f);
      break;
    }
    //Make a black box that covers the screen and shrinks down over time
    case 10:
      factory.makeAnimation(*reg, glm::vec2(0,0), glm::vec4(1.0f), glm::vec2(1000.0f,860.0f),"button2",
        "random",0.5f,0.0f);
      break;
    //Set light rays and beams on fire
    case 11:
      factory.makeRay( *reg, glm::vec2((int) newMouseClick.xPos,
        (int)newMouseClick.yPos), glm::vec4(0.9f, 0.9f, 0.1f, 0.6f));
      for(auto entt : view){
        if(reg->all_of<Light>(entt)){
          reg->emplace<Animated>(entt,4.0f,0.0f);
          reg->emplace<Flammable>(entt);
        }
      }
      break;
    //Make a massive box that is on fire for a long, long time
    case 12:
      entity=factory.makeShape( *reg, glm::vec2((int) newMouseClick.xPos,
        (int)newMouseClick.yPos), glm::vec4(1.0f), glm::vec2(40.0f), "BIG BOX");
      if (!colEngine->registerEntity(* reg, entity)){  reg->destroy(entity);  }
      else  reg->emplace<Animated>(entity,100000.0f,0.0f);
      break;
    //Makes a shape with the gas component but no physics or flammable
    case 13:
      entity=factory.makeShape( *reg, glm::vec2((int) newMouseClick.xPos,
        (int)newMouseClick.yPos), glm::vec4(1.0f), glm::vec2(40.0f), "GAS BOX");
      if (!colEngine->registerEntity(* reg, entity)){  reg->destroy(entity);}
      else{
        reg->emplace<Gas>(entity);
        reg->erase<Physics>(entity);
        reg->erase<Flammable>(entity);
      }
      break;
    //Nothing can happen for 2 seconds
    case 14:
      usleep(2000000);
      break;
    //Print out Lorem ipsum placeholder text to stdout
    case 15:
      std::cout<<"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."<<std::endl;
      break;
    //Sit through a minute with blast.wav and electric.wav playing on repeat
    case 16:{
      bool blastOrWav=true;
      for(int i=0;i<60;i++){
        if(blastOrWav)    sfxAudio.playAudio("audio/blast.wav");
        else              sfxAudio.playAudio("audio/electric.wav");
        blastOrWav=!blastOrWav;
        usleep(1000000);
      }
      break;
    }
    //The noise gets loud and chaotic
    case 17:
      for(int i=0; i<2; i++){
        sfxAudio.playAudio("audio/playMusic1.wav");
        usleep(100000);
        sfxAudio.playAudio("audio/playMusic2.wav");
      }
    //Nothing happens
    default:
      break;
  }
}