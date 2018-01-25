#include "Engine/Tools/Motion.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

Motion::Motion()
{
	m_name = "";
	m_framerate = 60;
	m_currentPoseNumber = 0;
	m_localTime = 0;
	m_CurrentPose = new Pose();
	m_skeletonInstance = new SkeletonInstance();
	m_skeletonInstance->m_currentPose = m_CurrentPose;
	m_skeletonInstance->skeleton = nullptr;
	m_actionTaken = false;
}


Motion::Motion(Skeleton* skeleton)
{
	m_name = "";
	m_framerate = 60;
	m_currentPoseNumber = 0;
	m_localTime = 0;
	m_CurrentPose = new Pose();
	m_skeletonInstance = new SkeletonInstance();
	m_skeletonInstance->m_currentPose = m_CurrentPose;
	m_skeletonInstance->skeleton = skeleton;
	m_actionTaken = false;
}

Motion::~Motion()
{

}

void Motion::set_name(char const * motion_name)
{
	m_name = motion_name;
}

void Motion::set_duration(float duration_fl)
{
	m_poses.resize((int)((unsigned int)ceil(duration_fl * m_framerate) + 1));
}

Pose* Motion::get_pose(unsigned int frame_idx)
{
	return m_poses[frame_idx];
}

float Motion::get_duration()
{
	return m_poses.size() / m_framerate;
}

int Motion::getCurrentPoseBasedOnTime(float time)
{
	float poseDuration = get_duration();

	if (m_localTime >= poseDuration)
	{
		m_localTime =  time;
		m_actionTaken = false;
	}
	

	float currentPercentInPose = m_localTime / poseDuration;

	int index = (int)ceil(m_poses.size() * currentPercentInPose);

	if (index == (int)m_poses.size())
	{
		index = m_poses.size()-1;
	}
	return index;
}

void Motion::evaluate(Pose** currentPose, float time)
{
	m_localTime += time;
	
	int index = getCurrentPoseBasedOnTime(time);
	m_CurrentPose = m_poses[index];

	currentPose = &m_CurrentPose;


	if (index > 150 && index < 155 && m_actionTaken == false)
	{
		DebuggerPrintf("Gun Shot");
		m_actionTaken = true;
	}
}
