/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

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
const unsigned int SCR_HEIGHT = 860;
const char* menuTitle = "C.A.N.G. Physics Simulator";

Game physSim(SCR_WIDTH, SCR_HEIGHT);

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
    GLFWwindow* window;
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, menuTitle, NULL, NULL);
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

    physSim.Init();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    /* RENDER LOOP */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

				// check for user input
        Input::processInput(window);

        physSim.Update(deltaTime);

        // here we have a click object that holds position data, and we get that data
        // from the input class
        Click mousePosData = Input::getLastMouseClickPos();
        // set background color based on mouse position data (to demonstrate that it works)
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
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
    physSim.Width = width;
    physSim.Height = height;
}
