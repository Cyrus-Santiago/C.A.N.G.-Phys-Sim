#include "../include/glad.h"
#include <GLFW/glfw3.h>
#include "../include/stb_image.h"
#include "../include/input.hpp"
#include <iostream>

// initialize private position coordinate variables
double Input::xPos = 0;
double Input::yPos = 0;

// initialize public click position coordinate vatiables
double Input::xClick = 0;
double Input::yClick = 0;

// This function is called by glfw whenever a mouse click occurs
void Input::mouseClickCallback(GLFWwindow * window, int button, int action, int mods) {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    // debug statement
    std::cout << "X=" << xPos << ", Y=" << yPos << std::endl;
    // we record the mouse click in the input class as a public variable
    xClick = xPos; yClick = yPos;
  }
}

// this function is called by glfw whenever the moust changes position
void Input::mousePosCallback(GLFWwindow * window, double x, double y) {
  // we keep track of the position in the Input class as a private variable
  xPos = x; yPos = y;
}

// function takes in user input and responds accordingly
void Input::processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}