#include "../../incs/SDLMIXAudio.hpp"

void SDLMIXAudio::init() {}
void SDLMIXAudio::cleanup() {}

bool SDLMIXAudio::loadSound(const std::string &id, const std::string &filepath) {}
void SDLMIXAudio::unloadSound(const std::string &id) {}

void SDLMIXAudio::playSound(const std::string &id) {}
void SDLMIXAudio::playMusic(const std::string &id, bool loop = true) {}
void SDLMIXAudio::stopMusic() {}
void SDLMIXAudio::pauseMusic() {}
void SDLMIXAudio::resumeMusic() {}

void SDLMIXAudio::setSoundVolume(float volume) {}
void SDLMIXAudio::setMusicVolume(float volume) {}
void SDLMIXAudio::setMasterVolume(float volume) {}

bool SDLMIXAudio::isMusicPlaying() const {}