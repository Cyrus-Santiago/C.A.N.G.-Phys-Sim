#include "../include/glad.h"
#include "../include/glfw3.h"
#include "../include/stb_image.h"
#include "../include/input.hpp"
#include <iostream>

double Input::xClick = 1;
double Input::yClick = 1;

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
    if(((xPos > 86) && (xPos < 172)) && ((yPos > 432) && (yPos < 459))) {
      xClick = xPos; yClick = yPos;
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
