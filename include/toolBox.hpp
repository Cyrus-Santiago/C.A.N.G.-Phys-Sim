/*
Since each of these classes fulfill similar roles and control the logic of the toolbar
buttons, I've decided to put all of them in one file for simplicity sake, rather than creating
a seperate file for each similar class with similar methods. I can fix it later if it's a 
universally hated design choice. -Cyrus
*/

#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "entt.hpp"
#include "../include/input.hpp"
#include "../include/factory.hpp"

class Tools{
    public:
        static entt::entity getObject(entt::registry *reg, Click newMouseClick);
        
        /*Methods pertaining to the Move button functionality*/
        static void moveObject(entt::registry *reg, Click newMouseClick);
        void moveLoop(entt::registry *reg, float dt);

        /*Methods pertaining to the Resize button functionality*/
        void resizeObject(entt::registry *reg, Click newMouseClick);
        entt::entity* outlineObject(entt::registry *reg, glm::vec2 shapeDimensions, Click newMouseClick, std::string type);

        /*Methods pertaining to the Delete button functionality*/
        void deleteObject(entt::registry *reg, Click newMouseClick);
    
    private:
    Factory factory;
    entt::entity outline[4]; //Store outline entities for disposal after resizing is complete
};

#endif