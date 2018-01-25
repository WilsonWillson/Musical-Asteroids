#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/IntVector2.hpp"
#include <string>
class SpriteSheet
{
	friend class SpriteAnimation;
	friend class BitMapFont;

public:
	SpriteSheet(Texture* texture, int tilesWide, int tilesHigh);
	AABB2 GetTexCoordsForSpriteCoords(int spriteX, int spriteY) const; // mostly for atlases
	AABB2 GetTexCoordsForSpriteIndex(int spriteIndex) const; // mostly for sprite animations
	int GetNumSprites() const;
	Texture* GetTexture() const { return m_spriteSheetTexture; }
	Texture* 	m_spriteSheetTexture;	// Image with grid-based layout of sub-images
private:
	
	IntVector2	m_spriteLayout;		// # of sprites across, and down, on the sheet
};
