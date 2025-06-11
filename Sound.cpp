#include "Sound.h"

Sound::Sound() {}

bool Sound::loadSounds() {
    if (!flipBuffer.loadFromFile("assets/sounds/flip.wav")) {
        std::cerr << "Failed to load flip sound!" << std::endl;
        return false;
    }
    flipSound.setBuffer(flipBuffer);

    if (!matchBuffer.loadFromFile("assets/sounds/match.wav")) {
        std::cerr << "Failed to load match sound!" << std::endl;
        return false;
    }
    matchSound.setBuffer(matchBuffer);

    if (!winBuffer.loadFromFile("assets/sounds/win.wav")) {
        std::cerr << "Failed to load win sound!" << std::endl;
        return false;
    }
    winSound.setBuffer(winBuffer);

    if (!loseBuffer.loadFromFile("assets/sounds/lose.wav")) {
        std::cerr << "Failed to load lose sound!" << std::endl;
        return false;
    }
    loseSound.setBuffer(loseBuffer);

    return true;
}

void Sound::playFlipSound() { flipSound.play(); }
void Sound::playMatchSound() { matchSound.play(); }
void Sound::playWinSound() { winSound.play(); }
void Sound::playLoseSound() { loseSound.play(); }
