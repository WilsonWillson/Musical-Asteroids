#pragma once


//-----------------------------------------------------------------------------------------------
class UVector3
{
public:

	static const  UVector3 ZERO;
	UVector3();
	UVector3(unsigned int initialX, unsigned int initialY, unsigned int initalZ);
	UVector3(const  UVector3& copy);

public:
	unsigned int x;
	unsigned int y;
	unsigned int z;
};
