#pragma once
#include "Engine/Math/Matrix4.hpp"
#include <vector>
class MatrixStack
{

public:
	MatrixStack();
	~MatrixStack();

	//-----------------Matrix Manipulation------------------------------------
	void push_direct(Matrix4 transform);
	void push(Matrix4 geo_trans);
	void pop();

	//-----------------Getters/Setters------------------------------------
	Matrix4 get_top();

public:
	Matrix4 m_topMatrix;
	std::vector<Matrix4> m_stack;
};
