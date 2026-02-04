#pragma once
#include "IAudio.hpp"
#include <string>
#include <unordered_map>

// Forward declarations if needed later
// struct Mix_Chunk;
// typedef struct _Mix_Music Mix_Music;

class SDLMIXAudio : public IAudio {
	private:
		// Add your SDL_mixer specific members here later
		// std::unordered_map<std::string, Mix_Chunk*> sounds;
		// Mix_Music* currentMusic;
		float soundVolume;
		float musicVolume;
		float masterVolume;

	public:
		SDLMIXAudio();
		SDLMIXAudio(const SDLMIXAudio &other) = delete;
		SDLMIXAudio &operator=(const SDLMIXAudio &other) = delete;

		~SDLMIXAudio() override;
	
		void init() override;
		void cleanup() override;

		bool loadSound(const std::string &id, const std::string &filepath) override;
		void unloadSound(const std::string &id) override;

		void playSound(const std::string &id) override;
		void playMusic(const std::string &id, bool loop) override;
		void stopMusic() override;
		void pauseMusic() override;
		void resumeMusic() override;

		void setSoundVolume(float volume) override;
		void setMusicVolume(float volume) override;
		void setMasterVolume(float volume) override;

		bool isMusicPlaying() const override;
};
