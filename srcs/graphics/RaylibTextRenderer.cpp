#include "../../incs/RaylibTextRenderer.hpp"
#include "../../incs/RaylibGraphic.hpp"
#include <rlgl.h>

RaylibTextRenderer::RaylibTextRenderer(RaylibGraphic& parent) : graphic(parent) {
	loadFont();
}

RaylibTextRenderer::~RaylibTextRenderer() {
	UnloadFont(customFont);
}

void RaylibTextRenderer::loadFont() {
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
	
	customFont = LoadFontEx("fonts/JetBrainsMono-VariableFont_wght.ttf", 64, codepoints, codepointCount);
	delete[] codepoints;
}

void RaylibTextRenderer::drawInstructions() {
	Vector3 textPosition = { 0.0f, 0.0f, 0.0f };
	float fontSize = 2.0f;
	float fontSpacing = 0.20f;
	float lineSpacing = 1.0f;
	
	DrawText3D(customFont, "[ ENTER ]              START", textPosition, fontSize, fontSpacing, lineSpacing, false, graphic.customWhite);
	DrawText3D(customFont, "          ············      ", textPosition, fontSize, fontSpacing, lineSpacing, false, graphic.customGray);
	
	textPosition.x += fontSize + lineSpacing + 1.0f;
	DrawText3D(customFont, "[ ↑ ↓ ← → ]             MOVE", textPosition, fontSize, fontSpacing, lineSpacing, false, graphic.customWhite);
	DrawText3D(customFont, "            ············     ", textPosition, fontSize, fontSpacing, lineSpacing, false, graphic.customGray);
	
	textPosition.x += fontSize + lineSpacing + 1.0f;
	DrawText3D(customFont, "[ 1   2   3 ]         TRAVEL", textPosition, fontSize, fontSpacing, lineSpacing, false, graphic.customWhite);
	DrawText3D(customFont, "    /   /     ·······       ", textPosition, fontSize, fontSpacing, lineSpacing, false, graphic.customGray);
	
	textPosition.x += fontSize + lineSpacing + 1.0f;
	DrawText3D(customFont, "[ Q   ESC ] ··········· QUIT", textPosition, fontSize, fontSpacing, lineSpacing, false, graphic.customWhite);
	DrawText3D(customFont, "    /       ···········     ", textPosition, fontSize, fontSpacing, lineSpacing, false, graphic.customGray);
}

void RaylibTextRenderer::DrawTextCodepoint3D(Font font, int codepoint, Vector3 position, float fontSize, bool backface, Color tint) {
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

void RaylibTextRenderer::DrawText3D(Font font, const char *text, Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, Color tint) {
    int length = TextLength(text);
    
    float textOffsetY = graphic.cubeSize * 7;
    float textOffsetX = -graphic.cubeSize * 7;
    
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