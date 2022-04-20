/* Ray - Class Methods
   Written by Amethyst Skye
   Description: Handles all data for various operations performed on light ray(s) within simulation environment. */

#include "../include/ray.hpp"

/* Arguments: none
 * Returns:   N/A
 * Purpose:   Prints data associated with rays, used for debugging. */
    void Ray::printRayStats(){
        std::cout <<"Head: (" << Position[0] << ", " << Position[1] << ")" << std::endl;
        std::cout <<"Tail: (" << Tail[0] << ", " << Tail[1] << ")" << std::endl;
        std::cout <<"Dimensions: " << Dimensions[0] << "," << Dimensions[1] << std::endl;
        std::cout <<"Angle: " << Angle << std::endl;
    }

/* Arguments: x, y coordinates of mouse click
 * Returns:   N/A
 * Purpose:   Upon mouse click, this function will set the head of a ray to
 *            be oriented at that specified location. */
    void Ray::setPosition(double x, double y){
        Position[0] = (float)x;
        Position[1] = (float)y;
    }

/* Arguments: x, y coordinates
 * Returns:   N/A
 * Purpose:   This sets the tail of a ray to be sourced from the location
 *            of the x, y coordinate pair. */
    void Ray::setTail(double x, double y){
        Tail[0] = (float)x;
        Tail[1] = (float)y;
    }

/* Arguments: Ray position and tail vectors
 * Returns:   N/A
 * Purpose:   This function determines the dimensions of the ray based on the
 *            previously defined head and tail. This function is specifically 
 *            used for rays only. */
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

/* Arguments: Ray position and tail vectors
 * Returns:   N/A
 * Purpose:   This function determines the dimensions of the beam based on the
 *            previously defined head, tail, and beam width. This function is specifically 
 *            used for beams only. */
    void Ray::setBeamDimensions(glm::vec2 position, glm::vec2 tail, int beamWidth){
        Dimensions[0] = fabs(tail[0]-position[0]);
        Dimensions[1] = 10 * beamWidth;
        offsetFlag = false;

        /* This will allow ray to rotate properly. 
         * Otherwise, when the beam angle approaches 90 
         * degrees, it begins to disappear */
        if (Dimensions[0] < fabs(tail[1] - position[1])){
            Dimensions[0] = fabs(tail[1] - position[1]);
            offsetFlag = true;
        }
    }

/* Arguments: none
 * Returns:   N/A
 * Purpose:   Determines the offset needed for a ray upon placing in
 *            the play area. Since all rays originate from the same 
 *            coordinates, the ray may be angled and must be shifted
 *            accordingly to ensure the ray stays in the play area. */
    void Ray::determineOffset(){
        if (offsetFlag == true){
            Offset[0] = sin(Angle) * (Dimensions[0]/2);
        }
        else 
            Offset[0] = 0;
        Offset[1] = sin(Angle) * (Dimensions[0]/2);
    }

/* Arguments: none
 * Returns:   N/A
 * Purpose:   Determines the offset needed for a beam upon placing in
 *            the play area. Since all beams originate from the same 
 *            coordinates, the beam may be angled and must be shifted
 *            accordingly to ensure the beam stays in the play area.
 *            This function also accounts for the beam width. */
    void Ray::determineBeamOffset(){
        if (offsetFlag == true){
            Offset[0] = (sin(Angle) * (Dimensions[0]/2)) - (Dimensions[1]/2);
            Offset[1] = (sin(Angle) * (Dimensions[0]/2)) - 10;
        }
        else{
            Offset[0] = 0;
            Offset[1] = (sin(Angle) * (Dimensions[0]/2));
        }
    }

/* Arguments: Boolean
 * Returns:   N/A
 * Purpose:   A boolean indicates whether ray was successfully drawn or not.
 *            If true, a diagnostic of the ray data will be printed to stdout.
 *            Otherwise, an error statement will print. This is used for
 *            debugging purposes. */
    void Ray::successfulDraw(bool x){
        if(x == true){
            std::cout << "Successfully placed light ray" << std::endl;
            printRayStats();
        }
        else
            std::cout << "Light ray could not be placed" << std::endl;
    }

/* Arguments: x, y coordinates
 * Returns:   N/A
 * Purpose:   Assigns position to mouse click coordinates, and tail to 
 *            fixed location. This method will assign all neccessary 
 *            data to a ray so it can be drawn to play area. */
    void Ray::init(double xPos, double yPos){
        setPosition((float)xPos, (float)yPos); /* head is at click */
        setTail(805, 45); /* tail is always at top right corner */
        setDimensions(Position, Tail); /* tells us ray dimensions for drawing */
        setDirection(); /* set angle relative to head/tail position */
        determineOffset();
        Texture = ResourceManager::GetTexture("laser");
        Velocity = {0, 0}; /* we don't want the ray to move */
        Destroyed = false;
    }

/* Clear all ray data 
 * ~ Depreciated ~ */
    void Ray::clear(){
        Position = {0, 0};
        Tail = {0, 0};
        Dimensions = {0, 0};
        Velocity = {0, 0};
        Destroyed = true;
    }

/* Arguments: none
 * Returns:   N/A
 * Purpose:   This method determines the angle needed to orient a ray/beam 
 *            properly. */
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

/* Arguments: none
 * Returns:   Incident Ray
 * Purpose:   This method calculates data needed to determine incidence
 *            of a ray or beam. */
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

/* Arguments: none
 * Returns:   Refractive Ray
 * Purpose:   This method calculates data needed to determine refraction
 *            of a ray or beam. */
    Ray Ray::refractive(){
        Ray newRay = incident();
        if(Direction[1] == 0)
            newRay.Tail[1] *= (-1);
        else if (Direction[0] == 0)
            newRay.Tail[0] *= (-1);
        return(newRay);
    }

/* Calculations for ray rotation.
 * This number can be set in the Sprite Renderer class where we specify rotate.
 * ~ Depreciated ~ */
    float Ray::rotateLeft(){
        return(-0.25); /* rotate 22.5 degrees to the left */
    }
    float Ray::rotateRight(){
        return(0.25); /* rotate 22.5 degrees to the right */
    }
