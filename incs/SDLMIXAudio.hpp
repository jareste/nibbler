#pragma once
#include "IAudio.hpp"
#include <ncurses.h>
#include <locale.h>
#include <iostream>
#include <vector>
#include <random>
#include <cstring>
#include <fstream>
#include <string>



class SDLMIXAudio : public IAudio {
	private:

	public:
		SDLMIXAudio() = default;;
		SDLMIXAudio(const SDLMIXAudio &other) = delete;
		SDLMIXAudio &operator=(const SDLMIXAudio &other) = delete;

		~SDLMIXAudio() = default;
	
		virtual void init() override;
		virtual void cleanup() override;

		virtual bool loadSound(const std::string &id, const std::string &filepath) override;
		virtual void unloadSound(const std::string &id) override;

		virtual void playSound(const std::string &id) override;
		virtual void playMusic(const std::string &id, bool loop = true);
		virtual void stopMusic() override;
		virtual void pauseMusic() override;
		virtual void resumeMusic() override;

		virtual void setSoundVolume(float volume) override;
		virtual void setMusicVolume(float volume) override;
		virtual void setMasterVolume(float volume) override;

		virtual bool isMusicPlaying() const override;
};
