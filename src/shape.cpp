//Made by Griffen
#include "../include/simulationObject.hpp"

class Shape{
//----------Fields---------------
    private:
        unsigned int shapeWidth;
        unsigned int shapeHeight;
//----------Constructor----------
    public:
        explicit Shape(unsigned int width, unsigned int height):
        shapeWidth(width), shapeHeight(height){}
//----------Methods--------------
    public:
        unsigned int getHeight()   {
            return shapeHeight;
        }
        unsigned int getWidth()    {
            return shapeWidth;
        }
    
};