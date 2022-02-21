/* Title: Ray - Class Methods
   Author: Amethyst Skye
   Description: Handles all operations performed on light ray(s) within simulation environment.
 */

#include "../include/ray.hpp"

    /* Print the contents of the rayOrigin array */
    void Ray::printRayCoords(){
        std::cout <<"Ray Origin: (" << rayOrigin[0] << ", " << rayOrigin[1] << ", " << rayOrigin[2] << ")" << std::endl;

        std::cout <<"End: (" << rayEnd[0] << ", " << rayEnd[1] << ", " << rayEnd[2] << ")" << std::endl;
    }

    /* Set position of the ray origin */
    void Ray::setOrigin(float x, float y, float z){

        rayOrigin[0] = x;
        rayOrigin[1] = y;
        rayOrigin[2] = z;
    }
    /* Set position of the ray end */
    void Ray::setEnd(float x, float y, float z){
        rayEnd[0] = x;
        rayEnd[1] = y;
        rayEnd[2] = z;
    }

    /* Delete ray from environment */
    void Ray::deleteRay(){
        isActiveFlag = 0;
        setOrigin(0,0,0);
        setEnd(0,0,0);
    }

    int drawRay(){
        return 0;
    }

    void Ray::placeRay(Click mouseClick){
        if(isActiveFlag == 1){
            setOrigin(mouseClick.xPos, mouseClick.yPos, 0);
            setEnd(0,0,0); /* change later - needs some math */
            printRayCoords();
            if(drawRay() == 1)
                std::cout << "Successfully placed light ray" << std::endl;
            else
                std::cout << "Light ray could not be placed" << std::endl;
        }
    }

    std::string Ray::isActive(){
        if(isActiveFlag == 1) return ("active");
        else return ("not active");
    }

    /*For Testing
    int main(){

        Ray ray1 = {1.8, 2.3, 3.5};
        Ray ray2 = {4.2, 5.1, 9.4};

        ray1.setEnd(3.3, 5.5, 6.6);
        ray2.setEnd(1.1, 9.9, 3.2);

        std::cout << "Ray 1 is " << ray1.isActive() << ", and coordinates are "<< std::endl;
        ray1.printRayCoords();
        ray2.deleteRay();

        std::cout << "Ray 2 is " << ray2.isActive() << ", and coordinates are "<< std::endl;
        ray2.printRayCoords();

        return 0;
    } */
