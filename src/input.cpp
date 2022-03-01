// This code was written mostly by Nate, with contributions from the rest of us

#include "../include/input.hpp"
#include "../include/audio.hpp"
#include <map>
#include <vector>
#include <algorithm>

Audio audio;
double Input::xClick = 1;
double Input::yClick = 1;
int Input::screenHeight=1;
int Input::screenWidth=1;
int Input::validClick=-1;
std::vector<Button> Input::Buttons;
//This function checks for the area in which a mouse click was made.
//If a button was clicked, its state is set to "pressed". 0,1 is returned
//a legal click is made. -1 is returned if no legal click is made.
int Input::determineAreaPressed(double xPos, double yPos)  {
  //If the click was in the menu area
  if(yPos > screenHeight * 0.6){
    for (int i = 0; i < Buttons.size(); ++i) {
      int upperBoundX=Buttons[i].Position.x + Buttons[i].Size.x;
      int upperBoundY=Buttons[i].Position.y + Buttons[i].Size.y;
      // checks bounds of current button and compares that to click data
      if ((xPos > Buttons[i].Position.x) && (xPos < upperBoundX) &&
        (yPos > Buttons[i].Position.y) && (yPos < upperBoundY)) {
        // records click
        xClick = xPos; yClick = yPos;
        // debug
        audio.playAudio();
        std::cout << "Button " << i << " pressed!" << std::endl;
        // records button press so we can do something with it
        if (Buttons[i].Pressed) Buttons[i].Pressed = false;
        else{
          std::transform(Buttons.begin(), Buttons.end(),Buttons.begin(),[](Button button){
            button.Pressed=false;
            return(button);
          });
          Buttons[i].Pressed = true;
        }
        //Returns 0 when a legal mouse click was made
        return 0;
      }
    }
  }
  //If the click is in the play area
  else if((xPos < screenWidth*0.95) && (xPos > screenWidth*0.0425) &&
    (yPos < screenHeight*0.45) && (yPos > screenHeight * 0.05)) {
    std::cout<<"in play area"<<std::endl;
    xClick = xPos; yClick = yPos;
    return 1;
  }
  std::cout<<"out of bounds"<<std::endl;
  return -1;
}

// This function is called by glfw whenever a mouse click occurs
void Input::mouseClickCallback(GLFWwindow * window, int button, int action, int mods) {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    // debug statement
    //std::cout << "X=" << xPos << ", Y=" << yPos << std::endl;
    // we record the mouse click in the input class as a public variable
    // declare variables that glfw will store mouse position data in
    double xPos, yPos;
    // call glfw to give us mouse position data
    glfwGetCursorPos(window, &xPos, &yPos);
    Input::validClick=Input::determineAreaPressed(xPos,yPos);
  }
}

// function takes in user input and responds accordingly
void Input::processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// allows us to access mouse position data without using global variables
Click Input::getLastMouseClickPos() {
  Click clickPosData(xClick, yClick);
  return clickPosData;
}

void Input::getButtonData(std::vector<Button> buttons) {
  Buttons = buttons;
}
std::vector<Button> Input::giveButtonData() {
  return Buttons;
}
