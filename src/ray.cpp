/* Ray (Extends Simulation Object) - Class Methods
   Written by Amethyst Skye
   Description: Handles all data for various operations performed on light ray(s) within simulation environment. */

#include "../include/ray.hpp"

/* Print the contents of the rayOrigin array */
    void Ray::printRayStats(){
        std::cout <<"Head: (" << Position[0] << ", " << Position[1] << ")" << std::endl;
        std::cout <<"Tail: (" << Tail[0] << ", " << Tail[1] << ")" << std::endl;
        std::cout <<"Direction: " << Direction[0] << "," << Direction[1] << std::endl;
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
        Size[0] = (tail[0]-position[0]);
        Size[1] = 10;
    }

/* Was ray drawn successfully? */
    void Ray::successfulDraw(int x){
        if(x == 1){
            std::cout << "Successfully placed light ray" << std::endl;
            printRayStats();
        }
        else
            std::cout << "Light ray could not be placed" << std::endl;
    }

/* Initial Stats upon inserting a ray */
    void Ray::init(double xPos, double yPos){
        Position = {(float)xPos, (float)yPos}; /* head is at click */
        Tail = {420, 200}; /* tail is always at origin */
        setSize(Position, Tail);
        setDirection(); /* set angle relative to head/tail position */
        Texture = ResourceManager::GetTexture("laser");
        Velocity = {0, 0}; /* we don't want the ray to move */
        Destroyed = false;
    }

/* Clear all ray data */
    void Ray::clear(){
        Position = {0, 0};
        Tail = {0, 0};
        Size = {0, 0};
        Velocity = {0, 0};
        Destroyed = true;
    }

/* Angle Logic */
    void Ray::setDirection(){
        float posX2 = Position[0] + Size[1],
              posY2 = Position[1] + Size[1],
              tailX2 = Tail[0] + Size[1],
              tailY2 = Tail[1] + Size[1];
        glm::vec2 Position2 = {posX2, posY2},
                  Tail2 = {tailX2, tailY2};

        if (Position[1] == Tail[1])
            Direction[0] = 0.0f; /* 0 degree with x axis */
        else if (Position[0] == Position2[0])
            Direction[1] = 0.0f; /* 0 degree with y axis */
        else{
            float xSlope = Position[0] - Tail[0],
                  ySlope = Position[1] - Tail[0];
            Direction[0] = (float)atan(ySlope/xSlope) * (180/M_PI);
            Direction[1] = (float)atan(xSlope/ySlope) * (180/M_PI);
        }
    }

/* Physical calculation for incident ray */
    Ray Ray::incident(){
        Ray newRay(Tail); /* Position of previous tail is new Origin */
        /* For incidence on x-axis - i.e. no angle on y-axis */
        if (Direction[1] == 0){
            float theta1 = Direction[0],
                theta2 = 90 - theta1;
            newRay.Tail[0] = Position[0] + (Position[0] * sinf(theta1/theta2));
            newRay.Tail[1] = Position[1]; /* keep y-coord */
        }
        /* For incidence on y-axis - i.e. no angle on x-axis */
        else if(Direction[0] == 0){
            float theta3 = Direction[1],
                theta4 = 90 - theta3;
            newRay.Tail[0] = Position[0]; /* Keep x-coord */
            newRay.Tail[1] = Position[1] + (Position[1] * sinf(theta3/theta4));
        }
        return(newRay);
    }

/* Physical calcualtion for refractive ray */
    Ray Ray::refractive(){
        Ray newRay = incident();
        if(Direction[1] == 0)
            newRay.Tail[1] *= (-1);
        else if (Direction[0] == 0)
            newRay.Tail[0] *= (-1);
        return(newRay);
    }

/* Calculations for ray rotation  - this number can be set in the Sprite Renderer class where we specify rotate */
    float Ray::rotateLeft(){
        return(-0.25); /* rotate 22.5 degrees to the left */
    }
    float Ray::rotateRight(){
        return(0.25); /* rotate 22.5 degrees to the right */
    }
