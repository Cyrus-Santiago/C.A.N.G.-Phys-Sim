/* Project: Ray - C.A.N.G. Physics Simulator 2022
   Author: Amethyst Skye
   Class Description: Handles all operations performed on light ray(s) within simulation environment.
 */

#include <iostream>
#include <cmath>

class Ray{

    /* We can construct a ray based upon coordinates of mouse click.
       The user clicks where the origin will default until moved */
    public: float rayOrigin[3];
    /* End point of ray */
    public: float rayEnd[3];
    /* Flag which will allow us to remove ray when set to 0 */
    public: bool isActiveFlag = 1;

    /* Ray Constructor */
    Ray(float x, float y, float z){
        rayOrigin[0] = x;
        rayOrigin[1] = y;
        rayOrigin[2] = z;
    }

    /* Print the contents of the rayOrigin array */
    void printRayCoords(){
        std::cout <<"Origin: (" << rayOrigin[0] << ", " << rayOrigin[1] << ", " << rayOrigin[2] << ")" << std::endl;

        std::cout <<"End: (" << rayEnd[0] << ", " << rayEnd[1] << ", " << rayEnd[2] << ")" << std::endl;
    }

    /* Set position of the ray origin */
    void setOrigin(float x, float y, float z){
        rayOrigin[0] = x;
        rayOrigin[1] = y;
        rayOrigin[2] = z;
    }
    /* Set position of the ray end */
    void setEnd(float x, float y, float z){
        rayEnd[0] = x;
        rayEnd[1] = y;
        rayEnd[2] = z;
    }

    /* Delete ray from environment */
    void deleteRay(){
        isActiveFlag = 0;
        setOrigin(0,0,0);
        setEnd(0,0,0);
    }

    std::string isActive(){
        if(isActiveFlag == 1) return ("active");
        else return ("not active");
    }
};

    int main(){

        /* Testing */
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
    }
