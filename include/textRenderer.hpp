// This code was written by Nate

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "../include/character.hpp"

#include <glm/fwd.hpp>
#include <map>
#include <string>
#include <vector>

// this class facilitates rendering text. Well actually sprite renderer
// does that still... should've called it TextManager or something
class TextRenderer {
public:

    // stores information about bitmap character coordinates
    struct CharacterPosition {
        float x, y;
    };

    // stores information about a sentence
    struct CharactersArray {
        std::vector<Character> Characters;
        glm::vec2 position;
        unsigned int fontSize;
        float time;
    };

    // this array is for storing bitmap coordinates
    static std::vector<Character> CharPosData;

    // this array is for storing words we want to draw
    static std::map<std::string, CharactersArray> Sentences;
    
    // an array of structures that store information about bitmap coordinates
    static CharacterPosition CharPositions[27];

    // constructor
    TextRenderer() { }

    // initial setup (mainly adding bitmap data to the array)
    static void Init();
    
    // creates a new sentence so we can draw it later, this saves us from having
    // to build the same sentences repeatedly
    static void NewSentence(SpriteRenderer &renderer, std::string sentence,
              glm::vec2 position, unsigned int fontSize);

    // updates delta frame for fading in/out effects
    static void Update(float dt);

    // draws text from memory, can't draw text that hasn't been created with
    // new sentence
    static void Draw(SpriteRenderer &renderer, std::string name,
        glm::vec4 color);

    // turns down opacity on text for a fade out, then stops rendering it
    static void Hide(SpriteRenderer &renderer, std::string name);

    static float dt;
};

#endif