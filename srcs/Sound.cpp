#include "Sound.hpp"
#include <iostream>


sf::SoundBuffer Sound::_eatBuffer;
sf::SoundBuffer Sound::_gameOverBuffer;
sf::SoundBuffer Sound::_moveBuffer;

sf::Sound Sound::_eatSound(Sound::_eatBuffer);
sf::Sound Sound::_gameOverSound(Sound::_gameOverBuffer);
sf::Sound Sound::_moveSound(Sound::_moveBuffer);

void Sound::init()
{
    if (!_eatBuffer.loadFromFile("docs/bark.wav"))
        std::cerr << "Failed to load eat.wav\n";

    if (!_gameOverBuffer.loadFromFile("docs/GO.wav"))
        std::cerr << "Failed to load gameover.wav\n";

    if (!_moveBuffer.loadFromFile("docs/move.wav"))
        std::cerr << "Failed to load move.wav\n";
}

void Sound::makeSound(SoundType type)
{
    switch (type)
    {
        case SoundType::EAT:
            _eatSound.play();
            break;
        case SoundType::GAME_OVER:
            _gameOverSound.play();
            break;
        case SoundType::MOVE:
            _moveSound.play();
            break;
    }
}
