#include "Engine/Math/UVector4.hpp"


UVector4::UVector4()
	: x(0)
	, y(0)
	, z(0)
	, w(0)
{

}

UVector4::UVector4(unsigned int initialX, unsigned int initialY, unsigned int initalZ, unsigned int initalW)
	: x(initialX)
	, y(initialY)
	, z(initalZ)
	, w(initalW)
{

}

UVector4::UVector4(const UVector4& copy)
	: x(copy.x), y(copy.y), z(copy.z), w(copy.w)
{

}
