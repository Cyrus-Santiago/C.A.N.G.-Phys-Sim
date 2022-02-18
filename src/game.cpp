/* Code on this page was written with the help https://learnopengl.com */

#include "../include/game.hpp"

Shader ourShader;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {

}

Game::~Game() {

}

void Game::Init() {
    // here we declare ourShader, and use the resource manager to load the
    // shaders and compile them, we've named these shaders default
    Shader ourShader;
    ourShader = ResourceManager::LoadShader("src/shader.vs", "src/shader.fs", "default");

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

    ourShader.Use(); // activate shader

}

void Game::Update(float dt) {

}

void Game::ProcessInput(float dt) {

}

void Game::Render() {
    // use compiled shader
    ourShader.Use();
    // bind to vertex array buffer
    //glBindVertexArray(VAO);
}