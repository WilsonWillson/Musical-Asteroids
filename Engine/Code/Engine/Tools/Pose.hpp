#pragma once
#pragma once
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Tools/Skeleton.hpp"
#include <vector>

class Pose
{
public:
	Pose();
	Pose(const Skeleton* currentSkeleton);
	~Pose();

public:
	std::vector<Matrix4> local_transforms;
};