#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"


class BitMapFont
{
	
public:

	BitMapFont();
	BitMapFont(const std::string& bitmapFontName); // must be constructed by Renderer

	AABB2 GetTexCoordsForGlyph(int glyphUnicode) const; // pass ‘A’ or 65 for A, etc.

public:
	//float GetGlyphAspect(int glyphUnicode) const { return 1.0f; } // used more later
	SpriteSheet* m_spriteSheet; // used internally; assumed to be a 16x16 glyph sheet
};