#include "glad.h"
#include <GLFW/glfw3.h>

// represents the state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_PAUSE
};

// class to hold a lot of the game logic so we can decouple it from
// window management stuff
class Game {    
    public:
        GameState State;
        bool Keys[1024];
        unsigned int Width, Height;

        // constuctor
        Game(unsigned int width, unsigned int height);
        ~Game();

        // initialize game state and load shaders and textures
        void Init();

        // game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
};