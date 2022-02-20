/* Project: C.A.N.G. Physics Simulator 2022
   Author: Amethyst Skye
   Class Description: Handles all operations performed on light ray(s) within simulation environment.
 */

#include <iostream>
#include <cmath>

class Ray{

    /* We can construct a ray based upon coordinates of mouse click */
    public: float rayOrigin[3];
    /* Flag which will allow us to remove ray when set to 0 */
    public: bool isActive = 1;

    /* Ray Constructor */
    Ray(float x, float y, float z){
        rayOrigin[0] = x;
        rayOrigin[1] = y;
        rayOrigin[2] = z;
    }

    /* This method will print the contents of the rayOrigin array */
    void printRayCoords(){
        std::cout << "Ray[0] = " << rayOrigin[0] << std::endl;
        std::cout << "Ray[1] = " << rayOrigin[1] << std::endl;
        std::cout << "Ray[2] = " << rayOrigin[2] << std::endl;
    }

};

    int main(){

        Ray ray = {1.8, 2.3, 3.5};
        ray.printRayCoords();

        std::cout << "Ray is " << ray.isActive << std::endl;

        return 0;
    }
