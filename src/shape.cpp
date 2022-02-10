class Shape {
//----------Fields---------------
    private:
        float shapeLength;
        float shapeWidth;
//----------Constructor----------
    public:
        explicit Shape(float length, float width):
        shapeLength(length), shapeWidth(width){}
//----------Methods--------------
    public:
        float getLength()   {
            return shapeLength;
        }
        float getWidth()    {
            return shapeWidth;
        }
    
};