#pragma once

#include "Engine/Math/Matrix4.hpp"
class Pose;
class Skeleton;
class Motion;

class SkeletonInstance
{
public:
	Matrix4 get_joint_global_transform(unsigned int joint_idx) const;

	// returns INVALID_INDEX (-1) if no parent exists.
	// Use the skeleton to get this information.
	int get_joint_parent(unsigned int joint_idx) const;

	void apply_motion(Motion *motion, float time);

public:
	Skeleton *skeleton; // skeleton we're applying poses to.  Used for heirachy information.
	Pose* m_currentPose;  // my current skeletons pose.

	// Excercise:  How do we evaluate a global transform for this joint if the pose only contains 
    // local transforms?
    // Hint:  You will need "get_joint_parent" below.
};
