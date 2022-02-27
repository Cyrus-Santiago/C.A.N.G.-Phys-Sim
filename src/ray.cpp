/* Ray (Extends Simulation Object) - Class Methods
   Written by Amethyst Skye
   Description: Handles all operations performed on light ray(s) within simulation environment. */

#include "../include/ray.hpp"

    /* Print the contents of the rayOrigin array */
    void Ray::printRayStats(){
        std::cout <<"Origin: (" << Position[0] << ", " << Position[1] << ")" << std::endl;
        std::cout <<"Tail: (" << Tail[0] << ", " << Tail[1] << ")" << std::endl;
        std::cout <<"Size: " << fabsf(Tail[0]-Position[0]) << " x " << Size[1] << std::endl;
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
        Size[1] = 10;
    }

    void Ray::successfulDraw(int x){
        if(x == 1){
            std::cout << "Successfully placed light ray" << std::endl;
            printRayStats();
        }
        else
            std::cout << "Light ray could not be placed" << std::endl;
    }

    void Ray::init(){
        Position = {250, 100};
        Tail = {500, 500};
        setSize(Position, Tail);
        Texture = ResourceManager::GetTexture("laser");
        Velocity = {0, 0};
        Move(0);
        Destroyed = false;
    }

    void Ray::clear(){
        Position = {0, 0};
        Tail = {0, 0};
        setSize(Position, Tail);
        Texture = ResourceManager::GetTexture("laser");
        Velocity = {0, 0};
        Move(0);
        Destroyed = true;
    }

    void Ray::setDirection(){
        /* This data will indicate if there is any difference in the y-coordinates */
        glm::vec2 Position2 = {Position[0] + Size[0], Position[1] + Size[1]},
        Tail2 = {Tail[0] + Size[0], Tail[1] + Size[1]};
        /* Angle Logic */
        if (Position[1] == Position2[1])
            Direction[0] = 0.0f; /* 0 degree with x axis */
        else if (Position2[0] == Tail2[0])
            Direction[1] = 0.0f; /* 0 degree with y axis */
        else{
            float xSlope = Position2[0] - Position[1];
            float ySlope = Position2[1] - Position[1];
            Direction[0] = (float)atan(ySlope/xSlope);
            Direction[1] = (float)atan(xSlope/ySlope);
        }
    }

    void Ray::incident(){
        /* Calcualtion for incident ray */
    }

    void Ray::refractive(){
        /* Calcualtion for refractive ray */
    }


