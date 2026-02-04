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
		virtual void playMusic(const std::string &id, bool loop = true);
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