#pragma once

#include <SFML/Audio.hpp>
#include <iostream>

class Sound {
public:
    Sound();

    bool loadSounds();
    void playFlipSound();
    void playMatchSound();
    void playWinSound();
    void playLoseSound();

private:
    sf::SoundBuffer flipBuffer;
    sf::SoundBuffer matchBuffer;
    sf::SoundBuffer winBuffer;
    sf::SoundBuffer loseBuffer;

    sf::Sound flipSound;
    sf::Sound matchSound;
    sf::Sound winSound;
    sf::Sound loseSound;
};
