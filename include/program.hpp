
#ifndef PROGRAM_H
#define PROGRAM_H

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <cmath>
#include "../include/glad.h"
#include "../include/glfw3.h"
#include "../include/game.hpp"
#include "../include/shader.hpp"
#include "../include/input.hpp"
#include "../include/audio.hpp"
class Program {
    public:
        static int program();
        static void endProgram();
};
#endif