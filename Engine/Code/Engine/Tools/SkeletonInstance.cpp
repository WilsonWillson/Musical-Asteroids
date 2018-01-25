#include "Engine/Tools/SkeletonInstance.hpp"

#include "Engine/Tools/Skeleton.hpp"
#include "Engine/Tools/Pose.hpp"
#include "Engine/Tools/Motion.hpp"
#include <string>

Matrix4 SkeletonInstance::get_joint_global_transform(unsigned int jointIndex) const
{
	Matrix4 currentTransform = m_currentPose->local_transforms[jointIndex];

	int currentJoinIndex = get_joint_parent(jointIndex);
	while (currentJoinIndex != -1)
	{
		currentTransform = currentTransform * m_currentPose->local_transforms[currentJoinIndex];
		currentJoinIndex = get_joint_parent(currentJoinIndex);
	}
	return currentTransform;
}

int SkeletonInstance::get_joint_parent(unsigned int jointIndex) const
{
	std::string parentBoneName = skeleton->m_skeleton[jointIndex]->parentBoneName;
	for (unsigned int index = 0; index < skeleton->m_skeleton.size(); index++)
	{
		if (skeleton->m_skeleton[index]->boneName == parentBoneName)
			return index;
	}
	return -1;
}

void SkeletonInstance::apply_motion(Motion *motion, float time)
{
	  motion->evaluate(&m_currentPose, time);
}

