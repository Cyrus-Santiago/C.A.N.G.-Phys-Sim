/* Code on this page was written with the help https://learnopengl.com */

#define STB_IMAGE_IMPLEMENTATION
#include "../include/glad.h"
#include "../include/glfw3.h"
#include "../include/stb_image.h"

#include "../include/shader.hpp"
#include "../include/input.hpp"
#include "../include/resourceManager.hpp"
#include "../include/game.hpp"

#include <iostream>
#include <cmath>

// settings
const unsigned int SCR_WIDTH = 860;
const unsigned int SCR_HEIGHT = 540;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, Input::mouseClickCallback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Game physSim(SCR_WIDTH, SCR_HEIGHT);
    physSim.Init();

    /* RENDER LOOP */
    while (!glfwWindowShouldClose(window))
    {
				// check for user input
        Input::processInput(window);

        // here we have a click object that holds position data, and we get that data
        // from the input class
        Click mousePosData = Input::getLastMouseClickPos();
        // set background color based on mouse position data (to demonstrate that it works)
        glClearColor(abs(sin(mousePosData.xPos)),
                     abs(sin(mousePosData.xPos * mousePosData.yPos)),
                     abs(sin(mousePosData.yPos)), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        physSim.Render();

				// draw elements based on vertexes
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
				
				// swaps color buffer and shows it as output to screen
        glfwSwapBuffers(window);
				// checks for events being triggered and calls relevant callback function
        glfwPollEvents();
    }

		// free memory and exit graciously
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}

// this function gets called everytime the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		// changes viewport with window size
    glViewport(0, 0, width, height);
}
