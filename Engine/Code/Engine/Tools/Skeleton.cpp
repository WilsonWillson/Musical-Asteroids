#include "Engine/Tools/Skeleton.hpp"

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
	clear();
}

void Skeleton::clear()
{
	m_skeleton.clear();
}

void Skeleton::add_joint(char const *name, char const *parent_name, Matrix4 const &transform)
{
	BoneStructure* newBone = new BoneStructure();
	newBone->boneName = name;
	if (parent_name == NULL)
		parent_name = "";
	newBone->parentBoneName = parent_name;
	newBone->m_transform = transform;
	m_skeleton.push_back(newBone);
}

unsigned int Skeleton::get_joint_count() const
{
	return m_skeleton.size();
}

unsigned int Skeleton::get_joint_index(char const *name)
{
	for (unsigned int jointIndex = 0; jointIndex < m_skeleton.size(); jointIndex++)
	{
		if (m_skeleton[jointIndex]->boneName == name)
			return jointIndex;
	}
	return (unsigned int)-1;
}

Matrix4 Skeleton::get_joint_transform(unsigned int jointIndex) const
{
	return m_skeleton[jointIndex]->m_transform;
}

Matrix4 Skeleton::get_joint_transform(char const *name) const
{
	Matrix4 mb;
	for (unsigned int jointIndex = 0; jointIndex < m_skeleton.size(); jointIndex++)
	{
		if (m_skeleton[jointIndex]->boneName == name)
			return m_skeleton[jointIndex]->m_transform;
	}
	return mb;
}
char const * Skeleton::get_joint_name(unsigned int jointIndex) const
{
			return m_skeleton[jointIndex]->boneName.c_str();
}

std::string Skeleton::get_joint_parent(unsigned int jointIndex) const
{
			return m_skeleton[jointIndex]->parentBoneName;
}

unsigned int Skeleton::getParentJoint(unsigned int jointIndex) const
{
	std::string name = m_skeleton[jointIndex]->parentBoneName;

		for (unsigned int parentJointIndex = 0; parentJointIndex < m_skeleton.size(); parentJointIndex++)
		{
			if (m_skeleton[parentJointIndex]->boneName == name)
				return parentJointIndex;
		}
		return (unsigned int)-1;
}
