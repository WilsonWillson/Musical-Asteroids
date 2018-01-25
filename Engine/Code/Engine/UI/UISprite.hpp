#include "Engine/UI/UIElement.hpp"

class DX11Texture2D;
class SimpleRenderer;

enum FillType
{
	UI_FILL_CROP
};


class UISprite : public UIElement
{
public:
	UISprite();
	~UISprite();

	void SetSprite(char const * fileName, SimpleRenderer* renderer);
	void SetFill(FillType fillType);
public:
	DX11Texture2D*  texture;
	FillType typeOfFill;
};