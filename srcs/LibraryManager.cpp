#include "../incs/LibraryManager.hpp"

LibraryManager::LibraryManager() : 
	graphicHandle(nullptr), graphic(nullptr),
	audioHandle(nullptr), audio(nullptr)	 
{}

LibraryManager::~LibraryManager() { unloadGraphicLib(); unloadAudioLib(); }

// video
bool LibraryManager::loadGraphicLib(const char *libPath) {
	graphicHandle = dlopen(libPath, RTLD_NOW);
		if (!graphicHandle) {
			std::cerr << "dlopen error: " << dlerror() << std::endl;
			return false;
		}
		
		CreateGraphicFn create = (CreateGraphicFn)dlsym(graphicHandle, "createGraphic");
		DestroyGraphicFn destroy = (DestroyGraphicFn)dlsym(graphicHandle, "destroyGraphic");
		
		if (!create || !destroy) {
			std::cerr << "Symbol error: " << dlerror() << std::endl;
			dlclose(graphicHandle);
			graphicHandle = nullptr;
			return false;
		}
		
		graphic = create();
		std::cout << "Loaded: " << libPath << std::endl;
		return true;
}

void LibraryManager::unloadGraphicLib() {
	if (graphic) {
		using DestroyFn = void (*)(IGraphic*);
		DestroyFn destroy = (DestroyFn)dlsym(graphicHandle, "destroyGraphic");
		if (destroy) {
			destroy(graphic);
		}
		graphic = nullptr;
	}

	if (graphicHandle) {
		dlclose(graphicHandle);
		graphicHandle = nullptr;
	}
}

// audio
bool LibraryManager::loadAudioLib(const char* libPath) {
	audioHandle = dlopen(libPath, RTLD_NOW);

	if (!audioHandle) {
		std::cerr << "dl open error: " << dlerror() << std::endl;
		return false;
	}

	CreateAudioFn create = (CreateAudioFn)dlsym(audioHandle, "createAudio");
	DestroyAudioFn destroy = (DestroyAudioFn)dlsym(audioHandle, "destroyAudio");

	if (!create || !destroy) {
		std::cerr << "Symbol error: " << dlerror() << std::endl;
			dlclose(audioHandle);
			audioHandle = nullptr;
			return false;
	}

	audio = create();
	std::cout << "Loaded: " << libPath << std::endl;
	return true;
}

void LibraryManager::unloadAudioLib() {
	if (audio) {
		using DestroyAudioFn = void (*)(IAudio*);
		DestroyAudioFn destroy = (DestroyAudioFn)dlsym(audioHandle, "destroyAudio");
		if (destroy) {
			destroy(audio);
		}
		audio = nullptr;

		if (audioHandle) {
			dlclose(audioHandle);
			audioHandle = nullptr;
		}
	}
}

// getter
IGraphic *LibraryManager::getGraphicLib() { return graphic; }

IAudio *LibraryManager::getAudioLib() { return audio; }