#ifndef PHYSCALC_H
#define PHYSCALC_H

#include <math.h>

class PhysCalc{
public:
    //Kinetic Energy baby! Physics 1! Determines whether collision is elastic or not
    bool calcKineticEnergy(float m1, float m2, float u1, float u2);
    //Momentum Calculus used to get velocity cause momentum stays the same
    float getVelFromMomentum(float m1, float m2, float u1, float u2, bool velNum, bool elastic);

    //Gets the Velocity after an elastic collision in 1-Dimensional Space
    float getVelElastic1D(float m1, float m2, float u1, float u2, bool velNum);
    //Gets the Velocity after an elastic collision with one moving object in 2-Dimensional Space (No Gravity)
    float getVelElastic2DOne(float m1, float m2, float v1, float angle, bool velNum);
    //Gets the Velocity after an elastic collision with two moving objects in 2-Dimensional Space (No Gravity)
    float getVelElastic2DTwo(float m1, float m2, float v1, float v2, float angle1, float angle2, bool velNum, bool XorY);
    //Gets the Velocity after an inelastic collision
    float getVelInelastic(float m1, float m2, float u1, float u2, bool velNum, int dimensions);

    /*The Coefficient of Restitution determines the type of collision (Elastic or Inelastic), but
    since there are multiple ways to calculate COR based on situation, each using different variable 
    we need multiple functions for each of the 3 situations*/

    //One Dimensional Collisions between Two Objects
    float calcCOR1D(float u1, float v1, float u2, float v2);
    //Bouncing off a Stationary Object
    float calcCORBounce(float u, float v);
    //Dropped from rest onto Horizontal Surface (Spawning/Dropping onto Border)
    float calcCORDrop(float bounceHeight, float dropHeight);
    //Calculate the angle an object is moving
    float calcAngle(float xVel, float yVel);

private:
    float pi = 3.14;
    float theta=0.00;
    float CORThreshold=1.0f;
};

#endif