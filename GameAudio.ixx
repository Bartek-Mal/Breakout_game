module;

#include <SFML/Audio.hpp>
#include <iostream>

export module GameAudio;

namespace GameAudio {
    sf::Music menuMusic;
    sf::Music gameMusic;
    sf::SoundBuffer collisionBuffer;
    sf::SoundBuffer shrinkBuffer;
    sf::SoundBuffer resetSizeBuffer;
    sf::Sound collisionSound;
    sf::Sound shrinkSound;
    sf::Sound resetSizeSound;

    export void initialize() {
        if (!menuMusic.openFromFile("menu_music.mp3")) {
            std::cerr << "Failed to load menu music." << std::endl;
        }
        if (!gameMusic.openFromFile("game_music.mp3")) {
            std::cerr << "Failed to load game music." << std::endl;
        }
        if (!collisionBuffer.loadFromFile("collision_sound.wav")) {
            std::cerr << "Failed to load collision sound." << std::endl;
        }
        else {
            collisionSound.setBuffer(collisionBuffer);
        }
        if (!shrinkBuffer.loadFromFile("shrink_sound.wav")) {
            std::cerr << "Failed to load shrink sound." << std::endl;
        }
        else {
            shrinkSound.setBuffer(shrinkBuffer);
        }
        if (!resetSizeBuffer.loadFromFile("reset_size_sound.wav")) {
            std::cerr << "Failed to load reset size sound." << std::endl;
        }
        else {
            resetSizeSound.setBuffer(resetSizeBuffer);
        }
    }

    export void playMenuMusic() {
        gameMusic.stop(); 
        menuMusic.setLoop(true);
        menuMusic.play();
    }

    export void playGameMusic() {
        menuMusic.stop(); 
        gameMusic.setLoop(true);
        gameMusic.play();
    }

    export void playCollisionSound() {
        collisionSound.play();
    }

    export void playShrinkSound() {
        shrinkSound.play();
    }

    export void playResetSizeSound() {
        resetSizeSound.play();
    }

    export void stopAllMusic() {
        menuMusic.stop();
        gameMusic.stop();
    }
}
