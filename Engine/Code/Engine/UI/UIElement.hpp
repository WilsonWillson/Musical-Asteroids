
#ifndef MY_HEADER_FILE_
#define MY_HEADER_FILE_
#include"Engine/Math/Vector2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Matrix4.hpp"
#include <vector>

struct ui_metric_t
{
	Vector2 ratio;
	Vector2 unit;
};




class UIElement
{
public:
	UIElement();
	~UIElement();
	Vector2 get_size();
	unsigned int getSpecificChildIndex(UIElement* child);
	unsigned int getSpecificChildIndexOfParent(UIElement* child);
	UIElement* get_parent();
	AABB2 get_element_bounds();
	AABB2 getLocalBounds();
	Vector2 GetRelativePosition();
	Vector2 GetLocalPosition();
	Matrix4 GetLocalTransform();
	Matrix4 GetWorldTransform();


	void set_position(Vector2 ratio, Vector2 unit);
	void set_size(Vector2 sizeToSet);
	void set_size_ratio(Vector2 size_ratio);
	void set_pivot(Vector2 ratio_in_respect_to_my_bounds);

	UIElement* add_child(UIElement*);
	void RemoveChildAndAllDescendents(UIElement*);
	void remove_self_from_parent();
		
	template <typename T>
	T* create_child()
	{
		return (T*)add_child(new T());
	}

private:
	void calculate_boundsRelativeToYourParentsPosition();

public:
	ui_metric_t position;
	Vector2 pivot; // ratio of my bounds
	ui_metric_t size;

	AABB2 bounds;

	UIElement *parent;
	std::vector<UIElement*> children;
};
#endif