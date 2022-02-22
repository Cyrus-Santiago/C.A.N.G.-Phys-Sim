/* Ray - Class Methods
   Written by Amethyst Skye
   Description: Handles all operations performed on light ray(s) within simulation environment. */

#include "../include/ray.hpp"
#include <cmath>

    /* Print the contents of the rayOrigin array */
    void Ray::printRayCoords(){
        std::cout <<"Ray Origin: (" << Origin[0] << ", " << Origin[1] << ")" << std::endl;

        std::cout <<"Ray End: (" << End[0] << ", " << End[1] << ")" << std::endl;
    }

    /* Set position of the ray origin */
    glm::vec2 Ray::setOrigin(double x, double y){
        Origin[0] = (float)x;
        Origin[1] = (float)y;

        return (Origin);
    }
    /* Set position of the ray end */
    glm::vec2 Ray::setEnd(double x, double y){
        End[0] = (float)x;
        End[1] = (float)y;

        return (End);
    }

    /* Ray Dimensions (length, width) */
    glm::vec2 Ray::setSize(glm::vec2 origin, glm::vec2 end){
        Size[0] = fabsf(end[0]-origin[0]);
        Size[1] = 0.01;

        return (Size);
    }

    /* Delete ray from environment */
    void Ray::deleteRay(){
        isActive = 0;
        setOrigin(0,0);
        setEnd(0,0);
    }

    void Ray::successfulDraw(int x){
        if(x == 1){
            std::cout << "Successfully placed light ray" << std::endl;
            printRayCoords();
        }
        else
            std::cout << "Light ray could not be placed" << std::endl;
    }

    /* Used to draw the ray image */
    void Ray::drawRay(SpriteRenderer &renderer){
        rayTexture = ResourceManager::GetTexture("button1");
        Color = {0.9f, 0.9f, 0.1f};
        renderer.DrawSprite(rayTexture, Origin, Size, 0, Color, {0,0}, {0,0,});
        successfulDraw(1);
    }

    /* Handles placing light ray into sim environment */
    void Ray::placeRay(Click mouseClick){
        setOrigin(mouseClick.xPos, mouseClick.yPos);
        setEnd(0,0); /* change later - needs some math from play area bounds */
        isActive = 1;
    }
