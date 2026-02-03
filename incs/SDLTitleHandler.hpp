#pragma once

#include <SDL2/SDL.h>
#include <vector>

class SDLTitleHandler {
private:
	SDL_Renderer* renderer;
	
	// Helper to draw multiple rectangles at once
	void drawRects(const std::vector<SDL_Rect>& rects, SDL_Color color);

public:
	SDLTitleHandler(SDL_Renderer* renderer);
	~SDLTitleHandler() = default;

	void renderTitle(int centerX, int centerY, int square, int sep, SDL_Color white, SDL_Color blue, SDL_Color red);
	void renderGameOver(int centerX, int centerY, int square, int sep, SDL_Color white);

	void drawGameOver();
};
