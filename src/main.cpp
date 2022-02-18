/* Code on this page was written with the help https://learnopengl.com */

#define STB_IMAGE_IMPLEMENTATION
#include "../include/glad.h"
#include "../include/glfw3.h"
#include "../include/stb_image.h"

#include "../include/shader_s.hpp"
#include "../include/input.hpp"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 860;
const unsigned int SCR_HEIGHT = 540;

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
    glfwSetCursorPosCallback(window, Input::mousePosCallback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    Shader ourShader("src/shader.vs", "src/shader.fs");

    // vertex points that will be used for drawing
    float vertices[] = {
        // positions
         0.8f,  0.8f, 0.0f, // top right    0
         0.8f, -0.5f, 0.0f, // bottom right 1
        -0.8f, -0.5f, 0.0f, // bottom left  2
        -0.8f,  0.8f, 0.0f, // top left     3

        // buttons
        -0.6f, -0.6f, 0.0f, // top right    4
        -0.6f, -0.7f, 0.0f, // bottom right 5
        -0.8f, -0.7f, 0.0f, // bottom left  6
        -0.8f, -0.6f, 0.0f  // top left     7

    };
		// tells GPU which vertices to draw triangles with in order to draw a rectangle
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle

        4, 5, 7,
        5, 6, 7
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    // generates buffer IDs for our Vertex buffer object and Element Buffer Object
		glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

		// bind the Vertex Array Object
    glBindVertexArray(VAO);

		// bind the Vertex Buffer Object to the GL_ARRAY_BUFFER type
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// copy vertex data into buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// bind the Element Buffer Object to the GL_ELEMENT_ARRAY_BUFFER type
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// copy indices data into buffer's memory
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // here we tell opengl how to interpret data we've placed in memory
    // arg1 specifies location of vertex attribute,
    // arg2 specifies the size of the vertex attribute,
    // arg3 specifies the data type and in GLSL vec * contains floats,
    // arg4 will normalize the data if set to true,
  	// arg5 defines the stride, how far to the next set of position data,
    // arg6 is the offset of where the position data begins in the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ourShader.use(); // activate shader

    /* RENDER LOOP */
    while (!glfwWindowShouldClose(window))
    {
				// check for user input
        Input::processInput(window);

        // set background color
        glClearColor(abs(sin(Input::xClick)), abs(sin(Input::xClick * Input::yClick)), abs(sin(Input::yClick)), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

				// use compiled shader
        ourShader.use();
				// bind to vertex array buffer
        glBindVertexArray(VAO);
				// draw elements based on vertexes
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
				
				// swaps color buffer and shows it as output to screen
        glfwSwapBuffers(window);
				// checks for events being triggered and calls relevant callback function
        glfwPollEvents();
    }

		// free memory and exit graciously
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

// this function gets called everytime the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		// changes viewport with window size
    glViewport(0, 0, width, height);
}
