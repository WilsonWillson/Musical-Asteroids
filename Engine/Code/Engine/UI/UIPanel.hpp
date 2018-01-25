#include "Engine/UI/UIElement.hpp"


class SimpleRenderer;

class UIPanel : public UIElement
{
public:
	UIPanel();
	~UIPanel();

public:
	void RenderPanel(SimpleRenderer* renderer);
};