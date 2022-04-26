#include "../include/explosion.hpp"
#include "../include/factory.hpp"


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

