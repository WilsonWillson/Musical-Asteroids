#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/IntVector2.hpp"
#include <string>

SpriteSheet::SpriteSheet(Texture* texture, int tilesWide, int tilesHigh)
{
	m_spriteSheetTexture = texture;
	m_spriteLayout.x = tilesWide;
	m_spriteLayout.y = tilesHigh;
}

AABB2 SpriteSheet::GetTexCoordsForSpriteCoords(int spriteX, int spriteY) const
{
	AABB2 texCoords;
	texCoords.m_mins.x = (float)spriteX / (float)m_spriteLayout.x;
	texCoords.m_mins.y = (float)spriteY / (float)m_spriteLayout.y;
	texCoords.m_maxs.x = (float)(spriteX + 1 ) / (float)m_spriteLayout.x;
	texCoords.m_maxs.y = (float)(spriteY + 1) / (float)m_spriteLayout.y;
	const float TINY_OFFSET_AMOUNT = (1.f / 8192.f);
	texCoords.m_mins.x += TINY_OFFSET_AMOUNT;
	texCoords.m_mins.y += TINY_OFFSET_AMOUNT;
	texCoords.m_maxs.x -= TINY_OFFSET_AMOUNT;
	texCoords.m_maxs.y -= TINY_OFFSET_AMOUNT;
	return texCoords;
}

AABB2 SpriteSheet::GetTexCoordsForSpriteIndex(int spriteIndex) const
{
	int spriteY = spriteIndex / m_spriteLayout.x;
	int spriteX = spriteIndex % m_spriteLayout.x;
	return GetTexCoordsForSpriteCoords(spriteX, spriteY);
}

int SpriteSheet::GetNumSprites() const
{
	return (m_spriteLayout.x * m_spriteLayout.y) - 1;
}
