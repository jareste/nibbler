#pragma once
#include "IGraphic.hpp"
#include "Snake.hpp"
#include "Food.hpp"
#include "colors.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <iostream>
#include <array>

class RaylibGraphic : public IGraphic {
private:
	float	cubeSize;
	float	cameraSize;
	float 	menuFov;
	float	customFov;
	float	separator;
	int		gridWidth;
	int		gridHeight;
	int		screenWidth;
	int		screenHeight;

	float	accumulatedTime;

	Camera3D	camera;
	
	// Postprocessing noise system
	static const int							GRAIN_TEXTURE_COUNT = 8;
	std::array<Texture2D, GRAIN_TEXTURE_COUNT>	grainTextures;
	int											currentGrainFrame;
	float										grainFrameTimer;
	float										grainFrameInterval;
	
	// Font with Unicode support
	Font										customFont;
	
	// Colors
	Color customWhite = { 255, 248, 227, 255};				// Warm off-white (cream)
	Color customGray = { 125, 125, 125, 255};				// Mid gray
	Color customBlack = { 23, 23, 23, 255};					// Deep navy black
	
	// Ground colors - Light squares
	Color groundLightTop = customWhite;
	Color groundLightFront = { 26, 64, 96, 255 };			// Blue
	Color groundLightSide = { 254, 74, 81, 255 };			// Red
	Color groundHidden = customBlack;
	
	// Ground colors - Dark squares
	Color groundDarkTop = { 200, 195, 178, 255 };			// Darker cream/beige
	Color groundDarkFront = { 18, 45, 68, 255 };			// Darker blue
	Color groundDarkSide = { 180, 52, 58, 255 };			// Darker red
	
	// Wall colors
	Color wallColor = { 147, 112, 219, 255 };				// Medium purple
	Color wallColorFade = { 147, 112, 219, 120 };			// Faded purple (transparent)
	
	// Snake colors - Blue shades (matches ground blue)
	// Light segments
	Color snakeLightTop = { 135, 206, 250, 255 };			// Light sky blue (brightest)
	Color snakeLightFront = { 26, 64, 96, 255 };			// Ground blue (darker)
	Color snakeLightSide = { 70, 130, 180, 255 };			// Steel blue (lighter than front)
	
	// Dark segments
	Color snakeDarkTop = { 70, 130, 180, 255 };				// Steel blue
	Color snakeDarkFront = { 18, 45, 68, 255 };				// Ground dark blue (darker)
	Color snakeDarkSide = { 26, 64, 96, 255 };				// Ground blue (lighter than front)
	Color snakeHidden = customBlack;
	
	// Food colors - Red shades (based on ground red)
	Color foodTop = { 255, 120, 120, 255 };					// Light coral red (brightest)
	Color foodFront = { 180, 52, 58, 255 };					// Ground dark red (darker)
	Color foodSide = { 254, 74, 81, 255 };					// Ground red (lighter than front)
	Color foodHidden = customBlack; 
	
public:
	RaylibGraphic();
	RaylibGraphic(const RaylibGraphic &other) = delete;
	RaylibGraphic &operator=(const RaylibGraphic &other) = delete;
	~RaylibGraphic();

	void setupCamera(); 
	void drawGroundPlane() ;
	void drawWalls();
	void drawSnake(const Snake* snake);
	void drawFood(const Food* food);
	void drawCubeCustomFaces(Vector3 position, float width, float height, float length,
	                         Color front, Color back, Color top, Color bottom, Color right, Color left);
	void drawNoiseGrain();  // Post Processing
	void DrawText3D(Font font, const char *text, Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, Color tint);
	void DrawTextCodepoint3D(Font font, int codepoint, Vector3 position, float fontSize, bool backface, Color tint);

	void DrawOutlinedText(const char *text, int posX, int posY, int fontSize, Color color, int outlineSize, Color outlineColor);

	void init(int width, int height) override;
	void render(const GameState& state, float deltaTime) override;
	void renderMenu(const GameState &state, float deltaTime) override;
	void drawTitle();
	void drawInstructions();
	void renderGameOver(const GameState &state, float deltaTime) override;
	Input pollInput() override;
};

extern "C" IGraphic* createGraphic() {
	return new RaylibGraphic();
}

extern "C" void destroyGraphic(IGraphic* g) {
	delete g;
}
