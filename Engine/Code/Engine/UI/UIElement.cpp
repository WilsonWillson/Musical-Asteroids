#include "Engine/UI/UIElement.hpp"


UIElement::UIElement()
{
	bounds = AABB2(0.f, 0.f);
	pivot = Vector2(0.f, 0.f);
	parent = nullptr;
	position.ratio = Vector2(0.0f, 0.0f);
	position.unit = Vector2(0.0f, 0.0f);

	size.ratio = Vector2(0.0f, 0.0f);
	size.unit = Vector2(0.0f, 0.0f);

}

UIElement::~UIElement()
{

}

Vector2 UIElement::get_size()
{
	return size.unit;
}
/*
Vector2 UIElement::get_sizeRelativeToYourParents()
{
	if(parent == nullptr)
		return size.unit;
	else
	{
		Vector2 getSize = parent->get_size() * size.ratio + size.unit;
		return getSize;
	}
}
*/
unsigned int UIElement::getSpecificChildIndex(UIElement* child)
{
	for (unsigned int j = 0; j < children.size(); j++)
	{
		if (children[j] == child)
			return j;
	}

	return -1;
}

unsigned int UIElement::getSpecificChildIndexOfParent(UIElement* child)
{
	for (unsigned int j = 0; j < parent->children.size(); j++)
	{
		if (parent->children[j] == child)
			return j;
	}

	return -1;
}

UIElement* UIElement::get_parent()
{
	return parent;
}

AABB2 UIElement::get_element_bounds()
{
	AABB2 bounds;
	Vector2 size = get_size();

	bounds.m_mins = pivot;
	bounds.m_maxs = pivot + size;
	return bounds;
}

AABB2 UIElement::getLocalBounds()
{
	AABB2 bounds;
	Vector2 size = get_size();
	Vector2 pivot_pos = pivot * size;

	bounds.m_mins = Vector2(0.f, 0.f);
	bounds.m_maxs = size;

	bounds.m_mins += (pivot_pos * -1);
	bounds.m_maxs += (pivot_pos * -1);

	return bounds;
}

//This is called GetLocalPosition in Forseth's code
Vector2 UIElement::GetRelativePosition()
{
	if (parent == nullptr)
		return pivot;
	else
	{
		Vector2 parent_bounds = parent->GetRelativePosition();
		Vector2 parent_size = parent->get_size();
		Vector2 pivot_position = parent_bounds + (parent_size * position.ratio + position.unit);

		Vector2 my_local_bounds = GetRelativePosition();
		my_local_bounds += pivot_position;

		return my_local_bounds;
	}
}

Vector2 UIElement::GetLocalPosition()
{
	return pivot;
}

Matrix4 UIElement::GetLocalTransform()
{
	Matrix4 identity;

	Matrix4 localTransform = identity.CreateTranslation(GetLocalPosition());

	return localTransform;
}

Matrix4 UIElement::GetWorldTransform()
{
	if (parent == nullptr)
		return GetLocalTransform();
	else
	{
		Matrix4 transform;
		transform = parent->GetWorldTransform();
		
		transform = transform * GetLocalTransform();
		return transform;
	}
}

void UIElement::set_position(Vector2 ratio, Vector2 unit)
{
	position.ratio = ratio;
	position.unit = unit;
}

void UIElement::set_size(Vector2 sizeToSet)
{
	size.unit = sizeToSet;
}

void UIElement::set_size_ratio(Vector2 size_ratio)
{
	size.ratio = size_ratio;
}

void UIElement::set_pivot(Vector2 ratio_in_respect_to_my_bounds)
{
	pivot = ratio_in_respect_to_my_bounds;
}

UIElement* UIElement::add_child(UIElement* child)
{
	child->parent = this;
	child->parent->children.push_back(child);
	return child;
}


//Remove Stuff
void UIElement::RemoveChildAndAllDescendents(UIElement* child)
{
	for (unsigned int i = 0; i < child->children.size(); i++)
	{
		if (child->children[i]->children.size() == 0)
		{
			child->children.erase(child->children.begin() + i);
		}
		else
		{
			RemoveChildAndAllDescendents(child->children[i]);
		}
	}
	child->remove_self_from_parent();
	
	delete child;
}

void UIElement::remove_self_from_parent()
{
	unsigned int index = getSpecificChildIndexOfParent(this);

	if (index != -1)
	{
		parent->children.erase(parent->children.begin() + index);
		return;
	}
	else
	{
		//Print error
	}
}



void UIElement::calculate_boundsRelativeToYourParentsPosition()
{
	Vector2 parent_size; //= AABB2::ThatContains(Vector2::ZERO, parent->get_size());

	// Absolute Positioning
	Vector2 relative_size = parent_size * size.ratio;
	Vector2 my_size = relative_size + size.unit;

	Vector2 relative_position = parent_size * position.ratio;
	Vector2 my_position = relative_position + position.unit;


}