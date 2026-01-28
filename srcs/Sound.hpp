#pragma once
#include <SFML/Audio.hpp>

enum class SoundType
{
    EAT,
    GAME_OVER,
    MOVE
};

class Sound
{
public:
    static void init();
    static void makeSound(SoundType);

private:
    static sf::SoundBuffer _eatBuffer;
    static sf::SoundBuffer _gameOverBuffer;
    static sf::SoundBuffer _moveBuffer;

    static sf::Sound _eatSound;
    static sf::Sound _gameOverSound;
    static sf::Sound _moveSound;
};
