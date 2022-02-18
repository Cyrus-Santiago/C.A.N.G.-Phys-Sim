#include "../include/glfw3.h"

// class only exists so we have an object that holds click data,
// maybe I should've used a struct instead...
class Click {
    public:
        double xPos, yPos;
        Click(double x, double y) {
            xPos = x; yPos = y;
        }
};

// class that stores mouse click position, mouse position, and callback functions for
// glfw to call
class Input {

private:
    // these variables store where the mouse was clicked
    static double xClick, yClick;

public:
    // called by glfw on mouse click
    static void mouseClickCallback(GLFWwindow * window, int button, int action, int mods);
    // called by glfw on moust move
    static void mousePosCallback(GLFWwindow * window, double xPos, double yPos);
    // called by glfw on keyboard input
    static void processInput(GLFWwindow *window);

    static Click getLastMouseClickPos();
};

class Button {
    public:
        float top, bottom, left, right;
        Button(float T, float B, float L, float R) {
            top = T;
            bottom = B;
            left = L;
            right = R;
        }
};