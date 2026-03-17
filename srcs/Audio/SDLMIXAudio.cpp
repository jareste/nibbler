#include "../../incs/SDLMIXAudio.hpp"

#include <SDL.h>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#ifdef LOG_AUDIO
#include <fstream>
static std::ofstream logFile("sdl_mix_audio.log", std::ios::out | std::ios::app);

#define LOG(msg) do { \
	logFile.write(("[SDLMIXAudio] " + std::string(msg) + "\n").c_str(), 0); \
	logFile.flush(); \
} while(0)

#else
#define LOG(msg) do {} while(0)
#endif


// Constructor
SDLMIXAudio::SDLMIXAudio()
	: initialized(false),
	  soundVolume(1.0f),
	  musicVolume(1.0f),
	  masterVolume(1.0f) {
	LOG("Constructor called");
}

// Destructor
SDLMIXAudio::~SDLMIXAudio() {
	cleanup();
	LOG("Destructor called");
}

int SDLMIXAudio::toSDLVolume(float volume) const {
	float clamped = std::clamp(volume, 0.0f, 1.0f);
	return static_cast<int>(clamped * MIX_MAX_VOLUME);
}

void SDLMIXAudio::applyVolumes() {
	if (!initialized)
		return;

	const float finalSound = std::clamp(soundVolume * masterVolume, 0.0f, 1.0f);
	const float finalMusic = std::clamp(musicVolume * masterVolume, 0.0f, 1.0f);

	Mix_Volume(-1, toSDLVolume(finalSound));
	Mix_VolumeMusic(toSDLVolume(finalMusic));
}

// Initialization
void SDLMIXAudio::init() {
	if (initialized) {
		LOG("Already initialized");
		return;
	}

	LOG("Initializing SDL_mixer audio subsystem...");

	if ((SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO) == 0) {
		if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
			throw std::runtime_error(
				std::string("SDL_InitSubSystem(SDL_INIT_AUDIO) failed: ") + SDL_GetError()
			);
		}
	}

	const int flags = MIX_INIT_OGG | MIX_INIT_MP3;
	int initted = Mix_Init(flags);
	if ((initted & flags) != flags) {
		LOG("Warning: Mix_Init incomplete: " + std::string(Mix_GetError()));
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		throw std::runtime_error(
			std::string("Mix_OpenAudio failed: ") + Mix_GetError()
		);
	}

	Mix_AllocateChannels(32);

	initialized = true;
	applyVolumes();

	loadSound("eat", "assets/snake_eat.wav");
	loadMusic("bgm", "assets/snakecharmer.mp3");
	playMusic("bgm", true);
	setMusicVolume(0.5f);

	LOG("SD_mixer initialized");
}

void SDLMIXAudio::cleanup() {
	if (!initialized)
		return;

	stopMusic();

	LOG("Cleaning up SDL_mixer audio subsystem...");
	
	for (std::unordered_map<std::string, Mix_Chunk*>::iterator it = sounds.begin();
		 it != sounds.end(); ++it) {
		if (it->second)
			Mix_FreeChunk(it->second);
	}
	sounds.clear();

	for (std::unordered_map<std::string, Mix_Music*>::iterator it = musics.begin();
		 it != musics.end(); ++it) {
		if (it->second)
			Mix_FreeMusic(it->second);
	}
	musics.clear();

	currentMusicId.clear();

	Mix_CloseAudio();
	Mix_Quit();

	if ((SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO) != 0)
		SDL_QuitSubSystem(SDL_INIT_AUDIO);

	initialized = false;
	LOG("Cleanup done");
}

// Sound management
bool SDLMIXAudio::loadSound(const std::string &id, const std::string &filepath) {
	if (!initialized) {
		LOG("loadSound failed: audio not initialized");
		return false;
	}

	LOG("loadSound called with id='" + id + "' filepath='" + filepath + "'");

	unloadSound(id);

	Mix_Chunk* chunk = Mix_LoadWAV(filepath.c_str());
	if (!chunk) {
		LOG("loadSound failed: Mix_LoadWAV failed for '" + filepath + "': " + std::string(Mix_GetError()));
		return false;
	}

	sounds[id] = chunk;
	LOG("Loaded sound '" + id + "' from " + filepath);
	return true;
}

void SDLMIXAudio::unloadSound(const std::string &id) {
	LOG("unloadSound called with id='" + id + "'");
	std::unordered_map<std::string, Mix_Chunk*>::iterator it = sounds.find(id);
	if (it != sounds.end()) {
		if (it->second)
			Mix_FreeChunk(it->second);
		sounds.erase(it);
		LOG("Unloaded sound '" + id + "'");
	}
}

bool SDLMIXAudio::loadMusic(const std::string &id, const std::string &filepath) {
	if (!initialized) {
		LOG("loadMusic failed: audio not initialized");
		return false;
	}

	LOG("loadMusic called with id='" + id + "' filepath='" + filepath + "'");

	std::unordered_map<std::string, Mix_Music*>::iterator existing = musics.find(id);
	if (existing != musics.end()) {
		if (existing->second)
			Mix_FreeMusic(existing->second);
		musics.erase(existing);
	}

	Mix_Music* music = Mix_LoadMUS(filepath.c_str());
	if (!music) {
		LOG("loadMusic failed: Mix_LoadMUS failed for '" + filepath + "': " + std::string(Mix_GetError()));
		return false;
	}

	musics[id] = music;
	LOG("Loaded music '" + id + "' from " + filepath);
	return true;
}

void SDLMIXAudio::unloadMusic(const std::string &id) {
	std::unordered_map<std::string, Mix_Music*>::iterator it = musics.find(id);
	if (it != musics.end()) {
		if (currentMusicId == id)
			Mix_HaltMusic();
		if (it->second)
			Mix_FreeMusic(it->second);
		musics.erase(it);
		if (currentMusicId == id)
			currentMusicId.clear();
	}
}

void SDLMIXAudio::playSound(const std::string &id) {
	if (!initialized) {
		LOG("playSound failed: audio not initialized");
		return;
	}

	LOG("playSound called with id='" + id + "'");

	std::unordered_map<std::string, Mix_Chunk*>::iterator it = sounds.find(id);
	if (it == sounds.end() || !it->second) {
		LOG("playSound failed: unknown sound id '" + id + "'");
		return;
	}

	if (Mix_PlayChannel(-1, it->second, 0) == -1) {
		LOG("playSound failed: Mix_PlayChannel failed for '" + id + "': " + std::string(Mix_GetError()));
	}
}

void SDLMIXAudio::playMusic(const std::string &id, bool loop) {
	if (!initialized) {
		LOG("playMusic failed: audio not initialized");
		return;
	}

	LOG("playMusic called with id='" + id + "' loop=" + (loop ? "true" : "false"));

	std::unordered_map<std::string, Mix_Music*>::iterator it = musics.find(id);
	if (it == musics.end() || !it->second) {
		LOG("playMusic failed: unknown music id '" + id + "'");
		return;
	}

	int loops = loop ? -1 : 1;
	if (Mix_PlayMusic(it->second, loops) == -1) {
		LOG("playMusic failed: Mix_PlayMusic failed for '" + id + "': " + std::string(Mix_GetError()));
		return;
	}

	currentMusicId = id;
}

void SDLMIXAudio::stopMusic() {
	if (!initialized)
		return;
	LOG("stopMusic called");
	Mix_HaltMusic();
}

void SDLMIXAudio::pauseMusic() {
	if (!initialized)
		return;
	LOG("pauseMusic called");
	if (Mix_PlayingMusic())
		Mix_PauseMusic();
}

void SDLMIXAudio::resumeMusic() {
	if (!initialized)
		return;
	LOG("resumeMusic called");
	if (Mix_PausedMusic())
		Mix_ResumeMusic();
}

// Volume control
void SDLMIXAudio::setSoundVolume(float volume) {
	soundVolume = std::clamp(volume, 0.0f, 1.0f);
	applyVolumes();
}

void SDLMIXAudio::setMusicVolume(float volume) {
	musicVolume = std::clamp(volume, 0.0f, 1.0f);
	applyVolumes();
}

void SDLMIXAudio::setMasterVolume(float volume) {
	masterVolume = std::clamp(volume, 0.0f, 1.0f);
	applyVolumes();
}

// State queries
bool SDLMIXAudio::isMusicPlaying() const {
	return initialized && Mix_PlayingMusic() != 0;
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
