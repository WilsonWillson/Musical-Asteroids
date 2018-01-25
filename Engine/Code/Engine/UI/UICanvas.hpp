#include "Engine/UI/UIElement.hpp"

class DX11Texture2D;

class UICanvas : public UIElement
{
public:
	UICanvas();
	~UICanvas();

	void SetReferenceResolution(float resolution);

public:
	DX11Texture2D*  texture;
	float			referenceResolution;

};