#include "../include/physCalc.hpp"
#include <assert.h>

float PhysCalc::calcKineticEnergy(float m1, float m2, float u1, float u2){
    float preKE = 0.5*m1*pow(u1,2.0)+0.5*m2*pow(u2,2.0);
    float v1 = getVelAfterCol(m1,m2,u1,u2,0);
    float v2 = getVelAfterCol(m1,m2,u1,u2,1);
    float postKE = 0.5*m1*pow(v1,2.0)+0.5*m2*pow(v2,2.0);
    assert(preKE==postKE);
    return postKE;
}

float PhysCalc::calcMomentum(float m1, float m2, float u1, float u2){
    float preMom = m1*u1+m2*u2;
    float v1 = getVelAfterCol(m1,m2,u1,u2,0);
    float v2 = getVelAfterCol(m1,m2,u1,u2,1);
    float postMom = m1*v1+m2*v2;
    assert(preMom==postMom);
    return postMom;
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

float PhysCalc::getVelAfterCol(float m1, float m2, float u1, float u2, bool velNum){
    float COR = 0.0;
    if(!velNum)
        return (((m1-m2)/(m1+m2))*u1 + ((2*m2)/(m1+m2))*u2);
    else    //Calculates velocity of second object after collision
        return (((2*m2)/(m1+m2))*u1 + ((m1-m2)/(m1+m2))*u2);
}