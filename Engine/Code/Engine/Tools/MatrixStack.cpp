#include "Engine/Tools/MatrixStack.hpp"

MatrixStack::MatrixStack()
{
 	Matrix4 newMatrix;
	m_topMatrix = newMatrix;
	m_stack.push_back(m_topMatrix);
}

MatrixStack::~MatrixStack()
{

}

//-----------------Matrix Manipulation------------------------------------
void MatrixStack::push_direct(Matrix4 transform)
{
	m_stack.push_back(m_topMatrix);
	m_topMatrix = transform;
}

void MatrixStack::push(Matrix4 geo_trans)
{
	m_topMatrix = m_topMatrix *  geo_trans;
}

void MatrixStack::pop()
{
	m_topMatrix = m_stack.back();
	m_stack.pop_back();
}

//----------------Getters/Setters-------------------------------------
Matrix4 MatrixStack::get_top()
{
	return m_topMatrix;
}

