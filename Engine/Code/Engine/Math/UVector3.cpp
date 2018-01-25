#include "Engine/Math/UVector3.hpp"


UVector3::UVector3()
	: x(0)
	, y(0)
	, z(0)
{

}

UVector3::UVector3(unsigned int initialX, unsigned int initialY, unsigned int initalZ)
	: x(initialX)
	, y(initialY)
	, z(initalZ)
{

}

UVector3::UVector3(const UVector3& copy)
	: x(copy.x), y(copy.y), z(copy.z)
{

}
