/* Ray (Extends Simulation Object) - Class Methods
   Written by Amethyst Skye
   Description: Handles all operations performed on light ray(s) within simulation environment. */

#include "../include/ray.hpp"
#include <cmath>

    /* Print the contents of the rayOrigin array */
    void Ray::printRayCoords(){
        std::cout <<"Ray Origin: (" << Position[0] << ", " << Position[1] << ")" << std::endl;
    }

    /* Set position of the ray origin */
    glm::vec2 Ray::setPosition(double x, double y){
        Position[0] = (float)x;
        Position[1] = (float)y;

        return (Position);
    }
    /* Set coordinates where the ray will end */
    glm::vec2 Ray::setTail(double x, double y){
        Tail[0] = (float)x;
        Tail[1] = (float)y;

        return(Tail);
    }

    /* Ray Dimensions (length, width) */
    void Ray::setSize(glm::vec2 position, glm::vec2 tail){
        Size[0] = fabsf(tail[0]-position[0]);
        Size[1] = 0.01;
    }

    void Ray::successfulDraw(int x){
        if(x == 1){
            std::cout << "Successfully placed light ray" << std::endl;
            printRayCoords();
        }
        else
            std::cout << "Light ray could not be placed" << std::endl;
    }

    void Ray::init(){
        Position = {100, 100};
        Tail = {500, 250};
        setSize(Position, Tail);
        Texture = ResourceManager::GetTexture("laser");
        Velocity = {0, 0};
        Move(0);
        Destroyed = false;
    }
