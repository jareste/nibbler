#pragma once
#include <raylib.h>
#include "DataStructs.hpp"

// Forward declaration to avoid circular dependency
class RaylibGraphic;

class RaylibTextRenderer {
	private:
		RaylibGraphic&	graphic;
		Font			customFont;
		
		void loadFont();

	public:
		RaylibTextRenderer() = delete;
		RaylibTextRenderer(RaylibGraphic& parent);
		RaylibTextRenderer(const RaylibTextRenderer &other) = delete;
		RaylibTextRenderer &operator=(const RaylibTextRenderer &other) = delete;

		~RaylibTextRenderer();
		
		Font& getFont();
		
		void drawModes(const GameState &state);
		void drawInstructions(const GameState &state);
		void DrawText3D(Font font, const char *text, Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, Color tint, float YRotation, float YOffset, float XOffset);
		void DrawTextCodepoint3D(Font font, int codepoint, Vector3 position, float fontSize, bool backface, Color tint);

		void drawWinner(const GameState& state);
		void drawRetry(const GameState& state);
};