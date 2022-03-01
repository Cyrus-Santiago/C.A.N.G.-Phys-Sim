#ifndef PHYSCALC_H
#define PHYSCALC_H

#include <math.h>

class PhysCalc{
public:
    //Kinetic Energy baby! Physics 1!
    float calcKineticEnergy(float m1, float m2, float u1, float u2);
    //Momentum Calculus
    float calcMomentum(float m1, float m2, float u1, float u2);
    //Gets the Velocity after a collision, uses COR to work for both Elastic and Inelatic Collisions
    float getVelAfterCol(float m1, float m2, float u1, float u2, bool velNum);

    /*The Coefficient of Restitution determines the type of collision (Elastic or Inelastic), but
    since there are multiple ways to calculate COR based on situation, each using different variable 
    we need multiple functions for each of the 3 situations*/

    //One Dimensional Collisions between Two Objects
    float calcCOR1D(float u1, float v1, float u2, float v2);
    //Bouncing off a Stationary Object
    float calcCORBounce(float u, float v);
    //Dropped from rest onto Horizontal Surface (Spawning/Dropping onto Border)
    float calcCORDrop(float bounceHeight, float dropHeight);

private:
    float theta=0.00;
    float CORThreshold=0.5f;
};

#endif