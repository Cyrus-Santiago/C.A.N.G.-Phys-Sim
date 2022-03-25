
#include <memory>
#include <cmath>
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include "../include/factory.hpp"
#include "../include/button.hpp"
#include "../include/textRenderer.hpp"
#include "../include/program.hpp"
#include "../include/input.hpp"
#include "../include/physCalc.hpp"

PhysCalc physCalc;
Button buttonT;

// Black-Box acceptance test
BOOST_AUTO_TEST_CASE( test1 ) {
    Factory factoryTest;
    entt::registry * regTest = new entt::registry;
    BOOST_CHECK(factoryTest.makeParticle(* regTest) != entt::null);
    BOOST_CHECK(regTest->valid(factoryTest.makeParticle(* regTest)));
    delete regTest;
}

// Black-Box acceptance test
BOOST_AUTO_TEST_CASE( test2 ) {
    // should be true
    BOOST_CHECK(physCalc.calcKineticEnergy(5, 5, 5, 5) == true);
    // should be false
    BOOST_CHECK(physCalc.calcKineticEnergy(10, 1, 3, 1) == false);
}

// Black-Box acceptance test
BOOST_AUTO_TEST_CASE( test3 ) {
    // should be true
    BOOST_CHECK(lround(physCalc.getVelFromMomentum(1.75, 2.222, 7.8, 4.76, true, true) - 4.39875) == 0);
    // should be false
    BOOST_CHECK(lround(physCalc.getVelFromMomentum(9.12, 3.535, 8.54, 5.22, true, true) - 5.78427) != 0);
}

// Black-Box acceptance test
BOOST_AUTO_TEST_CASE( test4 ) {
    // should be true
    BOOST_CHECK(lround(physCalc.calcCOR1D(3.98, 2.23, 3.65, 4.86) - 7.9697) == 0);
    // should be false
    BOOST_CHECK(lround(physCalc.calcCOR1D(5.32, 6.32, 9.45, 7.23) - 32.222) != 0);
}

// Black-Box acceptance test
BOOST_AUTO_TEST_CASE( test5 ) {
    // should be true
    BOOST_CHECK(lround(physCalc.calcCORBounce(3.98, 2.23) - 0.560301) == 0);
    // should be false
    BOOST_CHECK(lround(physCalc.calcCORBounce(5.32, 6.32) - 12.432) != 0);
}

// Black-Box acceptance test
BOOST_AUTO_TEST_CASE( test6 ) {
    // should be true
    BOOST_CHECK(lround(physCalc.calcCORDrop(5.44, 7.45) - 0.854518) == 0);
    // should be false
    BOOST_CHECK(lround(physCalc.calcCORDrop(43.87, 9.45) - 0.231234) != 0);
}

// Black-Box acceptance test
BOOST_AUTO_TEST_CASE( test7 ) {
    // should be true
    BOOST_CHECK(lround(physCalc.calcCORDrop(5.44, 7.45) - 0.854518) == 0);
    // should be false
    BOOST_CHECK(lround(physCalc.calcCORDrop(43.87, 9.45) - 0.231234) != 0);
}

// Black-Box acceptance test
BOOST_AUTO_TEST_CASE( test8 ) {
    // should be true
    BOOST_CHECK(lround(physCalc.getVelElastic1D(3.98, 2.23, 3.65, 4.86, true) - 3.30902) == 0);
    // should be false
    BOOST_CHECK(lround(physCalc.getVelElastic1D(5.32, 6.32, 9.45, 7.23, true) - 32.222) != 0);
}

// Black-Box acceptance test
BOOST_AUTO_TEST_CASE( test9 ) {
    // should be true
    BOOST_CHECK(lround(physCalc.getVelElastic2DOne(1.75, 2.222, 7.8, 4.76, true) - 4.74296) == 0);
    // should be false
    BOOST_CHECK(lround(physCalc.getVelElastic2DOne(49.12, 13.535, 38.54, 5.22, true) - 5.78427) != 0);
}

// Black-Box acceptance test
BOOST_AUTO_TEST_CASE( test10 ) {
    // should be true
    BOOST_CHECK(lround(physCalc.calcAngle(3.98, 2.23) - 0.510718) == 0);
    // should be false
    BOOST_CHECK(lround(physCalc.calcAngle(5.32, 6.32) - 12.432) != 0);
}

// White-Box test, branch coverage
// Coverage: 100%
BOOST_AUTO_TEST_CASE( test11 ) {
    TextRenderer::Init();
    std::string newSentence = "Hello";
    // first and second branch not taken
    TextRenderer::NewSentence(newSentence, glm::vec2(0, 0), 10);
    BOOST_CHECK(TextRenderer::Sentences.size() == 1);
    // first branch taken, second branch taken
    TextRenderer::NewSentence(newSentence, glm::vec2(10, 10), 10);
    BOOST_CHECK(TextRenderer::Sentences.size() == 2);
}

// This is more big bang style than anything, we weren't able to get anything
// drawing with the test suite though. So testing is very difficult since we're
// flying blind. But both of these features are mostly fleshed out and
// operational.
//
// this is a test of the menu/buttons classes and the input class meant to see if
// a click of the button is being registered.
BOOST_AUTO_TEST_CASE( test12 ) {
    buttonT = Button(glm::vec2(0, 0), glm::vec2(100, 100), "nothing");
    Menu::Buttons.push_back(buttonT);
    Input::getButtonData(Menu::Buttons);
    Input::determineAreaPressed(0, 0);
    Input::giveButtonData();
    BOOST_CHECK(Menu::Buttons[0].Pressed == true);
}