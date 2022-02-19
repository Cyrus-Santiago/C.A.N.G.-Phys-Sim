/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */


#include "../include/input.hpp"

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

    // here I hardcoded in button position data, and update click if the mouse is clicked
    // within these bounds, otherwise it's not updated
    if ((xPos > Buttons[0].Position.x) && (xPos < Buttons[0].Position.x + Buttons[0].Size.x) && (yPos > Buttons[0].Position.y) && (yPos < Buttons[0].Position.y + Buttons[0].Size.y)) {
      xClick = xPos; yClick = yPos;
      std::cout << "BUTTON 1!" << std::endl;
    }
    if ((xPos > Buttons[1].Position.x) && (xPos < Buttons[1].Position.x + Buttons[1].Size.x) && (yPos > Buttons[1].Position.y) && (yPos < Buttons[1].Position.y + Buttons[1].Size.y)) {
      xClick = xPos; yClick = yPos;
      std::cout << "BUTTON 2!" << std::endl;
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