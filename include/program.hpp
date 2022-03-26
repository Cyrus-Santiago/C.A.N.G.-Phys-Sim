
#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <cmath>
#include "../include/glad.h"
#include "../include/glfw3.h"
#include "../include/game.hpp"
#include "../include/shader.hpp"
#include "../include/input.hpp"

class Program {
    public:
        static int program();
        static void endProgram();
};
#endif