#pragma once
#include <raylib.h>
#include "DataStructs.hpp"

// Forward declaration to avoid circular dependency
class RaylibGraphic;

class RaylibTextRenderer {
	private:
		RaylibGraphic& graphic;
		Font customFont;  // Font with Unicode support
		
		void loadFont();

	public:
		RaylibTextRenderer() = delete;
		RaylibTextRenderer(RaylibGraphic& parent);
		~RaylibTextRenderer();
		RaylibTextRenderer (const RaylibTextRenderer &other) = delete;
		RaylibTextRenderer &operator=(const RaylibTextRenderer &other) = delete;
		
		Font& getFont() { return customFont; }
		
		void drawInstructions();
		void DrawText3D(Font font, const char *text, Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, Color tint, float YRotation, float YOffset, float XOffset);
		void DrawTextCodepoint3D(Font font, int codepoint, Vector3 position, float fontSize, bool backface, Color tint);

		void drawRetry(const GameState& state);
};