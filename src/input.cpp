#include "../include/glad.h"
#include <GLFW/glfw3.h>
#include "../include/stb_image.h"
#include "../include/input.hpp"
#include <iostream>

double xPos, yPos;

void Input::mouseClickCallback(GLFWwindow * window, int button, int action, int mods) {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    std::cout << "X=" << xPos << ", Y=" << yPos << std::endl;
  }
}

void Input::mousePosCallback(GLFWwindow * window, double x, double y) {
  xPos = x;
  yPos = y;
}

// function takes in user input and responds accordingly
void Input::processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}