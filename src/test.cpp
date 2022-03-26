//  This test framework uses http://www.boost.org/libs/test

#include <boost/test/tools/old/interface.hpp>
#include <vector>
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include "../include/input.hpp"
#include "../include/factory.hpp"
#include "../include/explosion.hpp"
#include "../include/audio.hpp"
#include "../include/physCalc.hpp"
#include "../include/collision.hpp"
Factory factorie;
entt::registry * regg;
Audio awDO;
Collision col;
PhysCalc phys;
int fakeButtonsxSize=50;
int fakeButtonsySize=20;
int fakeButtonsx[]={10,70,140,210,280,350};
int fakeButtonsy[]={450,480,510,540,570,590};

int pseudoDetermineAreaPressed(float xPos, float yPos, int screenHeight, int screenWidth){
//If the click was in the menu area
    if(yPos > screenHeight * 0.5){
        for (int i = 0; i < 6; ++i) {
            int upperBoundX=fakeButtonsx[i] + fakeButtonsxSize;
            int upperBoundY=fakeButtonsy[i] + fakeButtonsySize;
            // checks bounds of current button and compares that to click data
            if ((xPos > fakeButtonsx[i]) && (xPos < upperBoundX) &&
                (yPos > fakeButtonsy[i]) && (yPos < upperBoundY)) {
                // records click
                //xClick = xPos; yClick = yPos;
                //Returns 0 when a legal mouse click was made
                return 0;
            }
        }
    }
    //If the click is in the play area
    else if((xPos < screenWidth*0.95) && (xPos > screenWidth*0.0425) &&
        (yPos < screenHeight*0.45) && (yPos > screenHeight * 0.05)) {
        //std::cout<<"in play area"<<std::endl;
        //xClick = xPos; yClick = yPos;
        return 1;  
    }
    //std::cout<<"out of bounds"<<std::endl;
    return -1;
}

//Black Box Acceptance test
BOOST_AUTO_TEST_CASE( test1_getButtons ){
    std::vector<Button> Buttons= Menu::Buttons;
    Buttons=Input::giveButtonData();
    BOOST_CHECK_GT(Buttons.size(),0);
}
//Integration, top down Acceptance test
BOOST_AUTO_TEST_CASE( test2_updateExplosionVelocity ){
    regg = new entt::registry();
    factorie.makeForceVector(*regg);
    auto view = regg->view<Renderable>();
    float oldVel;
    
    for(auto entity : view){
        regg->patch<Forcewave>(entity, [entity](auto &force) {
            force.xVel =100;
        });
        oldVel=regg->get<Forcewave>(entity).xVel;
    }
    for(auto entity : view){
        Explosion::updateForcePositions(regg, 0.5);
        BOOST_CHECK_GT(oldVel,regg->get<Forcewave>(entity).xVel);
        oldVel=regg->get<Forcewave>(entity).xVel;
        Explosion::updateForcePositions(regg, 0.01);
        BOOST_CHECK_GT(oldVel,regg->get<Forcewave>(entity).xVel);
        oldVel=regg->get<Forcewave>(entity).xVel;
        Explosion::updateForcePositions(regg, 1);
        BOOST_CHECK_GT(oldVel,regg->get<Forcewave>(entity).xVel);
        oldVel=regg->get<Forcewave>(entity).xVel;
        Explosion::updateForcePositions(regg, 0);
        BOOST_CHECK_EQUAL(oldVel,regg->get<Forcewave>(entity).xVel);
        BOOST_CHECK_LT(regg->get<Forcewave>(entity).xVel,100);
    }
    delete regg;
}
//Integration, top down Acceptance test
BOOST_AUTO_TEST_CASE( test3_incrementTimeActive){
    regg = new entt::registry();
    factorie.makeForceVector(*regg);
    auto view = regg->view<Renderable>();
    float oldTime;
    for(auto entity : view){
        Explosion::updateTimeActive(regg, 0);
        BOOST_CHECK_EQUAL(0,regg->get<Forcewave>(entity).timeActive);
        oldTime=regg->get<Forcewave>(entity).timeActive;
        Explosion::updateTimeActive(regg, 0.5);
        BOOST_CHECK_LT(oldTime,regg->get<Forcewave>(entity).timeActive);
        oldTime=regg->get<Forcewave>(entity).timeActive;
        Explosion::updateTimeActive(regg, 1.0);
        BOOST_CHECK_GT(regg->get<Forcewave>(entity).timeActive,oldTime);
    }
    delete regg;
}
//White box the provides conditional coverage. The true function is supplied below.
//The test uses a condensed version. Button objects were not able to be created in this framework.
//The important pieces of the function are still carried over. "pseudoDetermineAreaPressed" may be seen above.
BOOST_AUTO_TEST_CASE(test4_simulateGettingMouseClicks){
    //button press
    BOOST_CHECK_EQUAL(0,pseudoDetermineAreaPressed(11, 460, 860, 1000));
    BOOST_CHECK_EQUAL(0,pseudoDetermineAreaPressed(399, 600, 860, 1000));
    BOOST_CHECK_EQUAL(0,pseudoDetermineAreaPressed(169, 520, 860, 1000));
    //play area
    BOOST_CHECK_EQUAL(1,pseudoDetermineAreaPressed(900, 45, 860, 1000));
    BOOST_CHECK_EQUAL(1,pseudoDetermineAreaPressed(80, 380, 860, 1000));
    BOOST_CHECK_EQUAL(1,pseudoDetermineAreaPressed(345, 162, 860, 1000));
    //Out of bounds
    BOOST_CHECK_EQUAL(-1,pseudoDetermineAreaPressed(0, 0, 860, 1000));
    BOOST_CHECK_EQUAL(-1,pseudoDetermineAreaPressed(1000, 860, 860, 1000));
    BOOST_CHECK_EQUAL(-1,pseudoDetermineAreaPressed(950, 400, 860, 1000));
}
//Integration, top down testing
BOOST_AUTO_TEST_CASE( test5_updateExplosionPositionsCorrectly){
    regg = new entt::registry();
    factorie.makeForceVector(*regg);
    auto view = regg->view<Renderable>();
    float oldPos;
    for(auto entity : view){
        oldPos=regg->get<Renderable>(entity).xPos;
        Explosion::updateForcePositions(regg, 0.5);
        BOOST_CHECK_LT(oldPos,regg->get<Renderable>(entity).xPos);
        oldPos=regg->get<Renderable>(entity).xPos;
        Explosion::updateForcePositions(regg, 0.01);
        BOOST_CHECK_LT(oldPos,regg->get<Renderable>(entity).xPos);
        oldPos=regg->get<Renderable>(entity).xPos;
        Explosion::updateForcePositions(regg, 1);
        BOOST_CHECK_LT(oldPos,regg->get<Renderable>(entity).xPos);
        oldPos=regg->get<Renderable>(entity).xPos;
        Explosion::updateForcePositions(regg, 0);
        BOOST_CHECK_EQUAL(oldPos,regg->get<Renderable>(entity).xPos);
        BOOST_CHECK_GT(regg->get<Renderable>(entity).xPos,100);
    }
    delete regg;
}
//White Box, conditonal coverage
BOOST_AUTO_TEST_CASE( test6_AdjustVolume){
    //audio engine not started
    BOOST_CHECK_EQUAL(-1,awDO.adjustVolume(true));
    BOOST_CHECK_EQUAL(-1,awDO.adjustVolume(false));
    //volume already at max
    BOOST_CHECK_EQUAL(1,awDO.adjustVolume(true));
    //turn volume down
    BOOST_CHECK_EQUAL(0,awDO.adjustVolume(false));
    //turn volume up
    BOOST_CHECK_EQUAL(0,awDO.adjustVolume(true));
    awDO.dropAudioEngine();
}
//White box, conditional coverage
BOOST_AUTO_TEST_CASE( test7_pauseVolume){
    //audio engine not started
    BOOST_CHECK_EQUAL(-1,awDO.pauseResumeAudio());
    //pause audio
    BOOST_CHECK_EQUAL(0,awDO.pauseResumeAudio());
    awDO.dropAudioEngine();

}
//Black box testing
BOOST_AUTO_TEST_CASE( test8_VerifyExplosionIsMade){
    regg = new entt::registry();
    //for some reason BOOST_CHECK_NE (not equal) wasn't working
    BOOST_CHECK(factorie.makeForceVector(*regg) != entt::null);
    delete regg;
}

//white box testing, conditional coverage
BOOST_AUTO_TEST_CASE( test9_VerifyDeletedVector){
    regg = new entt::registry();
    factorie.makeForceVector(*regg);
    auto view = regg->view<Renderable>();
    for(auto entity : view){
        BOOST_CHECK_EQUAL(regg->size(),1);
        Explosion::updateTimeActive(regg, 5.0);
        BOOST_CHECK_EQUAL(regg->size(),0);
        Explosion::updateTimeActive(regg, 5.0);
        BOOST_CHECK_EQUAL(regg->size(),0);
    }
    BOOST_CHECK_EQUAL(regg->size(),0);

}
//Back Box
BOOST_AUTO_TEST_CASE( test10_bounceHeight){
    BOOST_CHECK_NE(phys.calcBounceHeight(1000, -2), NAN);
    BOOST_CHECK_EQUAL(phys.calcBounceHeight(202, 0),phys.calcBounceHeight(202, 0));
    BOOST_CHECK_GT(phys.calcBounceHeight(100, 4),phys.calcBounceHeight(50, 4));
}
//integration, riskiest/hardest
BOOST_AUTO_TEST_CASE( test11_detectCollision){
    regg = new entt::registry();
    entt::entity exp1=factorie.makeForceVector(*regg);
    entt::entity exp2=factorie.makeForceVector(*regg);
    factorie.makeForceVector(*regg);
    auto view = regg->view<Renderable>();
    BOOST_CHECK_EQUAL(true,col.detector(exp1, exp2, *regg));
    delete regg;
}
//black box
BOOST_AUTO_TEST_CASE( test12_verify_values){
    regg = new entt::registry();
    entt::entity exp1=factorie.makeForceVector(*regg);
    BOOST_CHECK_EQUAL(regg->get<Forcewave>(exp1).xVel,100);
    BOOST_CHECK_EQUAL(regg->get<Forcewave>(exp1).yVel,100);
    BOOST_CHECK_EQUAL(regg->get<Forcewave>(exp1).timeActive,0);
    BOOST_CHECK_EQUAL(regg->get<Renderable>(exp1).yPos,1);
    BOOST_CHECK_EQUAL(regg->get<Renderable>(exp1).xPos,1);
    BOOST_CHECK_EQUAL(regg->get<Renderable>(exp1).type,"force");
}


//Functions used in white box

/*
//This function checks for the area in which a mouse click was made.
//If a button was clicked, its state is set to "pressed". 0,1 is returned
//a legal click is made. -1 is returned if no legal click is made.
int Input::determineAreaPressed(double xPos, double yPos)  {
  //If the click was in the menu area
  if(yPos > screenHeight * 0.5){
    for (int i = 0; i < Buttons.size(); ++i) {
      int upperBoundX=Buttons[i].Position.x + Buttons[i].Size.x;
      int upperBoundY=Buttons[i].Position.y + Buttons[i].Size.y;
      // checks bounds of current button and compares that to click data
      if ((xPos > Buttons[i].Position.x) && (xPos < upperBoundX) &&
        (yPos > Buttons[i].Position.y) && (yPos < upperBoundY)) {
        // records click
        xClick = xPos; yClick = yPos;
        // debug
        audio.playAudio("audio/click.wav");
        //std::cout << "Button " << i << " pressed!" << std::endl;
        // records button press so we can do something with it
        if (Buttons[i].Pressed) Buttons[i].Pressed = false;
        else{
          //functional operator "map" that changes all the values of "pressed" 
          //to false in the vector
          std::transform(Buttons.begin(), Buttons.end(),Buttons.begin(),[](Button button){
            button.Pressed=false;
            return(button);
          });
          Buttons[i].Pressed = true;
        }
        //Returns 0 when a legal mouse click was made
        return 0;
      }
    }
  }
  //If the click is in the play area
  else if((xPos < screenWidth*0.95) && (xPos > screenWidth*0.0425) &&
    (yPos < screenHeight*0.45) && (yPos > screenHeight * 0.05)) {
    //std::cout<<"in play area"<<std::endl;
    xClick = xPos; yClick = yPos;
    return 1;
  }
  //std::cout<<"out of bounds"<<std::endl;
  return -1;
}

//This function calculates how long a force vector from an explosion has been
// on the screen. After 3.5 seconds, the velocity vectors will be deleted.
void Explosion::updateTimeActive(entt::registry *reg, float dt)  {
    //Get all force vector entities
    auto view =reg->view<Forcewave>();
    for(auto entity: view)  {
        //If the force vector has been surpassed the maximum time, delete it.
        if(reg->get<Forcewave>(entity).timeActive >= MAX_TIME){
            //std::cout<<reg->get<Renderable>(entity).xPos<<" "<<reg->get<Renderable>(entity).yPos<<std::endl;
            reg->destroy(entity);
        }
        //Else, update the time it's active.
        else    reg->patch<Forcewave>(entity, [dt, reg, entity](auto &force){
                    force.timeActive+=dt;
                });
    }
}

//Adjusts the volume based on the pressing '-' or '+'. //TODO does not work yet
    //A "true" (+) passed increments the volume. "false" (-) decrements it.
    int Audio::adjustVolume(bool IncOrDec)   {
        if(engineStartup() == -1) return -1;
        //std::cout<<"yup"<<std::endl;
        ik_f32 volume=engine->getSoundVolume();
        //If true, increase volume
        if(IncOrDec)    {
            if(volume==1)   {
          //      std::cout<<"Volume already at maximum (1.0)"<<std::endl;
                return 1;
            }
            else    volume+=0.1f;
        }
        else    {
            if(volume==0)   {
            //    std::cout<<"Volume already at minimum (0.0)"<<std::endl;
                return 2;
            }    
            else    volume-=0.1f;
        }
        engine->setSoundVolume(volume);
        //std::cout<<"volume="<<engine->getSoundVolume()<<std::endl;
        engine->update();
        //std::cout<<"done"<<std::endl;
        return 0;
    }
    //Pauses all audio when the pressing 'p'. A boolean bit is flipped
    //after pausing or resuming the audio. //TODO does not work yet
    int Audio::pauseResumeAudio() {
        if(engineStartup() == -1) return -1;
        //If passed true, pause all sounds. If false, resume
        engine->setAllSoundsPaused(paused);
        paused=!paused; //flip paused bit
        //std::cout<<"done"<<std::endl;
        return 0;
    }

*/