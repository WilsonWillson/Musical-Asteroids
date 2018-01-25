#pragma once

#include "Engine/Math/Matrix4.hpp"
#include "Engine/Math/Vector3.hpp"
#include <vector>
// Interface a skeleton I'll be using - up to you to figure out 
// how you want to store this.  
// 
// End of the day, this is a collection of global transforms in a heirachy 
// that can be referenced by name.

struct BoneStructure
{
	Matrix4 m_transform;
	std::string boneName;
	std::string parentBoneName;
};

class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	// Reset the skeleton - clear out all bones that make up
	// this skeleton
	void clear();

	// Adds a joint.  Can be parented to another 
	// joint within this skeleton.
	void add_joint(char const *name,
		char const *parent_name,
		Matrix4 const &transform);

	// get number of joints/bones in this skeleton.
	unsigned int get_joint_count() const;

	// Get a joint index by name, returns
	// (uint)(-1) if it doesn't exist.
	unsigned int get_joint_index(char const *name);

	// Get the global transform for a joint.
	Matrix4 get_joint_transform(unsigned int joint_idx) const;
	Matrix4 get_joint_transform(char const *name) const;

	char const * get_joint_name(unsigned int joint_idx) const;
	std::string get_joint_parent(unsigned int joint_idx) const;
	unsigned int getParentJoint(unsigned int joint_idx) const;
public:
	// what data would you need to do this?
	std::vector<BoneStructure*> m_skeleton;
};