#include <GLFW/glfw3.h>

// class that stores mouse click position, mouse position, and callback functions for
// glfw to call
class Input {
private:
    // these variables record where the mouse currently is
    static double xPos, yPos;

public:
    // these variables store where the mouse was clicked
    static double xClick, yClick;

    // called by glfw on mouse click
    static void mouseClickCallback(GLFWwindow * window, int button, int action, int mods);
    // called by glfw on moust move
    static void mousePosCallback(GLFWwindow * window, double xPos, double yPos);
    // called by glfw on keyboard input
    static void processInput(GLFWwindow *window);
};