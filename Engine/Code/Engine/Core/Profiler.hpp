#pragma once
#include "Engine/Core/MemoryManager.hpp"
#include "Engine/Core/Thread.hpp"
#include "Engine/Core/Time.hpp"
#include <string>

#define PROFILE_SCOPE(s) ProfileLogScope __pscope_##__LINE__##(s)
#define PROFILE_SCOPE_FUNCTION(s) ProfileLogScope __pscope_##__FUNCTION__##(__FUNCTION__)

class SimpleRenderer;
class DX11TextRenderer;


struct ProfilerNode
{
	char const *m_tag;
	unsigned long long m_startTime;
	unsigned long long m_endTime;
	ProfilerNode *m_parent;
	ProfilerNode *m_children;
	ProfilerNode *m_nextSibling;
};

struct ReportNode
{
	std::string m_message;
	ProfilerNode* m_treeNode;
	float			m_treeLevel;
};




class Profiler
{

public:
	static  Profiler* GetProfile();
	~Profiler();
	
	static void ProfilerSystemStartup();
	static void ProfilerSystemShutdown();
	static void ProfilerPush(char const *tag);
	static void ProfilerPop();
	static void ProfilerPause();
	static void ProfilerResume();
	static void ProfilerSnapshot();

	static void ProfilerTrackAllocation(size_t byte_size);
	static void ProfilerTrackFree(void *ptr, size_t byte_size);



	static void PrintReport();

	static void CheckIfInCorrectThread();
	static void IsNodeASbiling(ProfilerNode* newNode);
	static ProfilerNode* ProfilerGetPreviousFrame();
	static ProfilerNode* ProfilerGetPreviousFrame(char const *root_tag);

	static void PrintTreeView();
	static void PrintFlatView();

	static int HowDeepDoesNodeGoDownTree(ProfilerNode* childNode);
	static void SortByTotalTime(std::vector<ProfilerNode *> printTheseNodes);
	static void SortBySelfTime(std::vector<ProfilerNode *> printTheseNodes);

	static unsigned long long CalculateSelfTimeOfNode(ProfilerNode* childNode);


	static void RenderProfiler(SimpleRenderer* renderer, DX11TextRenderer m_loadedTextFile, DX11Texture2D* sample);
	static float GetTimeSpentPercent(unsigned long long selfTime);
	static unsigned long long GetTotalTimeOfNode(ProfilerNode* node);

	static void setTreeView();
	static void setFlatView();

	static void setSortByTotalTime();
	static void setSortBySelfTime();

	static 	void setElapsedFrameTimeLastFrame();
private:
	Profiler()
	{
		g_activeNode = nullptr;
		g_profilerPaused = false;
		g_enableTreeView = true;
		g_enableSortBySelfTime = false;
		g_enableSortByTotalTime = true;
	};



	static void AddNodeToPrintList(ProfilerNode* printTheseNodes);
	static void RenderFrameTimeInfo(SimpleRenderer* renderer, DX11TextRenderer m_loadedTextFile, DX11Texture2D* sample);

public:
	static Profiler* sProfile;
	
	static DWORD g_mainThreadID;

    static ProfilerNode* g_activeNode;


	static std::vector<ProfilerNode*> g_completedNodesThisFrame;
	static std::vector<ProfilerNode*> g_completedNodesPreviousFrame;
	static std::vector<ReportNode*> g_sortedNodeList;
	static ProfilerNode* g_lastCompletedNode;
	static bool g_ProfilePauseWhenFrameIsUp;
	static bool g_profilerPaused;
	static bool g_profileSnapshot;

	static bool g_enableTreeView;

	static bool g_enableSortBySelfTime;
	static bool g_enableSortByTotalTime;

	static unsigned long long g_ElapsedFrameTimeLastFrame;
	static float g_fps;
	static float g_ElapsedFrameTimeOverLastFrames[50];

};




class ProfileLogScope
{
public:
	ProfileLogScope(char const *n)
	{
		m_name = n;
		m_startop = GetCurrentPerformanceCounter();
		Profiler::ProfilerPush(n);
	}

	~ProfileLogScope()
	{
		//unsigned long long end_op = GetCurrentPerformanceCounter();
		//unsigned long long elapsed = end_op - start_op;

		//DebuggerPrintf("%s [%llu ops, %s]\n", name, elapsed, TimeOpCountToString(elapsed));
		Profiler::ProfilerPop();
	}




public:
	char const *m_name;
	unsigned long long m_startop;
};
