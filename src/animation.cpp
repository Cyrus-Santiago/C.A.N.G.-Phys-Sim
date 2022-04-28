#include "../include/animation.hpp"
#include "../include/factory.hpp"

/*
*Arguments: entity registry, delta time frame
*Returns:   N/A
*Purpose:   Updates appropriate values in a loop through each animated entity
*/
void Animation::animationUpdate(entt::registry &reg, float dt){
    auto view=reg.view<Animated>();
    for( auto entt : view){
        //Don't update the size of a force vector lol
        if(!reg.all_of<Forcewave>(entt)){
            resizeAnimation(reg,entt,dt);
        }
        updateTimeActive(reg,entt,dt);
    }
}

/*
*Arguments: entity registry, delta time frame
*Returns:   N/A
*Purpose:   updates the active time on things that have a time limit before they go bye bye */
void Animation::updateTimeActive(entt::registry &reg, entt::entity entt, float dt)  {
    //Get all animations and update the time on them.
    auto enttA=reg.get<Animated>(entt);
    if(enttA.timeActive >= enttA.maxTime) reg.destroy(entt);
    else   reg.patch<Animated>(entt, [dt](auto &animation){
            animation.timeActive+=dt;
    });
}
/*
*Arguments: entity registry, animated entity, delta time frame
*Returns:   N/A
*Purpose:   Changes the size of an animation by the same amount in the
*           x and y directions. The center of the animation remains the same.
*           Animations with a dR value of 0 are not resized.*/
void Animation::resizeAnimation(entt::registry &reg, entt:: entity entt,float dt)  {
    auto enttA = reg.get<Animated>(entt);
    float rateOfChange=enttA.dR * dt;
    reg.patch<Renderable>(entt, [dt,rateOfChange](auto &renderable){
        renderable.xPos-=(rateOfChange );      renderable.xSize+=rateOfChange *2;
        renderable.yPos-=(rateOfChange );      renderable.ySize+=rateOfChange*2;
    });
}