//  This test framework uses http://www.boost.org/libs/test
/*
//This should be put in the src directory with the main file being commented out so that this can run.
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

/*

/* Amethyst Testing */
/*
Factory testFactory;

BOOST_AUTO_TEST_CASE(MAKE_RAY_TEST){
    // This tests that the factory method will not return a null 'ray' 
    // We are using the factory method which uses the Ray
    // class to initilize position, tail, and size of the ray.
    // Should pass all
    entt::registry *testReg = new entt::registry();
    BOOST_CHECK(testFactory.makeRay(*testReg, {100, 100}) != entt::null);
    BOOST_CHECK(testReg->valid(testFactory.makeRay(*testReg, {100, 100})));
    delete testReg;
}

BOOST_AUTO_TEST_CASE(MAKE_BEAM_TEST){
    // This tests that the factory method will not return a null 'beam' 
    // We are using the factory method which uses the Beam
    // class to initilize position, tail, and size of the beam.
    // Should pass all
    entt::registry *testReg = new entt::registry();
    BOOST_CHECK(testFactory.makeBeam(*testReg, {100, 100}) != entt::null);
    BOOST_CHECK(testReg->valid(testFactory.makeBeam(*testReg, {100, 100})));
    delete testReg;
}

// White Box Test (full coverage)
BOOST_AUTO_TEST_CASE(BEAM_WIDTH_TEST){
    Beam testBeam;
    // This will ensure that the incBeamWidth() function works as it should
    BOOST_CHECK(testBeam.beamWidth == 0); //should fail
    BOOST_CHECK(testBeam.beamWidth == 1); //should pass
    testBeam.incBeamWidth();
    BOOST_CHECK(testBeam.beamWidth == 2); //should pass
    testBeam.incBeamWidth();
    BOOST_CHECK(testBeam.beamWidth == 3); //should pass
    testBeam.incBeamWidth();
    BOOST_CHECK(testBeam.beamWidth == 4); //should fail (cannot exceed width of 3)
}

BOOST_AUTO_TEST_CASE(Test2){
    Audio testAudio;
    // These will pass to ensure audio engine starts up
    // even if a valid file isn't passed to function 
    BOOST_CHECK(testAudio.playAudio("fakeSongTitle") != -1); // should fail
    BOOST_CHECK(testAudio.playAudio("audio/blast.wav") != -1); //should pass
}

BOOST_AUTO_TEST_CASE(Test4){
     // This tests that valid clicks are made in the window.
     // Function returns: 0 if legal mouse click, 1 if legal in
     // play area, and -1 if illegal mouse click 
    Input click;
    BOOST_CHECK(click.determineAreaPressed(0.3,0.3) == 1);
    BOOST_CHECK(click.determineAreaPressed(-0.3,0.3) == 1);
    BOOST_CHECK(click.determineAreaPressed(-0.3,0.3) == 0);
    BOOST_CHECK(click.determineAreaPressed(-0.3,0.3) == -1);
    BOOST_CHECK(click.determineAreaPressed(0.3,-0.3) == 1);
    BOOST_CHECK(click.determineAreaPressed(0.3,-0.3) == 0);
    BOOST_CHECK(click.determineAreaPressed(0.3,-0.3) == -1);
}

*/