#include <GLFW/glfw3.h>

class Input {
private:
    static double xPos, yPos;

public:
    static double xClick, yClick;

    static void mouseClickCallback(GLFWwindow * window, int button, int action, int mods);
    static void mousePosCallback(GLFWwindow * window, double xPos, double yPos);
    static void processInput(GLFWwindow *window);
};