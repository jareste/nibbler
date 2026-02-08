#pragma once
#include "IGraphic.hpp"
#include "Snake.hpp"
#include "Food.hpp"
#include "colors.h"
#include "RaylibTitleHandler.hpp"
#include "RaylibTextRenderer.hpp"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <iostream>
#include <array>
#include <memory>

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
	
	std::unique_ptr<RaylibTitleHandler> titleHandler;
	std::unique_ptr<RaylibTextRenderer>	textRenderer;

	// Postprocessing noise system
	static const int							GRAIN_TEXTURE_COUNT = 8;
	std::array<Texture2D, GRAIN_TEXTURE_COUNT>	grainTextures;
	int											currentGrainFrame;
	float										grainFrameTimer;
	float										grainFrameInterval;
	
	// Colors
	Color customWhite = { 255, 248, 227, 255};				// Warm off-white (cream)
	Color customGray = { 125, 125, 125, 255};				// Mid gray
	Color customBlack = { 23, 23, 23, 255};					// Deep navy black
	
	// Logo colors - Dark segments (darker overall, balanced contrast)
	Color logoDarkTop = { 180, 175, 160, 255 };				// Medium-dark cream/beige (brightest of dark set)
	Color logoDarkFront = { 70, 67, 58, 255 };				// Dark brown-gray (not too close to black)
	Color logoDarkSide = { 125, 120, 108, 255 };			// Medium warm gray (between top and front)

	// Logo colors - Light segments (lighter overall, more contrast)
	Color logoLightTop = customWhite;						// Brightest (255, 248, 227)
	Color logoLightFront = { 170, 165, 148, 255 };			// Medium beige (darker for more contrast)
	Color logoLightSide = { 215, 208, 188, 255 };			// Light beige (between top and front)

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
	
	// Snake A colors - Blue shades (matches ground blue)
	// Light segments
	Color snakeALightTop = { 135, 206, 250, 255 };			// Light sky blue (brightest)
	Color snakeALightFront = { 26, 64, 96, 255 };			// Ground blue (darker)
	Color snakeALightSide = { 70, 130, 180, 255 };			// Steel blue (lighter than front)
	
	// Dark segments
	Color snakeADarkTop = { 70, 130, 180, 255 };			// Steel blue
	Color snakeADarkFront = { 18, 45, 68, 255 };			// Ground dark blue (darker)
	Color snakeADarkSide = { 26, 64, 96, 255 };				// Ground blue (lighter than front)
	Color snakeAHidden = customBlack;
	
	// Snake B colors - Green/Yellow shades
	// Light segments
	Color snakeBLightTop = { 255, 255, 153, 255 };			// Light yellow (brightest)
	Color snakeBLightFront = { 184, 134, 11, 255 };			// Dark golden rod (darker)
	Color snakeBLightSide = { 255, 215, 0, 255 };			// Golden yellow (lighter than front)
	
	// Dark segments
	Color snakeBDarkTop = { 255, 215, 0, 255 };				// Golden yellow
	Color snakeBDarkFront = { 218, 165, 32, 255 };			// Goldenrod (darker)
	Color snakeBDarkSide = { 184, 134, 11, 255 };			// Dark golden rod (lighter than front)
	Color snakeBHidden = customBlack;
	
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

	friend class RaylibTitleHandler;
	friend class RaylibTextRenderer;

	float getCubeSize() const;
    float getSeparator() const;
    Camera3D& getCamera();
    float& getAccumulatedTime();

	void setupCamera(); 
	void drawGroundPlane() ;
	void drawWalls();
	void drawSnake(const Snake* snake);
	void drawFood(const Food* food);
	void drawCubeCustomFaces(Vector3 position, float width, float height, float length,
	                         Color front, Color back, Color top, Color bottom, Color right, Color left);
	void drawNoiseGrain();  // Post Processing

	void init(int width, int height) override;
	void render(const GameState& state, float deltaTime) override;
	void renderMenu(const GameState &state, float deltaTime) override;
	void renderGameOver(const GameState &state, float deltaTime) override;
	Input pollInput() override;
};
