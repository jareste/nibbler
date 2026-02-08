#include "../incs/SDLTextRenderer.hpp"

SDLTextRenderer::SDLTextRenderer(SDL_Renderer* renderer) 
	: renderer(renderer), mainFont(nullptr), smallFont(nullptr), initialized(false),
	  windowWidth(0), windowHeight(0), cellSize(0), borderOffset(0), square(0) {
}

SDLTextRenderer::~SDLTextRenderer() {
	if (mainFont) {
		TTF_CloseFont(mainFont);
		mainFont = nullptr;
	}
	if (smallFont) {
		TTF_CloseFont(smallFont);
		smallFont = nullptr;
	}
}

bool SDLTextRenderer::init(int width, int height, int cell, int border) {
	windowWidth = width;
	windowHeight = height;
	cellSize = cell;
	borderOffset = border;
	square = cellSize;

	int mainSize = (windowWidth < 1800) ? 34 : 34;
	int smallSize = (windowWidth < 1800) ? 24 : 24;

	// Load main font
	mainFont = TTF_OpenFont("fonts/JetBrainsMono-VariableFont_wght.ttf", mainSize);
	if (!mainFont) {
		// Fallback to system font
		mainFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", mainSize);
		if (!mainFont) {
			std::cerr << "[SDL Text Renderer] Failed to load main font: " << TTF_GetError() << std::endl;
			return false;
		}
	}

	// Load small font
	smallFont = TTF_OpenFont("fonts/JetBrainsMono-VariableFont_wght.ttf", smallSize);
	if (!smallFont) {
		// Fallback to system font
		smallFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", smallSize);
		if (!smallFont) {
			std::cerr << "[SDL Text Renderer] Failed to load small font: " << TTF_GetError() << std::endl;
			// Use main font as fallback
			smallFont = mainFont;
		}
	}

	initialized = (mainFont != nullptr);
	return initialized;
}

bool SDLTextRenderer::drawText(const std::string& text, int x, int y, int offset, 
								TTF_Font* fontToUse, SDL_Color color, bool centered) {
	if (!fontToUse || !initialized)
		return false;

	SDL_Surface* surface = TTF_RenderUTF8_Blended(fontToUse, text.c_str(), color);
	if (!surface) {
		std::cerr << "[SDL Text Renderer] Failed to render text: " << TTF_GetError() << std::endl;
		return false;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		SDL_FreeSurface(surface);
		std::cerr << "[SDL Text Renderer] Failed to create texture: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_Rect destRect;
	if (centered) {
		destRect = {
			x - (surface->w / 2),
			y + offset - (surface->h / 2),
			surface->w,
			surface->h
		};
	} else {
		destRect = {x, y + offset, surface->w, surface->h};
	}

	SDL_RenderCopy(renderer, texture, nullptr, &destRect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	return true;
}

void SDLTextRenderer::drawInstruction(int centerX, int centerY, int& offset,
                                       const std::string& labelText, const std::string& dotText,
                                       bool smallMode, TTF_Font* currentFont) {
	if (!initialized)
		return;

	// text -> white
	SDL_Color textColor = customWhite;
	drawText(labelText, centerX, centerY, offset, currentFont, textColor, true);

	// Dots -> gray
	textColor = customGray;
	drawText(dotText, centerX, centerY, offset, currentFont, textColor, true);

	offset += (smallMode ? 40 : 90);  // instructions line offset
}

void SDLTextRenderer::drawInstructions(int centerX, int centerY) {
	if (!initialized)
		return;

	bool smallMode = (windowWidth / 2) < 900;
	TTF_Font* currentFont = smallMode ? smallFont : mainFont;
	int offset = smallMode ? square * 3 : square * 7;
	
	// Enter instruction
	std::string instructionTextA = smallMode ?
		"[ ENTER ]          START" :
		"[ ENTER ]             START";
	std::string instructionTextB = smallMode ?
		"         ··········     " :
		"         ············     ";
	drawInstruction(centerX, centerY, offset, 
	                instructionTextA, instructionTextB, 
	                smallMode, currentFont);
	
	// Move instruction
	instructionTextA = smallMode ?
		"[ ↑ ↓ ← → ]         MOVE" :
		"[ ↑ ↓ ← → ]            MOVE";
	instructionTextB = smallMode ?
		"           ·········    " :
		"           ············    ";
	drawInstruction(centerX, centerY, offset, 
	                instructionTextA, instructionTextB, 
	                smallMode, currentFont);

	// Travel instruction
	instructionTextA = smallMode ?
		"[ 1   2   3 ]     TRAVEL" :
		"[ 1   2   3 ]        TRAVEL";
	instructionTextB = smallMode ?
		"    /   /    ·····      " :
		"    /   /    ········      ";
	drawInstruction(centerX, centerY, offset, 
	                instructionTextA, instructionTextB, 
	                smallMode, currentFont);

	// Quit instruction
	instructionTextA = smallMode ?
		"[ Q   ESC ]         QUIT" :
		"[ Q   ESC ]            QUIT";
	instructionTextB = smallMode ?
		"    /      ·········    " :
		"    /      ···········    ";
	drawInstruction(centerX, centerY, offset, 
	                instructionTextA, instructionTextB, 
	                smallMode, currentFont);
}

void SDLTextRenderer::drawScore(const GameState& state, int centerX, int centerY) {
	if (!initialized)
		return;

	bool smallMode = (windowWidth / 2) < 900;
	TTF_Font* currentFont = smallMode ? smallFont : mainFont;
	int yOffset = smallMode ? square * 3 : square * 7;
	
	std::string scoreNum = std::to_string(state.score);
	std::string appleWord = (state.score == 1) ? "APPLE" : "APPLES";
	
	int wordSpacing = 10;
	
	SDL_Surface* youSurface = TTF_RenderUTF8_Blended(currentFont, "YOU", lightBlue);
	SDL_Surface* ateSurface = TTF_RenderUTF8_Blended(currentFont, "ATE", customWhite);
	SDL_Surface* scoreSurface = TTF_RenderUTF8_Blended(currentFont, scoreNum.c_str(), lightRed);
	SDL_Surface* appleSurface = TTF_RenderUTF8_Blended(currentFont, appleWord.c_str(), customWhite);
	
	if (!youSurface || !ateSurface || !scoreSurface || !appleSurface) {
		if (youSurface) SDL_FreeSurface(youSurface);
		if (ateSurface) SDL_FreeSurface(ateSurface);
		if (scoreSurface) SDL_FreeSurface(scoreSurface);
		if (appleSurface) SDL_FreeSurface(appleSurface);
		return;
	}
	
	int totalWidth = youSurface->w + wordSpacing + ateSurface->w + wordSpacing + 
	                 scoreSurface->w + wordSpacing + appleSurface->w;
	
	int startX = centerX - (totalWidth / 2);
	int currentX = startX;
	
	// "YOU" - blue
	SDL_Texture* youTexture = SDL_CreateTextureFromSurface(renderer, youSurface);
	if (youTexture) {
		SDL_Rect youRect = {
			currentX,
			centerY + yOffset - (youSurface->h / 2),
			youSurface->w,
			youSurface->h
		};
		SDL_RenderCopy(renderer, youTexture, nullptr, &youRect);
		SDL_DestroyTexture(youTexture);
	}
	currentX += youSurface->w + wordSpacing;
	SDL_FreeSurface(youSurface);
	
	// "ATE" - white
	SDL_Texture* ateTexture = SDL_CreateTextureFromSurface(renderer, ateSurface);
	if (ateTexture) {
		SDL_Rect ateRect = {
			currentX,
			centerY + yOffset - (ateSurface->h / 2),
			ateSurface->w,
			ateSurface->h
		};
		SDL_RenderCopy(renderer, ateTexture, nullptr, &ateRect);
		SDL_DestroyTexture(ateTexture);
	}
	currentX += ateSurface->w + wordSpacing;
	SDL_FreeSurface(ateSurface);
	
	// Score - red
	SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	if (scoreTexture) {
		SDL_Rect scoreRect = {
			currentX,
			centerY + yOffset - (scoreSurface->h / 2),
			scoreSurface->w,
			scoreSurface->h
		};
		SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
		SDL_DestroyTexture(scoreTexture);
	}
	currentX += scoreSurface->w + wordSpacing;
	SDL_FreeSurface(scoreSurface);
	
	// "APPLE(S)" - white
	SDL_Texture* appleTexture = SDL_CreateTextureFromSurface(renderer, appleSurface);
	if (appleTexture) {
		SDL_Rect appleRect = {
			currentX,
			centerY + yOffset - (appleSurface->h / 2),
			appleSurface->w,
			appleSurface->h
		};
		SDL_RenderCopy(renderer, appleTexture, nullptr, &appleRect);
		SDL_DestroyTexture(appleTexture);
	}
	SDL_FreeSurface(appleSurface);
}

void SDLTextRenderer::drawRetryPrompt(int centerX, int centerY) {
	if (!initialized)
		return;

	bool smallMode = (windowWidth / 2) < 900;
	TTF_Font* currentFont = smallMode ? smallFont : mainFont;
	int offset = smallMode ? square * 4 : square * 8;
	
	// Retry instructions
	std::string gameoverTextA = smallMode ?
		"[ ENTER ]          RETRY" :
		"[ ENTER ]             RETRY";
	std::string gameoverTextB = smallMode ?
		"         ·········     " :
		"        ·············    ";
	
	drawInstruction(centerX, centerY, offset, 
	                gameoverTextA, gameoverTextB, 
	                smallMode, currentFont);

	// Quit instruction
	gameoverTextA = smallMode ?
		"[ Q   ESC ]         QUIT" :
		"[ Q   ESC ]            QUIT";
	gameoverTextB = smallMode ?
		"    /       ·······     " :
		"    /       ···········    ";
	
	drawInstruction(centerX, centerY, offset, 
	                gameoverTextA, gameoverTextB, 
	                smallMode, currentFont);
}

TTF_Font*	SDLTextRenderer::getMainFont() const { return mainFont; }
TTF_Font*	SDLTextRenderer::getSmallFont() const { return smallFont; }
bool		SDLTextRenderer::isInitialized() const { return initialized; }
