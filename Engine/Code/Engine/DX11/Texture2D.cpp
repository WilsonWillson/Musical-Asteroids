#include "Engine/DX11/Texture2D.hpp"
Texture2D::Texture2D(RHIDevice *device, ID3D11Texture2D *tex)
{

}

Texture2D::~Texture2D()
{

}

unsigned int Texture2D::get_width()
{
	return width;
}

unsigned int Texture2D::get_height()
{
	return height;
}

void Texture2D::set_width(unsigned int targetWidth)
{
	width = targetWidth;
}

void Texture2D::set_height(unsigned int targetHeight)
{
	height = targetHeight;
}

