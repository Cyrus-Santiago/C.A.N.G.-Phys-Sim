// this code was written by Nate

#include "../include/game.hpp"
#include <cassert>
#include <glm/fwd.hpp>
#include <string>
#include <iostream>
#include <utility>
#include <vector>

    std::vector<Character> TextRenderer::CharPosData;

    // this array is for storing words we want to draw
    std::map<std::string, TextRenderer::CharactersArray> TextRenderer::Sentences;
    
    // an array of structures that store information about bitmap coordinates
    TextRenderer::CharacterPosition TextRenderer::CharPositions[27];

    float TextRenderer::dt;

void TextRenderer::Init() {
    
    // an array used to store specific coordinates for letters in the bitmap
    CharacterPosition charPositions[] = {
      {0.07f, 0.01f}, {1.17f, 0.01f}, {2.23f, 0.01f}, {3.37f, 0.01f},
      {4.47f, 0.01f}, {5.57f, 0.01f}, {6.63f, 0.01f}, {7.77f, 0.01f},
      {8.87f, 0.01f}, {0.07f, 1.05f}, {1.17f, 1.05f}, {2.27f, 1.05f},
      {3.37f, 1.05f}, {4.50f, 1.05f}, {5.57f, 1.05f}, {6.67f, 1.05f},
      {7.77f, 1.05f}, {8.87f, 1.05f}, {0.07f, 2.09f}, {1.17f, 2.09f},
      {2.305f,2.09f}, {3.37f, 2.09f}, {4.47f, 2.09f}, {5.57f, 2.09f},
      {6.67f, 2.09f}, {7.77f, 2.09f}, {8.87f, 2.09f}, {1.2f,  7.75f}
    };

    // loops through each letter and put it in a character object,
    // then adds it to the charPosData array so we can draw the letters
    // later on
    for (int i = 0; i < 26; i++) {
        CharPosData.push_back(Character('A' + i,
            glm::vec2(charPositions[i].x, charPositions[i].y)));
    }
    // adds space and period to the charPosData array
    CharPosData.push_back(
        Character(' ', glm::vec2(charPositions[26].x, charPositions[26].y)));
    CharPosData.push_back(
        Character('.', glm::vec2(charPositions[27].x, charPositions[27].y)));
}

void TextRenderer::NewSentence(std::string newSentence,
    glm::vec2 position, unsigned int fontSize) {
    // here we check to see if the sentence we're trying to add already exists,
    // that way we don't add it twice if we don't have to
    if (Sentences.find(newSentence) != Sentences.end()) {
        if (Sentences.at(newSentence).position != position) {
            newSentence += " ";
        } else {
            return;
        }
    }
        

    // declare an array of characters to build the sentence from bitmap
    // data
    std::vector<Character> Characters;

    // loop through every char in sentence
    for (char &letter : newSentence) {
        // loop through each character in position data array
        for (int i = 0; i < 27; ++i) {
            // if a letter matches data from the array then we add
            // it to the Characters array so we can draw it
            if (letter == CharPosData[i].character) {
                Characters.push_back(CharPosData[i]);
                break;
            }
        }
    }

    // now we declare our struct, storing some important information about
    // the text we render so we don't have to ask again
    CharactersArray charArray = {Characters, position, fontSize, 0.0f};

    // adds the struct to the sentences array, and maps it to a key that's
    // equal to the sentence stored in the text we're rendering
    Sentences.insert(std::pair<std::string,
        CharactersArray>(newSentence, charArray));
    
    // make sure the world isn't falling apart and the sentence we just
    // added exists
    assert(Sentences.find(newSentence) != Sentences.end());
}

void TextRenderer::Draw(SpriteRenderer &renderer, std::string name,
    glm::vec4 color) {
    // ensure the programmer doesn't try to render text that hasn't been loaded
    assert(Sentences.find(name) != Sentences.end());
    // retrieve the struct information from sentence array
    CharactersArray charArray = Sentences.at(name);
		Character characters[] = {
			Character('M', {3.37f, 1.05f}),
			Character('A', {0.07f, 0.01f}),
			Character('S', {0.07f, 2.09f}),
			Character('H', {7.77f, 0.01f})
		};
    int i = 0;
    // loop through every character
    for (Character &character : characters) {
        // draw every character (increasing horizontal position in sentence)
        color.w = charArray.time;
        character.Draw(renderer, i, charArray.position, charArray.fontSize, 
        color);
        i++;
    }
    // check to see if opacity is full, if it isn't then we keep incrementing
    if (charArray.time < 1.0f) Sentences.at(name).time += dt;
}

void TextRenderer::Hide(SpriteRenderer &renderer, std::string name) {

    // assert that the thing we're hiding actually exists
    assert(Sentences.find(name) != Sentences.end());

    // get the struct using the key on Sentences array
    CharactersArray charArray = Sentences.at(name);

    // we only want to draw if the opacity is above 0
    if (charArray.time > 0.0f) {
        // we update the time for a fadeout effect
        Sentences.at(name).time -= dt;
        
        // I'd like to have this function just call draw, but the fade out
        // effect doesn't seem to work when I do that, so here's the code above
        // again.
        int i = 0;
        for (Character &character : charArray.Characters) {
            character.Draw(renderer, i, charArray.position, charArray.fontSize, 
            glm::vec4(1.0f, 1.0f, 1.0f, charArray.time));
            i++;
        }
    }
}

void TextRenderer::Update(float newDt) {
    // keep the delta frame updated so we're aware of time for the purposes of
    // fading effects
    dt = newDt;
}
