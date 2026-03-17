#pragma once

#include "IAudio.hpp"
#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

class SDLMIXAudio : public IAudio {
public:
	SDLMIXAudio();
	~SDLMIXAudio() override;

	void init() override;
	void cleanup() override;

	bool loadSound(const std::string &id, const std::string &filepath) override;
	void unloadSound(const std::string &id) override;
	bool loadMusic(const std::string &id, const std::string &filepath) override;
	void unloadMusic(const std::string &id) override;

	void playSound(const std::string &id) override;
	void playMusic(const std::string &id, bool loop) override;
	void stopMusic() override;
	void pauseMusic() override;
	void resumeMusic() override;

	void setSoundVolume(float volume) override;
	void setMusicVolume(float volume) override;
	void setMasterVolume(float volume) override;

	bool isMusicPlaying() const override;

private:
	int toSDLVolume(float volume) const;
	void applyVolumes();

private:
	bool initialized;
	float soundVolume;
	float musicVolume;
	float masterVolume;

	std::unordered_map<std::string, Mix_Chunk*> sounds;
	std::unordered_map<std::string, Mix_Music*> musics;
	std::string currentMusicId;
};
