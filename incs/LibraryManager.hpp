#pragma once
#include <dlfcn.h>
#include <iostream>

class IGraphic;
class IAudio;

class LibraryManager {
	private:
		// Graphic stuff
		void *graphicHandle;
		IGraphic *graphic;
		using CreateGraphicFn = IGraphic *(*)();
		using DestroyGraphicFn = void (*)(IGraphic*);

		// Audio stuff
		void	*audioHandle;
		IAudio	*audio;
		using	CreateAudioFn = IAudio *(*)();
		using	DestroyAudioFn = void (*)(IAudio*);
	
	public:
		LibraryManager();
		LibraryManager(const LibraryManager&) = delete;
		LibraryManager &operator=(const LibraryManager&) = delete;

		~LibraryManager();

		bool loadGraphicLib(const char *libPath);
		void unloadGraphicLib();

		bool loadAudioLib(const char *libpath);
		void unloadAudioLib();

		IGraphic *getGraphicLib();
		IAudio *getAudioLib();
};