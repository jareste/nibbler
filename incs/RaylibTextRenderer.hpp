#pragma once
#include <raylib.h>

// Forward declaration to avoid circular dependency
class RaylibGraphic;

class RaylibTextRenderer {
	private:
		RaylibGraphic& graphic;

	public:
		RaylibTextRenderer() = delete;
		RaylibTextRenderer(RaylibGraphic& parent);
		~RaylibTextRenderer() = default;
		RaylibTextRenderer (const RaylibTextRenderer &other) = delete;
		RaylibTextRenderer &operator=(const RaylibTextRenderer &other) = delete;
		
		void drawInstructions();
		void DrawText3D(Font font, const char *text, Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, Color tint);
		void DrawTextCodepoint3D(Font font, int codepoint, Vector3 position, float fontSize, bool backface, Color tint);
};