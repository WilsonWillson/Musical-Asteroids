#pragma once
#if !defined( __CORE_IMAGE__ )
#define  __CORE_IMAGE__
#include "Engine/Core/Rgba.hpp"

enum eImageFormat : unsigned int
{
	IMAGEFORMAT_RGBA8, // only supporting this one for class - up to you if you want to support more
	IMAGEFORMAT_D24S8,
};

//-----------------------------------------------------------------------------------------------
class Image
{
public:
	Image();

	~Image();

	bool LoadFromFile(char const *filename);
	void DestroyOldImage();

	inline bool IsValid() const { return nullptr != m_buffer; }

	inline unsigned int GetPixelSize() const { return m_bpp; }
	inline unsigned int GetWidth() const { return m_width; }
	inline unsigned int GetHeight() const { return m_height; }
	inline unsigned int  GetStride() const { return m_width * m_bpp; }
	inline unsigned int GetSlice() const { return GetStride() * m_height; }

public:
	unsigned char* m_buffer;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_bpp;

	eImageFormat m_format;

	bool create_clear(unsigned int w, unsigned int height, rgba_fl const color);
};
#endif 
