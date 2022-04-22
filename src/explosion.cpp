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
float Explosion::deltaX=0;
float Explosion::deltaY=0;
//This function updates the position of the force vectors. 
//The velocity of the vectors is decreased based on the duration.
void Explosion::updateForcePositions(entt::registry *reg, float dt)    {
    auto view =reg->view<Forcewave>();
    for (auto entity: view) {
        Explosion::deltaX=(dt* reg->get<Forcewave>(entity).xVel);
        Explosion::deltaY=(dt* reg->get<Forcewave>(entity).yVel);    
        //Changes position of the force vectors
        reg->patch<Renderable>(entity, [reg,entity](auto &renderable) {
            renderable.xPos += deltaX;
            renderable.yPos += deltaY;
        });
        //Changes position of force vector vertices
        reg->patch<Triangle>(entity, [reg, entity](auto &triangle) {
            //functional operator "map" to update each point position
            std::transform(triangle.points.begin(), triangle.points.end(), triangle.points.begin(),[](glm::vec2 point){
                point.x+=deltaX;
                point.y+=deltaY;
                return(point);
            });
        });
        //Decrease x and y velocities over time
        reg->patch<Forcewave>(entity, [dt, reg, entity](auto &force) {
            force.xVel -= (0.7 * dt * force.xVel);
            force.yVel -= (0.7 * dt * force.yVel);
        });
    }
}

