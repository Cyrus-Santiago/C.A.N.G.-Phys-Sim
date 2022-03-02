/* Although this code was physically typed out by Nate, he followed
a tutorial on https://learnopengl.com. Unless explicitly marked otherwise,
he does not feel comfortable claiming this code as his intellectual property
and it should not count towards his 1000 lines. */

#ifndef GAME_H
#define GAME_H

#include "glad.h"
#include <GLFW/glfw3.h>

#include "../include/resourceManager.hpp"
#include "../include/spriteRenderer.hpp"
#include "../include/textRenderer.hpp"

// represents the state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_DRAW_ELEMENT,
    GAME_DRAW_SHAPE,
    GAME_DRAW_RAY,
    GAME_DRAW_BEAM,
};

// class to hold a lot of the game logic so we can decouple it from
// window management stuff
class Maestro;

class Game {    
    public:
        GameState State;
        unsigned int Width, Height;

        // constuctor
        Game(unsigned int width, unsigned int height);
        ~Game();

        // initialize game state and load shaders and textures
        void Init();

        // game loop
        void Update(float dt);

        void Render();

    private:
        static GameState determineGameState();
        int bottomBorder;
};

#endif
