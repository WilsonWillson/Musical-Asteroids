#include "Engine/Core/Image.hpp"


#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_PNM
#include "ThirdParty/stb/stb_image.h"


//-------------------------Constructor/Decounstructor-----------------------------------------------
Image::Image() : 
m_buffer(nullptr),
m_width(0),
m_height(0),
m_bpp(0)
{}

Image::~Image()
{
	DestroyOldImage();
}

void Image::DestroyOldImage()
{
	if (nullptr != m_buffer) {
		::free(m_buffer);
	}
}

bool Image::create_clear(unsigned int w, unsigned int h, rgba_fl color)
{
	DestroyOldImage();

	m_width = w;
	m_height = h;
	m_format = IMAGEFORMAT_RGBA8;
	m_bpp = 4;

	unsigned int size = m_width * m_height * m_bpp;
	m_buffer = (unsigned char *) ::malloc(size);
	rgba_fl c = color;

	//rgba_fl  *colors = (rgba_fl *)m_buffer;
	unsigned int count = m_width * m_height * m_bpp;

	Rgba charColor;
	charColor.SetAsFloats(c.r, c.g, c.b, c.a);
	for (unsigned int i = 0; i < count; i +=4) {
		m_buffer[i] = charColor.r;
		m_buffer[i+1] = charColor.g;
		m_buffer[i+2] = charColor.b;
		m_buffer[i+3] = charColor.a;
	}

	return true;
}

//------------------------------------------------------------------------
bool Image::LoadFromFile(char const *filename)
{
	DestroyOldImage();

	int w;
	int h;
	int comp;

	unsigned char *data = stbi_load(filename, &w, &h, &comp, 0);
	if (data != nullptr)
	{
		if (comp == 3) 
		{
			unsigned char *new_buffer = stbi__convert_format(data, comp, 4, w, h);
			data = new_buffer;
		}

		m_buffer = (unsigned char *)data;
		m_width = w;
		m_height = h;
		m_bpp = 4;
		m_format = IMAGEFORMAT_RGBA8;
		return true;
	}

	return false;
}