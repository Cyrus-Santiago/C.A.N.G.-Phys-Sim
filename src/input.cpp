// This code was written by Nate

#include "../include/input.hpp"
#include <vector>

double Input::xClick = 1;
double Input::yClick = 1;

std::vector<Button> Input::Buttons;

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

    for (int i = 0; i < Buttons.size(); ++i) {
      // checks bounds of current button and compares that to click data
      if ((xPos > Buttons[i].Position.x) && (xPos < Buttons[i].Position.x + Buttons[i].Size.x) &&
          (yPos > Buttons[i].Position.y) && (yPos < Buttons[i].Position.y + Buttons[i].Size.y)) {
        // records click
        xClick = xPos; yClick = yPos;
        // debug
        std::cout << "Button " << i << " pressed!" << std::endl;
        // records button press so we can do something with it
        if (Buttons[i].Pressed)
          Buttons[i].Pressed = false;
        else
          Buttons[i].Pressed = true;
      }
    }
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