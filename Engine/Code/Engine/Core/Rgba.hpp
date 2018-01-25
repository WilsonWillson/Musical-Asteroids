#pragma once
#pragma warning(disable : 4201)


struct rgba_fl
{
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		struct  {
			float r;
			float g;
			float b;
			float a;
		};
		float data[4];
	};

	rgba_fl() {}
	rgba_fl(float const v)
		: r(v)
		, g(v)
		, b(v)
		, a(v) {}

	rgba_fl(unsigned int const rgba);
	rgba_fl(float _r, float _g, float _b, float _a)
		: r(_r)
		, g(_g)
		, b(_b)
		, a(_a) {}

	rgba_fl(float _r, float _g, float _b)
		: rgba_fl(_r, _g, _b, 1.0f) {}

	//rgba_fl(rgba_u32 const &c);

	inline void set(float pr, float pg, float pb, float pa)
	{
		this->r = pr;
		this->g = pg;
		this->b = pb;
		this->a = pa;
	}

	inline void set(float pr, float pg, float pb)
	{
		set(pr, pg, pb, 1.0f);
	}

	//------------------------------------------------------------------------
	// helpers
	//------------------------------------------------------------------------
	operator float const*const () const { return data; } // used for D3D methods that use color


														 // CONSTANTS //////////////////////////////////////////////////////////
	static rgba_fl const WHITE;
	static rgba_fl const BLACK;
	static rgba_fl const RED;
	static rgba_fl const GREEN;
	static rgba_fl const BLUE;
	static rgba_fl const YELLOW;
};



class Rgba
{
public:
	Rgba();
	~Rgba();

	Rgba(unsigned char redByte, unsigned char greenByte,
		unsigned char blueByte, unsigned char alphaByte = 255);

	void SetAsBytes(unsigned char redByte, unsigned char greenByte,
		unsigned char blueByte, unsigned char alphaByte = 255);

	void SetAsFloats(float normalizedRed, float normalizedGreen,
		float normalizedBlue, float normalizedAlpha = 1.0f);

	void GetAsFloats(float& out_normalizedRed, float& out_normalizedGreen,
		float& out_normalizedBlue, float& out_normalizedAlpha);

	void ScaleRGB(float rgbScale); // Scales (and clamps) RGB components, but not A

	void ScaleAlpha(float alphaScale); // Scales (and clamps) Alpha, RGB is untouched

	unsigned char ClampToChar(float floatColor);

	void setToRandomColor();
	void MakeColorDarker(float percent);
public:
	unsigned char r; // Red byte, 0-255
	unsigned char g; // Green byte, 0-255
	unsigned char b; // Blue byte, 0-255
	unsigned char a; // Alpha (opacity) byte, 0-255; 0 is transparent, 255 is opaque

	

};

