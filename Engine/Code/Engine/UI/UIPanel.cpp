#include "Engine/UI/UIPanel.hpp"
#include "Engine/RHI/SimpleRenderer.hpp"
#include "Engine/UI/UISprite.hpp"

UIPanel::UIPanel()
{

}

UIPanel::~UIPanel()
{

}

void UIPanel::RenderPanel(SimpleRenderer* renderer)
{
	for (int i = children.size() - 1; i > -1; i--)
	{
		AABB2 bounds = children[i]->get_element_bounds();
		UISprite* sprite = ((UISprite*)children[i]);
		renderer->SetTexture2d((sprite->texture));
		renderer->draw_quad2d(bounds, AABB2(0.f, 0.0f, 1.f, 1.f), rgba_fl(1, 0, 0, 1));
	}


}

