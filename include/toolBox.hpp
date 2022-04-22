/*
Since each of these classes fulfill similar roles and control the logic of the toolbar
buttons, I've decided to put all of them in one file for simplicity sake, rather than creating
a seperate file for each similar class with similar methods. I can fix it later if it's a 
universally hated design choice. -Cyrus
*/

#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "entt.hpp"
#include <vector>
#include "../include/input.hpp"
#include "../include/factory.hpp"

class Tools{
    public:
        static entt::entity getObject(entt::registry *reg, Click newMouseClick);
        static bool checkBorder(entt::registry *reg, entt::entity clickedObject);
        
        /*Methods pertaining to the Move button functionality*/
        static void moveObject(entt::registry *reg, Click newMouseClick);
        void moveLoop(entt::registry *reg, float dt);

        /*Stasis button functionality*/
        void lockObject(entt::registry *reg, Click newMouseClick);

        /*Methods pertaining to the Resize button functionality*/
        void resizeObject(entt::registry *reg, Click newMouseClick);
        entt::entity* outlineObject(entt::registry *reg, Click newMouseClick, std::string type);
        void clearOutline(entt::registry *reg, entt::entity* outline);

        /*Methods pertaining to the Delete button functionality*/
        void deleteObject(entt::registry *reg, Click newMouseClick);

        /*Clear button functionality*/
        void clearAll(entt::registry *reg);
    
    private:
        Factory factory;
        entt::entity outline[4]; //Store outline entities for disposal after resizing is complete
};

#endif