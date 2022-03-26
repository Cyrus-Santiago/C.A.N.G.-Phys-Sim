#include "../include/explosion.hpp"
#include "../include/factory.hpp"

/*explosion physics notes
    -Total momentum is conserved
    -Total momentum is 0 (50 units forward, 50 backward)
    -Opposite 180 degree directions, equal in magnitude
        Principle: m1 * dv1 = -m2 * dv2
    -If system at rest initially and equal magnitudes, then equal velocities
    References:
    - https://www.physicsclassroom.com/class/momentum/Lesson-2/Momentum-Conservation-in-Explosions
    - https://www.youtube.com/watch?v=_5lWzzYZ6XM
    - https://www.youtube.com/watch?v=FfaEbqmvKQA
*/

//up,upright,right,downright,down,downleft,left,upleft
float Explosion::velocityArrayX[]={0,100,100,100,0,-100,-100,-100};
float Explosion::velocityArrayY[]={-100,-100,0,100,100,100,0,-100};
float Explosion::rotation[]={0,45,90,135,180,225,270,315};
Factory factorie;
//This function updates the position of the force vectors. 
//The velocity of the vectors is decreased based on the duration.
void Explosion::updateForcePositions(entt::registry *reg, float dt)    {
    auto view =reg->view<Forcewave>();
    for (auto entity: view) {
        //Changes position of the force vectors
        reg->patch<Renderable>(entity, [dt, reg, entity](auto &renderable) {
            renderable.yPos += (dt* reg->get<Forcewave>(entity).yVel);
            renderable.xPos += (dt* reg->get<Forcewave>(entity).xVel);
        });
        //Changes position of force vector vertices
        reg->patch<Triangle>(entity, [dt, reg, entity](auto &triangle) {
            //It's a little redunant to make these variables but it probably 
            //speeds up the program slightly to only do the retrieval calculation once.
            float deltaX=(dt* reg->get<Forcewave>(entity).xVel);
            float deltaY=(dt* reg->get<Forcewave>(entity).yVel);
            triangle.vPosLeft.x += deltaX;  triangle.vPosLeft.y += deltaY;
            triangle.vPosTop.x += deltaX;  triangle.vPosTop.y += deltaY;
            triangle.vPosRight.x += deltaX;  triangle.vPosRight.y += deltaY;
        });
        //Decrease x and y velocities over time
        reg->patch<Forcewave>(entity, [dt, reg, entity](auto &force) {
            force.xVel -= (0.7 * dt * force.xVel);
            force.yVel -= (0.7 * dt * force.yVel);
        });
    }
}
//This function calculates how long a force vector from an explosion has been
// on the screen. After 3.5 seconds, the velocity vectors will be deleted.
void Explosion::updateTimeActive(entt::registry *reg, float dt)  {
    //Get all force vector entities
    auto view =reg->view<Forcewave>();
    for(auto entity: view)  {
        //If the force vector has been surpassed the maximum time, delete it.
        if(reg->get<Forcewave>(entity).timeActive >= MAX_TIME){
            /*
            if(reg->get<Renderable>(entity).rotate == 45){
                factorie.makeForceVector(*reg, glm::vec2((int) reg->get<Renderable>(entity).xPos,
                    (int)reg->get<Renderable>(entity).yPos), reg->get<Renderable>(entity).rotate, glm::vec4(1,1,1,1), 
                    glm::vec2(0,0));
                std::cout<<reg->get<Renderable>(entity).rotate<<" x "<<reg->get<Triangle>(entity).vPosLeft.x<<" y "<<reg->get<Triangle>(entity).vPosLeft.y<<std::endl;
                std::cout<<reg->get<Renderable>(entity).rotate<<" x "<<reg->get<Triangle>(entity).vPosTop.x<<" y "<<reg->get<Triangle>(entity).vPosTop.y<<std::endl;
                std::cout<<reg->get<Renderable>(entity).rotate<<" x "<<reg->get<Triangle>(entity).vPosRight.x<<" y "<<reg->get<Triangle>(entity).vPosRight.y<<std::endl;
            }
            */
            reg->destroy(entity);
        }
        //Else, update the time it's active.
        else    reg->patch<Forcewave>(entity, [dt, reg, entity](auto &force){
                    force.timeActive+=dt;
                });
    }
}
//TODO make collision for triangle, gotta do lame calculations
