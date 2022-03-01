#include "../include/physCalc.hpp"
#include <assert.h>

bool PhysCalc::calcKineticEnergy(float m1, float m2, float u1, float u2){
    float preKE = 0.5*m1*pow(u1,2.0)+0.5*m2*pow(u2,2.0);
    float v1 = getVelElastic1D(m1,m2,u1,u2,0);
    float v2 = getVelElastic1D(m1,m2,u1,u2,1);
    float postKE = 0.5*m1*pow(v1,2.0)+0.5*m2*pow(v2,2.0);
    if(preKE==postKE) //If this is true, collision is Elastic
        return true;
    else
        return false;
}

float PhysCalc::getVelFromMomentum(float m1, float m2, float u1, float u2, bool velNum, bool elastic){
    float preMom = m1*u1+m2*u2;
    float v1=0.00, v2=0.00;
    switch(elastic){
        case true: //Elastic
            v1 = ((m1-m2)/(m1+m2))*u1+((2*m2)/(m1+m2))*u2;
            v2 = ((m2-m1)/(m1+m2))*u2+((2*m1)/(m1+m2))*u1;
            break;
        case false: //Inelastic
            v1 = (m1/(m1+m2))*u1;
            v2 = v1;
            break;
    }
    float postMom = m1*v1+m2*v2;
    assert(preMom==postMom); //This should always work, momentum doesn't dissapate
    if(elastic) //If it's elastic, both velocities are the same
        return v1;
    else if(!velNum) //0 for first object velocity
        return v1;
    else    //1 for second object
        return v2;
}

float PhysCalc::calcCOR1D(float u1, float v1, float u2, float v2){
    return abs(v1-v2)/abs(u1-u2);
}

float PhysCalc::calcCORBounce(float u, float v){
    return v/u;
}

float PhysCalc::calcCORDrop(float bounceHeight, float dropHeight){
    return sqrt(bounceHeight/dropHeight);
}

float PhysCalc::getVelElastic1D(float m1, float m2, float u1, float u2, bool velNum){
    if(!velNum) //Calculates velocity of first object after collision
        return (((m1-m2)/(m1+m2))*u1 + ((2*m2)/(m1+m2))*u2);
    else    //Calculates velocity of second object after collision
        return (((2*m1)/(m1+m2))*u1 + ((m2-m1)/(m1+m2))*u2);
}

float PhysCalc::getVelElastic2DOne(float m1, float m2, float v1, float angle, bool velNum){
    float angle1 = atan((m2*sin(angle))/(m1+m2*cos(angle))); //Relative angles of deflection
    float angle2 = (pi-angle)/2;
    if(!velNum)
        return v1*(sqrt(pow(m1,2.0)+pow(m2,2.0)+2*m1*m2*cos(angle))/(m1+m2));
    else
        return v1*(2*m1)/(m1+m2)*sin(angle/2);
}

float PhysCalc::getVelElastic2DTwo(float m1, float m2, float v1, float v2, float angle1, float angle2, bool velNum, bool XorY){
    float v1x = v1*cos(angle1), v1y = v1*sin(angle1);
    float v2x = v2*cos(angle2), v2y = v2*sin(angle2);
    float newVel = 0.00;
    float cAngle = atan((v1x*v2x)/(v1y*v2y)); //Gotta double check the math on this one
    switch(velNum){
        case false: //First Object Velocity
            if(!XorY) //Solve for X Component
                return ((v1*cos(angle1-cAngle)*(m1-m2)+2*m2*v2*cos(angle2-cAngle))/(m1+m2))
                    *cos(cAngle)+v1*sin(angle1-cAngle)*cos(cAngle+(pi/2));
            else //Solve for Y Component
                return ((v1*cos(angle1-cAngle)*(m1-m2)+2*m2*v2*cos(angle2-cAngle))/(m1+m2))
                    *sin(cAngle)+v1*sin(angle1-cAngle)*sin(cAngle+(pi/2));
        case true:
             if(!XorY) //X Component
                return ((v1*cos(angle1-cAngle)*(m1-m2)+2*m2*v2*cos(angle2-cAngle))/(m1+m2))
                    *cos(cAngle)+v1*sin(angle1-cAngle)*cos(cAngle+(pi/2));
            else //Y Component
                return ((v1*cos(angle1-cAngle)*(m1-m2)+2*m2*v2*cos(angle2-cAngle))/(m1+m2))
                    *sin(cAngle)+v1*sin(angle1-cAngle)*sin(cAngle+(pi/2));
    }
}


float PhysCalc::getVelInelastic(float m1, float m2, float u1, float u2, bool velNum, int dimensions){
    float v1=getVelFromMomentum(m1,m2,u1,u2,velNum,false); 
    float v2=getVelFromMomentum(m1,m2,u1,u2,velNum,false);
    float COR=0.00, vA=0.00, vB=0.00, Jn = 0.00;
    switch(dimensions){
        case '1':
            COR = calcCOR1D(u1,v1,u2,v2);
            vA = (COR*(m2*u2-m2*u1)+m1*u1+m2*u2)/(m1+m2);
            vB = (COR*(m1*u2-m1*u1)+m1*u1+m2*u2)/(m1+m2);
            break;
        case '2':
            //TO-DO: Add 2D Inelastic Collisions - Cyrus
            break;
    }
    if(!velNum)
        return vA;
    else
        return vB;
}

float PhysCalc::calcAngle(float xVel, float yVel){
    return atan(yVel/xVel);
}