#include "Engine/Renderer/BitMapFont.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/Renderer.hpp"

BitMapFont::BitMapFont()
{

}


AABB2 BitMapFont::GetTexCoordsForGlyph(int glyphUnicode) const
{
	return m_spriteSheet->GetTexCoordsForSpriteIndex(glyphUnicode);
}