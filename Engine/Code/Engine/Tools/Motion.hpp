#pragma once
#include "Engine/Tools/Pose.hpp"
#include "Engine/Tools/SkeletonInstance.hpp"
#include <string>
#include <vector>
class Pose;
class SkeletonInstance;

class Motion
{
public:
	Motion(Skeleton* skeleton);
	Motion();
	~Motion();


	void set_name(char const * motion_name);
	void set_duration(float duration_fl);
	Pose* get_pose(unsigned int frame_idx);
	float get_duration();
	int getCurrentPoseBasedOnTime(float time);
	void evaluate(Pose** currentPose, float  time);

public:
	SkeletonInstance* m_skeletonInstance;
	std::string m_name;
	float m_framerate;
	float m_localTime;
	std::vector<Pose*> m_poses;
	Pose* m_CurrentPose;
	int m_currentPoseNumber;
	bool m_actionTaken;
};