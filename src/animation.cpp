#include "../include/animation.hpp"
#include "../include/factory.hpp"

//This function 
/*
*Arguments: entity registry, delta time frame
*Returns:   N/A
*Purpose:   updates the active time on things that have a time limit before they go bye bye */
void Animation::updateTimeActive(entt::registry *reg, float dt)  {
    //Get all force vector entities
    auto viewForcewave =reg->view<Forcewave>();
    for(auto entt: viewForcewave)  {
        auto enttF=reg->get<Forcewave>(entt);
        //If the force vector has been surpassed the maximum time, delete it.
        if(enttF.timeActive >= enttF.maxTime)  reg->destroy(entt);
        //calculates how long a force vector from an explosion has been on the screen. 
        else    reg->patch<Forcewave>(entt, [dt](auto &force){
                force.timeActive+=dt;
            });
    }
    //Get all animations and update the time on them.
    auto viewAnimated =reg->view<Animated>();
    for(auto entt : viewAnimated){
        auto enttA=reg->get<Animated>(entt);
        if(enttA.timeActive >= enttA.maxTime) reg->destroy(entt);
        else   reg->patch<Animated>(entt, [dt](auto &animation){
                animation.timeActive+=dt;
        });
    }
}