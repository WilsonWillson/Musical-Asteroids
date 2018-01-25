#include "Engine/UI/UISprite.hpp"
#include "Engine/DX11/DX11Texture2D.hpp"
#include "Engine/RHI/SimpleRenderer.hpp"

UISprite::UISprite()
{

}

UISprite::~UISprite()
{

}


void UISprite::SetSprite(char const * fileName, SimpleRenderer* renderer)
{
	texture = new DX11Texture2D(renderer->m_device, fileName);
}

void UISprite::SetFill(FillType fillType)
{
	typeOfFill = fillType;
}
