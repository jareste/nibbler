#include "../../incs/IGraphic.hpp"
#include "../../incs/Snake.hpp"
#include "../../incs/Food.hpp"
#include "../../incs/colors.h"
#include "../../incs/RaylibGraphic.hpp"
#include <rlgl.h>  // For low-level drawing functions (rlPushMatrix, rlBegin, etc.)

RaylibGraphic::RaylibGraphic() :
	cubeSize(2.0f),
	menuFov(50.0f),
	gridWidth(0),
	gridHeight(0),
	screenWidth(1920),
	screenHeight(1080),
	accumulatedTime(0.0f),
	currentGrainFrame(0),
	grainFrameTimer(0.0f),
	grainFrameInterval(0.05f) {
		separator = cubeSize * 2;
	}

RaylibGraphic::~RaylibGraphic() {
	// Unload all grain textures
	for (int i = 0; i < GRAIN_TEXTURE_COUNT; i++) {
		UnloadTexture(grainTextures[i]);
	}
	// Unload Unicode font
	UnloadFont(customFont);
	CloseWindow();
	std::cout << BYEL << "[Raylib 3D] Destroyed" << RESET << std::endl;
}

void RaylibGraphic::init(int width, int height) {
	gridWidth = width;
	gridHeight = height;
	
	InitWindow(screenWidth, screenHeight, "Nibbler 3D - Raylib");
	SetTargetFPS(60);
	
	setupCamera();
	
	// Load font with Unicode support
	// Create array with all codepoints we need
	int codepointCount = 256 - 32 + 8;  // ASCII printable (32-255) + arrows + dot
	int *codepoints = new int[codepointCount];
	
	// Load basic ASCII printable characters (32-255)
	for (int i = 0; i < (256 - 32); i++) {
		codepoints[i] = 32 + i;
	}
	
	// Add Unicode arrows and middle dot
	codepoints[256 - 32 + 0] = 0x2191;  // ↑
	codepoints[256 - 32 + 1] = 0x2193;  // ↓
	codepoints[256 - 32 + 2] = 0x2190;  // ←
	codepoints[256 - 32 + 3] = 0x2192;  // →
	codepoints[256 - 32 + 4] = 0x00B7;  // · (middle dot)
	codepoints[256 - 32 + 5] = 0x2022;  // • (bullet point, alternative)
	codepoints[256 - 32 + 6] = 0x00B7;  // · (duplicate for safety)
	codepoints[256 - 32 + 7] = 0x002E;  // . (period, fallback)
	
	// DejaVu Sans has better Unicode coverage including arrow characters
	customFont = LoadFontEx("fonts/JetBrainsMono-VariableFont_wght.ttf", 64, codepoints, codepointCount);
	delete[] codepoints;
	
	// noise pattern generation
	for (int i = 0; i < GRAIN_TEXTURE_COUNT; i++) {
		// Unique seeding for varaition
		Image grainImage = GenImageWhiteNoise(screenWidth, screenHeight, 0.75f);
		grainTextures[i] = LoadTextureFromImage(grainImage);
		UnloadImage(grainImage);
	}

	titleHandler = std::make_unique<RaylibTitleHandler>(*this);
	textRenderer = std::make_unique<RaylibTextRenderer>(*this);
	
	std::cout << BYEL << "[Raylib 3D] Initialized: " << width << "x" << height << RESET << std::endl;
}

void RaylibGraphic::drawCubeCustomFaces(Vector3 position, float width, float height, float length,
                                         Color front, Color back, Color top, Color bottom, Color right, Color left) {
	float x = position.x;
	float y = position.y;
	float z = position.z;
	
	// In isometric view, typically visible faces are: front (+Z), top (+Y), right (+X)
	rlPushMatrix();
	rlTranslatef(x, y, z);
	
	rlBegin(RL_QUADS);
	
	// Front face (+Z) - typically visible in isometric
	rlColor4ub(front.r, front.g, front.b, front.a);
	rlVertex3f(-width/2, -height/2, length/2);
	rlVertex3f(width/2, -height/2, length/2);
	rlVertex3f(width/2, height/2, length/2);
	rlVertex3f(-width/2, height/2, length/2);
	
	// Back face (-Z)
	rlColor4ub(back.r, back.g, back.b, back.a);
	rlVertex3f(-width/2, -height/2, -length/2);
	rlVertex3f(-width/2, height/2, -length/2);
	rlVertex3f(width/2, height/2, -length/2);
	rlVertex3f(width/2, -height/2, -length/2);
	
	// Top face (+Y) - typically visible in isometric
	rlColor4ub(top.r, top.g, top.b, top.a);
	rlVertex3f(-width/2, height/2, -length/2);
	rlVertex3f(-width/2, height/2, length/2);
	rlVertex3f(width/2, height/2, length/2);
	rlVertex3f(width/2, height/2, -length/2);
	
	// Bottom face (-Y)
	rlColor4ub(bottom.r, bottom.g, bottom.b, bottom.a);
	rlVertex3f(-width/2, -height/2, -length/2);
	rlVertex3f(width/2, -height/2, -length/2);
	rlVertex3f(width/2, -height/2, length/2);
	rlVertex3f(-width/2, -height/2, length/2);
	
	// Right face (+X) - typically visible in isometric
	rlColor4ub(right.r, right.g, right.b, right.a);
	rlVertex3f(width/2, -height/2, -length/2);
	rlVertex3f(width/2, height/2, -length/2);
	rlVertex3f(width/2, height/2, length/2);
	rlVertex3f(width/2, -height/2, length/2);
	
	// Left face (-X)
	rlColor4ub(left.r, left.g, left.b, left.a);
	rlVertex3f(-width/2, -height/2, -length/2);
	rlVertex3f(-width/2, -height/2, length/2);
	rlVertex3f(-width/2, height/2, length/2);
	rlVertex3f(-width/2, height/2, -length/2);
	
	rlEnd();
	rlPopMatrix();
}

void RaylibGraphic::setupCamera() {
	// Grid is now centered at origin (0, 0, 0)
	float centerX = 0.0f;
	float centerZ = 0.0f;
	
	float diagonal = sqrtf(gridWidth * gridWidth + gridHeight * gridHeight) * cubeSize;
	float distance = diagonal * 2.2f;  // 20% padding
	
	float elevation = 35.264f * DEG2RAD;  // Classic isometric angle
	float rotation = 45.0f * DEG2RAD;
	
	camera.position = (Vector3){ 
		centerX + distance * cosf(rotation) * cosf(elevation),
		distance * sinf(elevation),
		centerZ + distance * sinf(rotation) * cosf(elevation)
	};
	
	camera.target = (Vector3){ centerX, 0.0f, centerZ };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };

	cameraSize = static_cast<float>((gridWidth + gridHeight) / 2);
	customFov = 0.022619f * cameraSize * cameraSize + 0.198810f * cameraSize + 31.028571f;
	
	camera.fovy = customFov;
	camera.projection = CAMERA_ORTHOGRAPHIC;
}

void RaylibGraphic::drawGroundPlane() {
	float offsetX = (gridWidth * cubeSize) / 2.0f;
	float offsetZ = (gridHeight * cubeSize) / 2.0f;
	
	for (int z = 0; z < gridHeight; z++) {
		for (int x = 0; x < gridWidth; x++) {		
			Vector3 position = {
				x * cubeSize - offsetX,
				0.0f,
				z * cubeSize - offsetZ
			};
			
			if ((x + z) % 2 == 0) {
				drawCubeCustomFaces(position, cubeSize, cubeSize, cubeSize,
									groundLightFront, groundHidden, groundLightTop, groundHidden, groundLightSide, groundHidden);
			}
			else {
				drawCubeCustomFaces(position, cubeSize, cubeSize, cubeSize,
									groundDarkFront, groundHidden, groundDarkTop, groundHidden, groundDarkSide, groundHidden);
			}
		}
	}
}

void RaylibGraphic::drawWalls() {
	for (int level = 0; level < 3; level++) {
		float yPos = (level) * cubeSize;
		
		for (int x = -1; x <= gridWidth; x++) {
			// Top wall
			Vector3 topPos = { x * cubeSize, yPos, -cubeSize };
			DrawCube(topPos, cubeSize, cubeSize, cubeSize * 2, wallColor);
			
			// Bottom wall
			Vector3 bottomPos = { x * cubeSize, yPos, gridHeight * cubeSize };
			DrawCube(bottomPos, cubeSize, cubeSize, cubeSize, wallColorFade);
		}
		
		for (int z = 0; z < gridHeight; z++) {
			// Left wall
			Vector3 leftPos = { -cubeSize, yPos, z * cubeSize };
			DrawCube(leftPos, cubeSize, cubeSize, cubeSize, wallColor);
			
			// Right wall
			Vector3 rightPos = { gridWidth * cubeSize, yPos, z * cubeSize };
			DrawCube(rightPos, cubeSize, cubeSize, cubeSize, wallColor);
		}
	}
}

void RaylibGraphic::drawSnake(const Snake* snake) {
	float yPos = cubeSize;
	
	// Calculate offset to match grid centering
	float offsetX = (gridWidth * cubeSize) / 2.0f;
	float offsetZ = (gridHeight * cubeSize) / 2.0f;
	
	for (int i = 0; i < snake->getLength(); i++) {
		const Vec2& segment = snake->getSegments()[i];
		
		Vector3 position = {
			segment.x * cubeSize - offsetX,
			yPos,
			segment.y * cubeSize - offsetZ
		};
		
		// Head is full size, body is 80% size
		float size = (i == 0) ? cubeSize : cubeSize * 0.8f;
		if (i > 0) position.y *= 0.8f;  // Adjust Y position for body
		
		// Checkerboard pattern for all segments
		if (i % 2 == 0) {
			drawCubeCustomFaces(position, size, size, size,
			                    snakeLightFront, snakeHidden, snakeLightTop, snakeHidden, snakeLightSide, snakeHidden);
		} else {
			drawCubeCustomFaces(position, size, size, size,
			                    snakeDarkFront, snakeHidden, snakeDarkTop, snakeHidden, snakeDarkSide, snakeHidden);
		}
	}
}

void RaylibGraphic::drawFood(const Food* food) {
	float yPos = cubeSize;
	
	// Calculate offset to match grid centering
	float offsetX = (gridWidth * cubeSize) / 2.0f;
	float offsetZ = (gridHeight * cubeSize) / 2.0f;
	
	Vec2 foodPos = food->getPosition();
	Vector3 position = {
		foodPos.x * cubeSize - offsetX,
		yPos,
		foodPos.y * cubeSize - offsetZ
	};
	
	// Pulsing effect using controlled time (freezes when paused)
	float pulse = 1.0f + sinf(accumulatedTime * 3.0f) * 0.1f;

	drawCubeCustomFaces(position, cubeSize * 0.7f * pulse, cubeSize * 0.7f * pulse, cubeSize * 0.7f * pulse,
						foodFront, foodHidden, foodTop, foodHidden, foodSide, foodHidden);
}

void RaylibGraphic::drawNoiseGrain() {
	DrawTextureEx(grainTextures[currentGrainFrame], (Vector2){ 0.0f, 0.0f }, 0.0f, 1.0f, (Color){ 255, 255, 255, 20 });
}

void RaylibGraphic::render(const GameState& state, float deltaTime){
	camera.fovy = customFov;
	if (!state.isPaused) {
        accumulatedTime += deltaTime;
    }
	
	// Update film grain pattern at regular intervals
	grainFrameTimer += deltaTime;
	if (grainFrameTimer >= grainFrameInterval) {
		grainFrameTimer = 0.0f;
		currentGrainFrame = GetRandomValue(0, GRAIN_TEXTURE_COUNT - 1);
	}

	BeginDrawing();
	ClearBackground(customBlack);
	
	BeginMode3D(camera);
	
	drawGroundPlane();
	//drawWalls();
	drawSnake(&state.snake);
	drawFood(&state.food);

	// DEBUG
	//DrawGrid(gridWidth, cubeSize);
	
	EndMode3D();
	
	DrawText("Press 1/2/3 to switch libraries", 10, 10, 20, customWhite);
	DrawText("Arrow keys to move, Q/ESC to quit", 10, 35, 20, customWhite);
	DrawFPS(screenWidth - 95, 10);

	if (state.isPaused) {
        //DrawOutlinedText("PAUSED", screenWidth / 2 - 60, screenHeight / 2, 40, customWhite, 1, customBlack);
		DrawText("PAUSED", screenWidth / 2 - 60, screenHeight / 2, 40, customBlack);
    }
	
	// Post Processing
	drawNoiseGrain();
	
	EndDrawing();
}

void RaylibGraphic::drawInstructions() {
	Vector3 textPosition = { 0.0f, 0.0f, 0.0f };
	float fontSize = 2.0f;
	float fontSpacing = 0.20f;
	float lineSpacing = 1.0f;
	
	DrawText3D(customFont, "[ ENTER ]              START", textPosition, fontSize, fontSpacing, lineSpacing, false, customWhite);
	DrawText3D(customFont, "          ············      ", textPosition, fontSize, fontSpacing, lineSpacing, false, customGray);
	
	textPosition.x += fontSize + lineSpacing + 1.0f;
	DrawText3D(customFont, "[ ↑ ↓ ← → ]             MOVE", textPosition, fontSize, fontSpacing, lineSpacing, false, customWhite);
	DrawText3D(customFont, "            ············     ", textPosition, fontSize, fontSpacing, lineSpacing, false, customGray);
	
	textPosition.x += fontSize + lineSpacing + 1.0f;
	DrawText3D(customFont, "[ 1   2   3 ]         TRAVEL", textPosition, fontSize, fontSpacing, lineSpacing, false, customWhite);
	DrawText3D(customFont, "    /   /     ·······       ", textPosition, fontSize, fontSpacing, lineSpacing, false, customGray);
	
	textPosition.x += fontSize + lineSpacing + 1.0f;
	DrawText3D(customFont, "[ Q   ESC ] ··········· QUIT", textPosition, fontSize, fontSpacing, lineSpacing, false, customWhite);
	DrawText3D(customFont, "    /       ···········     ", textPosition, fontSize, fontSpacing, lineSpacing, false, customGray);
}

void RaylibGraphic::renderMenu(const GameState& state, float deltaTime) {
	if (!state.isPaused) {
        accumulatedTime += deltaTime;
    }
	
	// Update film grain pattern at regular intervals
	grainFrameTimer += deltaTime;
	if (grainFrameTimer >= grainFrameInterval) {
		grainFrameTimer = 0.0f;
		currentGrainFrame = GetRandomValue(0, GRAIN_TEXTURE_COUNT - 1);
	}
	
	BeginDrawing();
	ClearBackground(customBlack);

	BeginMode3D(camera);
	
	titleHandler->drawTitle();
	textRenderer->drawInstructions();

	EndMode3D();

	drawNoiseGrain();
	
	EndDrawing();
}

void RaylibGraphic::renderGameOver(const GameState& state, float deltaTime) {
	// Update film grain pattern at regular intervals
	grainFrameTimer += deltaTime;
	if (grainFrameTimer >= grainFrameInterval) {
		grainFrameTimer = 0.0f;
		currentGrainFrame = GetRandomValue(0, GRAIN_TEXTURE_COUNT - 1);
	}
	
	BeginDrawing();
	ClearBackground(customBlack);
	
	// TODO: Implement proper Raylib game over screen
	DrawText("GAME OVER", screenWidth/2 - 150, screenHeight/2 - 50, 50, customWhite);
	
	char scoreText[50];
	snprintf(scoreText, sizeof(scoreText), "Score: %d", state.score);
	DrawText(scoreText, screenWidth/2 - 80, screenHeight/2 + 20, 30, customWhite);
	DrawText("Press ENTER to restart", screenWidth/2 - 150, screenHeight/2 + 80, 25, customWhite);
	
	drawNoiseGrain();
	
	EndDrawing();
}

void RaylibGraphic::DrawTextCodepoint3D(Font font, int codepoint, Vector3 position, float fontSize, bool backface, Color tint) {
	// Character index position in sprite font
	int index = GetGlyphIndex(font, codepoint);
	float scale = fontSize / (float)font.baseSize;
	
	// Character destination rectangle on screen
	position.x += (float)(font.glyphs[index].offsetX - font.glyphPadding) * scale;
	position.z += (float)(font.glyphs[index].offsetY - font.glyphPadding) * scale;
	
	// Character source rectangle from font texture atlas
	Rectangle srcRec = { 
		font.recs[index].x - (float)font.glyphPadding, 
		font.recs[index].y - (float)font.glyphPadding,
		font.recs[index].width + 2.0f * font.glyphPadding, 
		font.recs[index].height + 2.0f * font.glyphPadding 
	};
	
	float width = (float)(font.recs[index].width + 2.0f * font.glyphPadding) * scale;
	float height = (float)(font.recs[index].height + 2.0f * font.glyphPadding) * scale;
	
	if (font.texture.id > 0) {
		const float x = 0.0f;
		const float y = 0.0f;
		const float z = 0.0f;
		
		// Normalized texture coordinates of the glyph inside the font texture (0.0f -> 1.0f)
		const float tx = srcRec.x / font.texture.width;
		const float ty = srcRec.y / font.texture.height;
		const float tw = (srcRec.x + srcRec.width) / font.texture.width;
		const float th = (srcRec.y + srcRec.height) / font.texture.height;
		
		rlCheckRenderBatchLimit(4 + 4 * backface);
		rlSetTexture(font.texture.id);
		
		rlPushMatrix();
		rlTranslatef(position.x, position.y, position.z);
		
		rlBegin(RL_QUADS);
		rlColor4ub(tint.r, tint.g, tint.b, tint.a);
		
		// Front Face
		rlNormal3f(0.0f, 1.0f, 0.0f);
		rlTexCoord2f(tx, ty); rlVertex3f(x, y, z);
		rlTexCoord2f(tx, th); rlVertex3f(x, y, z + height);
		rlTexCoord2f(tw, th); rlVertex3f(x + width, y, z + height);
		rlTexCoord2f(tw, ty); rlVertex3f(x + width, y, z);
		
		if (backface) {
			// Back Face
			rlNormal3f(0.0f, -1.0f, 0.0f);
			rlTexCoord2f(tx, ty); rlVertex3f(x, y, z);
			rlTexCoord2f(tw, ty); rlVertex3f(x + width, y, z);
			rlTexCoord2f(tw, th); rlVertex3f(x + width, y, z + height);
			rlTexCoord2f(tx, th); rlVertex3f(x, y, z + height);
		}
		rlEnd();
		rlPopMatrix();
		
		rlSetTexture(0);
	}
}

void RaylibGraphic::DrawText3D(Font font, const char *text, Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, Color tint) {
    int length = TextLength(text);
    
    float textOffsetY = cubeSize * 7;
    float textOffsetX = -cubeSize * 7;
    
    float scale = fontSize / (float)font.baseSize;
    
    rlPushMatrix();
    rlTranslatef(position.x, position.y, position.z);
    rlRotatef(+90.0f, 0.0f, 1.0f, 0.0f);
    
    for (int i = 0; i < length;) {
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);
        
        if (codepoint == 0x3f) codepointByteCount = 1;
        
        if (codepoint == '\n') {
            textOffsetY += fontSize + lineSpacing;
            textOffsetX = 0.0f;
        } else {
            if ((codepoint != ' ') && (codepoint != '\t')) {
                DrawTextCodepoint3D(font, codepoint, 
                    (Vector3){ textOffsetX, 0.0f, textOffsetY },
                    fontSize, backface, tint);
            }
            
            if (font.glyphs[index].advanceX == 0) {
                textOffsetX += (float)font.recs[index].width * scale + fontSpacing;
            } else {
                textOffsetX += (float)font.glyphs[index].advanceX * scale + fontSpacing;
            }
        }
        
        i += codepointByteCount;
    }
    
    rlPopMatrix();
}

Input RaylibGraphic::pollInput() {
	if (IsKeyPressed(KEY_UP))		return Input::Up;
	if (IsKeyPressed(KEY_DOWN))		return Input::Down;
	if (IsKeyPressed(KEY_LEFT))		return Input::Left;
	if (IsKeyPressed(KEY_RIGHT))	return Input::Right;
	if (IsKeyPressed(KEY_Q))		return Input::Quit;
	if (IsKeyPressed(KEY_ESCAPE))	return Input::Quit;
	if (IsKeyPressed(KEY_ONE))		return Input::SwitchLib1;
	if (IsKeyPressed(KEY_TWO))		return Input::SwitchLib2;
	if (IsKeyPressed(KEY_THREE))	return Input::SwitchLib3;
	if (IsKeyPressed(KEY_SPACE))	return Input::Pause;
	if (IsKeyPressed(KEY_ENTER))	return Input::Enter;
	if (IsKeyPressed(KEY_KP_ENTER))	return Input::Enter;
	
	if (WindowShouldClose())		return Input::Quit;
	
	return Input::None;
}

extern "C" IGraphic* createGraphic() {
	return new RaylibGraphic();
}

extern "C" void destroyGraphic(IGraphic* g) {
	delete g;
}