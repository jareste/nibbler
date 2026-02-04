#pragma once
#include <string>

class IAudio {
	public:
		virtual ~IAudio() = default;

		// init
		virtual void init() = 0;
		virtual void cleanup() = 0;

		// sound management
		virtual bool loadSound(const std::string &id, const std::string &filepath) = 0;
		virtual void unloadSound(const std::string &id) = 0;

		// playback
		virtual void playSound(const std::string &id) = 0;
		virtual void playMusic(const std::string &id, bool loop) = 0;
		virtual void stopMusic() = 0;
		virtual void pauseMusic() = 0;
		virtual void resumeMusic() = 0;

		// Volume control -> normalized between 0.0f and 1.0f
		virtual void setSoundVolume(float volume) = 0;
		virtual void setMusicVolume(float volume) = 0;
		virtual void setMasterVolume(float volume) = 0;

		// Helpers
		virtual bool isMusicPlaying() const = 0;
};


// This don't need to be here. In fact, they need to be taken out, but I'm leaving them
// to have the recipes when the time to build the audio implementation (i.e., the specific, library linked cpp file)

extern "C" {
	IAudio *createAudio();
	void destroyAudio(IAudio*);
}