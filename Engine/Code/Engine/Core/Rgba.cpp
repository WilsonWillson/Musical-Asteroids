#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/MathUtils.hpp"

Rgba::Rgba()
	:r(255),
	g(255),
	b(255),
	a(255)
{

}

Rgba::Rgba(unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte /*= 255*/)
	:r(redByte),
	g(greenByte),
	b(blueByte),
	a(alphaByte)
{

}


Rgba::~Rgba()
{

}
void Rgba::SetAsBytes(unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte /*= 255*/)
{
	r = redByte;
	g = greenByte;
	b = blueByte;
	a= alphaByte;
}

void Rgba::SetAsFloats(float normalizedRed, float normalizedGreen, float normalizedBlue, float normalizedAlpha)
{
	r = ClampToChar(normalizedRed);
	g = ClampToChar(normalizedGreen);
	b = ClampToChar(normalizedBlue);
	a = ClampToChar(normalizedAlpha);
}


void Rgba::GetAsFloats(float& out_normalizedRed, float& out_normalizedGreen, float& out_normalizedBlue, float& out_normalizedAlpha) 
{
	out_normalizedRed = ClampToChar(r);
	out_normalizedGreen = ClampToChar(g);
	out_normalizedBlue = ClampToChar(b);
	out_normalizedAlpha = ClampToChar(a);
}

void Rgba::ScaleRGB(float rgbScale)
{
	r *= (unsigned char)rgbScale;
	g *= (unsigned char)rgbScale;
	b *= (unsigned char)rgbScale;
}

void Rgba::ScaleAlpha(float alphaScale)
{
	a *= (unsigned char)alphaScale;
}

unsigned char Rgba::ClampToChar(float floatColor)
{
	int convertToWholeNumber = int(floatColor * 255);
	if (convertToWholeNumber > 255)
		convertToWholeNumber = 255;
	if (convertToWholeNumber < 0)
		convertToWholeNumber = 0;
	return (unsigned char)convertToWholeNumber;
}

void Rgba::setToRandomColor()
{
	r = ClampToChar(GetRandomFloatInRange(0,1));
	g = ClampToChar(GetRandomFloatInRange(0, 1));
	b = ClampToChar(GetRandomFloatInRange(0, 1));
	a = 1;
}

void Rgba::MakeColorDarker(float percent)
{
	r = (unsigned char) (r * percent);
	g = (unsigned char)(g* percent);
	b = (unsigned char)(b* percent);
}

