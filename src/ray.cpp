/* Ray - Class Methods
   Written by Amethyst Skye
   Description: Handles all data for various operations performed on light ray(s) within simulation environment. */

#include "../include/ray.hpp"

/* Print the contents of the rayOrigin array */
    void Ray::printRayStats(){
        std::cout <<"Head: (" << Position[0] << ", " << Position[1] << ")" << std::endl;
        std::cout <<"Tail: (" << Tail[0] << ", " << Tail[1] << ")" << std::endl;
        std::cout <<"Dimensions: " << Dimensions[0] << "," << Dimensions[1] << std::endl;
        std::cout <<"Angle: " << Angle << std::endl;
    }

/* Set position of the ray origin */
    void Ray::setPosition(double x, double y){
        Position[0] = (float)x;
        Position[1] = (float)y;
    }

/* Set coordinates where the ray will end */
    void Ray::setTail(double x, double y){
        Tail[0] = (float)x;
        Tail[1] = (float)y;
    }

/* Ray Dimensions (length, width) */
    void Ray::setDimensions(glm::vec2 position, glm::vec2 tail){
        Dimensions[0] = fabs(tail[0]-position[0]);
        Dimensions[1] = 10; /* always keep rays 10 pixels wide */
        offsetFlag = false;

        /* This will allow ray to rotate properly. 
         * Otherwise, when the ray angle approaches 90 
         * degrees, it begins to disappear */
        if (Dimensions[0] < fabs(tail[1] - position[1])){
            Dimensions[0] = fabs(tail[1] - position[1]);
            offsetFlag = true;
        }
    }

    void Ray::determineOffset(){
        if (offsetFlag == true){
            Offset[0] = sin(Angle) * (Dimensions[0]/2);
        }
        else 
            Offset[0] = 0;
        Offset[1] = sin(Angle) * (Dimensions[0]/2);
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
        Tail = {805, 45}; /* tail is always at top right corner */
        setDimensions(Position, Tail); /* tells us ray dimensions for drawing */
        setDirection(); /* set angle relative to head/tail position */
        determineOffset();
        Texture = ResourceManager::GetTexture("laser");
        Velocity = {0, 0}; /* we don't want the ray to move */
        Destroyed = false;
    }

/* Clear all ray data */
    void Ray::clear(){
        Position = {0, 0};
        Tail = {0, 0};
        Dimensions = {0, 0};
        Velocity = {0, 0};
        Destroyed = true;
    }

/* Angle Logic */
    void Ray::setDirection(){
        float posX2 = Position[0] - Dimensions[1],
              posY2 = Position[1] - Dimensions[1],
              tailX2 = Tail[0] - Dimensions[1],
              tailY2 = Tail[1] - Dimensions[1];
        glm::vec2 Position2 = {posX2, posY2},
                  Tail2 = {tailX2, tailY2};

        if (Position[1] == Tail[1])
            Direction[0] = 0.0f; /* 0 degree with x axis */
        else if (Position[0] == Position2[0])
            Direction[1] = 0.0f; /* 0 degree with y axis */
        else{
            float xSlope = Position[0] - Tail[0],
                  ySlope = Position[1] - Tail[1];
            Direction[0] = (float)atan(ySlope/xSlope);
            Direction[1] = (float)atan(xSlope/ySlope);      
        }
        Angle = Direction[0];
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
