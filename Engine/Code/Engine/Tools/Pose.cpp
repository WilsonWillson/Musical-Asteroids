#include "Engine/Tools/Pose.hpp"

Pose::Pose()
{

}

Pose::Pose(const Skeleton* currentSkeleton)
{
	for (unsigned int boneIndex = 0; boneIndex < currentSkeleton->m_skeleton.size(); boneIndex++)
	{
		Matrix4 mb;
		local_transforms.push_back(mb);
	}
}

Pose::~Pose()
{

}
