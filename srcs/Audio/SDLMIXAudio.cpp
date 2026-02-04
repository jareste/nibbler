#include "../../incs/SDLMIXAudio.hpp"
#include <iostream>

// Constructor
SDLMIXAudio::SDLMIXAudio() 
	: soundVolume(1.0f), musicVolume(1.0f), masterVolume(1.0f) {
	std::cout << "[SDLMIXAudio] Constructor called" << std::endl;
}

// Destructor
SDLMIXAudio::~SDLMIXAudio() {
	cleanup();
	std::cout << "[SDLMIXAudio] Destructor called" << std::endl;
}

// Initialization
void SDLMIXAudio::init() {
	std::cout << "[SDLMIXAudio] Init called - TODO: Initialize SDL_mixer" << std::endl;
	// TODO: Initialize SDL_mixer here
	// Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	// Mix_AllocateChannels(16);
}

void SDLMIXAudio::cleanup() {
	std::cout << "[SDLMIXAudio] Cleanup called - TODO: Cleanup SDL_mixer" << std::endl;
	// TODO: Cleanup SDL_mixer resources
	// Mix_CloseAudio();
}

// Sound management
bool SDLMIXAudio::loadSound(const std::string &id, const std::string &filepath) {
	std::cout << "[SDLMIXAudio] loadSound called - id: " << id << ", path: " << filepath << std::endl;
	// TODO: Load sound with Mix_LoadWAV
	return true;
}

void SDLMIXAudio::unloadSound(const std::string &id) {
	std::cout << "[SDLMIXAudio] unloadSound called - id: " << id << std::endl;
	// TODO: Free sound with Mix_FreeChunk
}

// Playback
void SDLMIXAudio::playSound(const std::string &id) {
	std::cout << "[SDLMIXAudio] playSound called - id: " << id << std::endl;
	// TODO: Play sound with Mix_PlayChannel
}

void SDLMIXAudio::playMusic(const std::string &id, bool loop) {
	std::cout << "[SDLMIXAudio] playMusic called - id: " << id << ", loop: " << loop << std::endl;
	// TODO: Play music with Mix_PlayMusic
}

void SDLMIXAudio::stopMusic() {
	std::cout << "[SDLMIXAudio] stopMusic called" << std::endl;
	// TODO: Stop music with Mix_HaltMusic
}

void SDLMIXAudio::pauseMusic() {
	std::cout << "[SDLMIXAudio] pauseMusic called" << std::endl;
	// TODO: Pause music with Mix_PauseMusic
}

void SDLMIXAudio::resumeMusic() {
	std::cout << "[SDLMIXAudio] resumeMusic called" << std::endl;
	// TODO: Resume music with Mix_ResumeMusic
}

// Volume control
void SDLMIXAudio::setSoundVolume(float volume) {
	soundVolume = volume;
	std::cout << "[SDLMIXAudio] setSoundVolume called - volume: " << volume << std::endl;
	// TODO: Set volume with Mix_Volume
}

void SDLMIXAudio::setMusicVolume(float volume) {
	musicVolume = volume;
	std::cout << "[SDLMIXAudio] setMusicVolume called - volume: " << volume << std::endl;
	// TODO: Set music volume with Mix_VolumeMusic
}

void SDLMIXAudio::setMasterVolume(float volume) {
	masterVolume = volume;
	std::cout << "[SDLMIXAudio] setMasterVolume called - volume: " << volume << std::endl;
	// TODO: Apply master volume scaling
}

// State queries
bool SDLMIXAudio::isMusicPlaying() const {
	// TODO: Check with Mix_PlayingMusic
	return false;
}

// Export functions for dynamic library loading
extern "C" {
	IAudio* createAudio() {
		return new SDLMIXAudio();
	}

	void destroyAudio(IAudio* audio) {
		delete audio;
	}
}