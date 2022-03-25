#include "../include/explosion.hpp"
#include "../include/factory.hpp"
#include <iostream>
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
float Explosion::rotation[]={-90,-45,0,45,90,135,180,225};

void Explosion::updateForcePositions(entt::registry *reg, float dt)    {
    auto view =reg->view<Forcewave>();
    for (auto entity: view) {
        //Changes position of the force vectors
        reg->patch<Renderable>(entity, [dt, reg, entity](auto &renderable) {
            renderable.yPos += (dt* reg->get<Forcewave>(entity).yVel);
            renderable.xPos += (dt* reg->get<Forcewave>(entity).xVel);
        });
        //Decrease x and y velocities over time
        reg->patch<Forcewave>(entity, [dt, reg, entity](auto &force) {
            force.xVel -= (0.7 * dt * force.xVel);
            force.yVel -= (0.7 * dt * force.yVel);
        });

    }
}
//TODO maybe program seg faults because it's not deleting properly? Maybe set deleted flag?
void Explosion::updateTimeActive(entt::registry *reg, float dt)  {
    auto view =reg->view<Forcewave>();
    for(auto entity: view)  {
        if(reg->get<Forcewave>(entity).timeActive >= MAX_TIME){
            //std::cout<<"destroying entity"<<std::endl;
            reg->destroy(entity);
        }
        else    reg->patch<Forcewave>(entity, [dt, reg, entity](auto &force){
                    force.timeActive+=dt;
                });
    }
}

