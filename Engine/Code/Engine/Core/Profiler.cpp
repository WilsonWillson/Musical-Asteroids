#include "Engine/Core/Profiler.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/DX11/DX11TextRenderer.hpp"
#include "Engine/RHI/SimpleRenderer.hpp"
#include "Engine/Threading/Job.hpp"

Profiler* Profiler::sProfile = nullptr;
DWORD Profiler::g_mainThreadID;
ProfilerNode* Profiler::g_activeNode;
std::vector<ProfilerNode*> Profiler::g_completedNodesThisFrame;
std::vector<ProfilerNode*> Profiler::g_completedNodesPreviousFrame;
std::vector<ReportNode*> Profiler::g_sortedNodeList;
ProfilerNode* Profiler::g_lastCompletedNode;

bool Profiler::g_ProfilePauseWhenFrameIsUp;

bool Profiler::g_profilerPaused;
bool Profiler::g_profileSnapshot;
bool Profiler::g_enableTreeView;
bool Profiler::g_enableSortBySelfTime;
bool Profiler::g_enableSortByTotalTime;
unsigned long long Profiler::g_ElapsedFrameTimeLastFrame;
float Profiler::g_fps;
float Profiler::g_ElapsedFrameTimeOverLastFrames[50];


//----------------Constructor/Deconstructor----------------------------------------------------
Profiler* Profiler::GetProfile()
{
	if (sProfile == nullptr)
		sProfile = new Profiler();

	return sProfile;
}

Profiler::~Profiler()
{

	for (unsigned int i = 0; i < g_completedNodesPreviousFrame.size(); i++)
	{
		delete g_completedNodesPreviousFrame[i];
		g_completedNodesPreviousFrame[i] = nullptr;
	}

	for (unsigned int i = 0; i < g_completedNodesThisFrame.size(); i++)
	{
		delete g_completedNodesThisFrame[i];
		g_completedNodesThisFrame[i] = nullptr;
	}

	for (unsigned int i = 0; i < g_sortedNodeList.size(); i++)
	{
		delete g_sortedNodeList[i];
		g_sortedNodeList[i] = nullptr;
	}
}

//---------------------------Profiler Code-------------------------------------------------

void Profiler::ProfilerSystemStartup()
{
	GetProfile();
	g_mainThreadID = ::GetCurrentThreadId();
}

void Profiler::ProfilerSystemShutdown()
{
	CheckIfInCorrectThread();
	delete sProfile;
}

void Profiler::ProfilerPush(char const *tag)
{
	CheckIfInCorrectThread();
	if (g_profilerPaused == false)
	{
		if (g_activeNode == nullptr)
		{
			ProfilerNode* newNode = new ProfilerNode();
			newNode->m_children = nullptr;
			newNode->m_parent = nullptr;
			newNode->m_tag = tag;
			newNode->m_startTime = GetCurrentPerformanceCounter();
			newNode->m_endTime = 0;

			g_activeNode = newNode;
		}
		else
		{
			ProfilerNode* newNode = new ProfilerNode();
			newNode->m_children = nullptr;
			newNode->m_parent = g_activeNode;
			g_activeNode->m_children = newNode;
			IsNodeASbiling(newNode);
			newNode->m_tag = tag;
			newNode->m_startTime = GetCurrentPerformanceCounter();
			newNode->m_endTime = 0;

			g_activeNode = newNode;
		}
	}
	
}

void Profiler::IsNodeASbiling(ProfilerNode* newNode)
{
	
	for (unsigned int completeNodesStep = 0; completeNodesStep < g_completedNodesThisFrame.size(); completeNodesStep++)
	{

		if (g_completedNodesThisFrame[completeNodesStep]->m_parent == newNode->m_parent)
		{
			newNode->m_nextSibling = g_completedNodesThisFrame[completeNodesStep];
			return;
		}

	}
		newNode->m_nextSibling = nullptr;
}

void Profiler::ProfilerPop()
{
	CheckIfInCorrectThread();
	if (g_activeNode != nullptr)
	{
		g_activeNode->m_endTime = GetCurrentPerformanceCounter();

		g_completedNodesThisFrame.push_back(g_activeNode);

		if (g_activeNode->m_parent != nullptr)
			g_activeNode = g_activeNode->m_parent;
		else
		{
			g_lastCompletedNode = g_activeNode;
			g_activeNode = nullptr;

			//Allows us to only track one tree while also preparing the next frames tree.
			for (unsigned int i = 0; i < g_completedNodesPreviousFrame.size(); i++)
			{
				delete g_completedNodesPreviousFrame[i];
				g_completedNodesPreviousFrame[i] = nullptr;
			}

			g_completedNodesPreviousFrame = g_completedNodesThisFrame;
			g_completedNodesThisFrame.clear();

			for (unsigned int i = 0; i < g_completedNodesThisFrame.size(); i++)
			{
				delete g_completedNodesThisFrame[i];
				g_completedNodesThisFrame[i] = nullptr;
			}

			setElapsedFrameTimeLastFrame();

			if (g_ProfilePauseWhenFrameIsUp == true)
			{
				g_profilerPaused = true;
			}
			if (g_profileSnapshot == true)
			{
				g_ProfilePauseWhenFrameIsUp = true;
				g_profilerPaused = false;
				g_profileSnapshot = false;
			}
		}
	}

}	

void Profiler::ProfilerPause()
{
	CheckIfInCorrectThread();
	g_ProfilePauseWhenFrameIsUp = true;

}

void Profiler::ProfilerResume()
{
	CheckIfInCorrectThread();
	if(g_activeNode == nullptr)
		g_profilerPaused = false;
}

void Profiler::ProfilerSnapshot()
{
	CheckIfInCorrectThread();

	if (g_profilerPaused == true)
	{
		g_profileSnapshot = true;
		g_profilerPaused = false;
	}
}


ProfilerNode* Profiler::ProfilerGetPreviousFrame()
{
	CheckIfInCorrectThread();
	return g_lastCompletedNode;
}

ProfilerNode* Profiler::ProfilerGetPreviousFrame(char const *root_tag)
{
	CheckIfInCorrectThread();
	int tempNodeWeWant = 0;
	for (unsigned int i = 0; i < g_completedNodesPreviousFrame.size(); i++)
	{
		if (g_completedNodesPreviousFrame[i]->m_tag == root_tag)
			tempNodeWeWant = i;
	}

	return g_completedNodesPreviousFrame[tempNodeWeWant];
}


//---------------------Printing---------------------------------------

void Profiler::AddNodeToPrintList(ProfilerNode* printTheseNodes)
{
	int howDeepisNode = HowDeepDoesNodeGoDownTree(printTheseNodes);

	unsigned long long totalTime = GetTotalTimeOfNode(printTheseNodes);
	unsigned long long selfTime = CalculateSelfTimeOfNode(printTheseNodes);



	char const* TotalTimeAsString = TimeOpCountToString(totalTime);
	char const* selfTimeAsString = TimeOpCountToString(selfTime);


	std::string printMessage = std::string( "Tag: " )+ printTheseNodes->m_tag;
	printMessage += "           Total Percent:  " + std::to_string(GetTimeSpentPercent(totalTime));
	printMessage += std::string("       Total Time: ") + TotalTimeAsString;
	printMessage += "         Self Percent: " + std::to_string(GetTimeSpentPercent(selfTime));
	printMessage += std::string("        SelfTime: ") + selfTimeAsString;


	ReportNode* newReport = new ReportNode();
	newReport->m_message = printMessage;
	newReport->m_treeNode = printTheseNodes;
	newReport->m_treeLevel = (float)howDeepisNode;
	g_sortedNodeList.push_back(newReport);

	delete TotalTimeAsString;
	delete selfTimeAsString;
}



void Profiler::PrintReport()
{
	CheckIfInCorrectThread();

	for (unsigned int i = 0; i < g_sortedNodeList.size(); i++)
	{
		delete g_sortedNodeList[i];
		g_sortedNodeList[i] = nullptr;
	}

	if (g_enableTreeView == true)
	{
		PrintTreeView();
	}
	else
	{
		PrintFlatView();
	}
}

void Profiler::PrintTreeView()
{
	CheckIfInCorrectThread();
	std::vector<ProfilerNode*> printTheseNodes = g_completedNodesPreviousFrame;

	if (g_enableSortByTotalTime == true)
		SortByTotalTime(printTheseNodes);

	if (g_enableSortBySelfTime == true)
		SortBySelfTime(printTheseNodes);

}

void Profiler::PrintFlatView()
{
	CheckIfInCorrectThread();
	std::vector<ProfilerNode*> printTheseNodes = g_completedNodesPreviousFrame;

	if (g_enableSortByTotalTime == true)
		SortByTotalTime(printTheseNodes);

	if (g_enableSortBySelfTime == true)
		SortBySelfTime(printTheseNodes);
}

void Profiler::SortByTotalTime(std::vector<ProfilerNode *> printTheseNodes)
{
	g_sortedNodeList.clear();
	while (!printTheseNodes.empty())
	{
		unsigned long long totalTime = 0;
		int neededNode = 0;
		for (unsigned int i = 0; i < printTheseNodes.size(); i++)
		{
			unsigned long long elapsedTime = printTheseNodes[i]->m_endTime - printTheseNodes[i]->m_startTime;

			if (elapsedTime > totalTime)
			{
				totalTime = elapsedTime;
				neededNode = i;
			}
		}

		AddNodeToPrintList(printTheseNodes[neededNode]);
		printTheseNodes.erase(printTheseNodes.begin() + neededNode);
	}
}

int Profiler::HowDeepDoesNodeGoDownTree(ProfilerNode* childNode)
{
	ProfilerNode* currentNode = childNode;
	int layersDeep = 0;
	while (1)
	{
		if (currentNode->m_parent == nullptr)
			return layersDeep;

		currentNode = currentNode->m_parent;
		layersDeep++;
	}
}


void Profiler::SortBySelfTime(std::vector<ProfilerNode *> printTheseNodes)
{	
	g_sortedNodeList.clear();

	while (!printTheseNodes.empty())
	{
		unsigned long long selfTime = 0;
		int neededNode = 0;
			for (unsigned int i = 0; i < printTheseNodes.size(); i++)
			{
				unsigned long long elapsedTime = CalculateSelfTimeOfNode(printTheseNodes[i]);
				if (elapsedTime > selfTime)
				{
					selfTime = elapsedTime;
					neededNode = i;
				}

			}
		AddNodeToPrintList(printTheseNodes[neededNode]);

		printTheseNodes.erase(printTheseNodes.begin() + neededNode);
	}
}


float Profiler::GetTimeSpentPercent(unsigned long long timeSpent)
{
	for (unsigned int i = 0; i < g_completedNodesPreviousFrame.size(); i++)
	{
		if (g_completedNodesPreviousFrame[i]->m_parent == nullptr)
		{
			unsigned long long totalTimeSpentInFrame = g_completedNodesPreviousFrame[i]->m_endTime - g_completedNodesPreviousFrame[i]->m_startTime;

			float timePercent = ((float)timeSpent / (float)totalTimeSpentInFrame);
			return timePercent;
		}
	}
	return -1;
}


unsigned long long Profiler::GetTotalTimeOfNode(ProfilerNode* node)
{
	return node->m_endTime - node->m_startTime;
}

unsigned long long Profiler::CalculateSelfTimeOfNode(ProfilerNode* childNode)
{
	ProfilerNode* currentNode = childNode;
	unsigned long long selfTime = currentNode->m_endTime - currentNode->m_startTime;

	//It's the lowest you can go on Tree
	if (currentNode->m_children == nullptr)
			return selfTime;

	//Minus Children's total time, because total time includes it's children
	selfTime = selfTime - (currentNode->m_children->m_endTime - currentNode->m_children->m_startTime);
	
	currentNode = currentNode->m_children;
	//Look for siblings then leave
		while (1)
		{
			if (currentNode->m_nextSibling == nullptr)
				return selfTime;

			selfTime = selfTime - (currentNode->m_nextSibling->m_endTime - currentNode->m_nextSibling->m_startTime);

			currentNode = currentNode->m_nextSibling;
		}
}


//----------------------------------Render------------------------------------

void Profiler::RenderProfiler(SimpleRenderer* renderer, DX11TextRenderer m_loadedTextFile, DX11Texture2D* sample)
{
	PROFILE_SCOPE_FUNCTION("Render Profiler");
	float y = .15f;
	for (unsigned int i = 0; i < g_sortedNodeList.size(); i++)
	{
		char *text = new char[g_sortedNodeList[i]->m_message.size() + 1];
		strcpy(text, g_sortedNodeList[i]->m_message.c_str());
		
		float x = -.7f;
		if(g_enableTreeView == true)
			x += (float)(g_sortedNodeList[i]->m_treeLevel / 10.f);
		
		renderer->DrawDX11Text2D(Vector2(x, y), 10, Rgba(255, 255, 255, 255), m_loadedTextFile, text);

		delete[] text;
		y -= .1f;
	}

	RenderFrameTimeInfo(renderer, m_loadedTextFile, sample);
}


void Profiler::RenderFrameTimeInfo(SimpleRenderer* renderer, DX11TextRenderer m_loadedTextFile, DX11Texture2D* sample)
{
	PROFILE_SCOPE_FUNCTION("Render FrameTime");
	renderer->DrawDX11Text2D(Vector2(-.95f, .35f), 10, Rgba(255, 255, 255, 255), m_loadedTextFile, "Last Frame Time");

	std::string message = TimeOpCountToString(g_ElapsedFrameTimeLastFrame);
	char *text = new char[message.size() + 1];
	strcpy(text, message.c_str());

	renderer->DrawDX11Text2D(Vector2(-.80f, .35f), 10, Rgba(255, 255, 255, 255), m_loadedTextFile, text);
	delete text;
	renderer->DrawDX11Text2D(Vector2(-.95f, .4f), 10, Rgba(255, 255, 255, 255), m_loadedTextFile, "FPS: ");


	message = std::to_string(g_fps);
	char *text2 = new char[message.size() + 1];
	strcpy(text2, message.c_str());

	renderer->DrawDX11Text2D(Vector2(-.90f, .4f), 10, Rgba(255, 255, 255, 255), m_loadedTextFile, text2);
	delete text2;



	renderer->SetTexture2d(sample);

	for (int i = 0; i < 50; i++)
	{
		if (g_ElapsedFrameTimeOverLastFrames[i] == NULL)
		{
			g_ElapsedFrameTimeOverLastFrames[i] = g_fps;
		}
	}

	for (int i = 0; i < 49; i++)
	{
		g_ElapsedFrameTimeOverLastFrames[i] = g_ElapsedFrameTimeOverLastFrames[i + 1];
	}
	g_ElapsedFrameTimeOverLastFrames[49] = g_fps;

	float currentX = -.50f;
	float startY = .20f;
	float maxY = .40f;
	float yDifference = maxY - startY;

	for (int i = 0; i < 50; i++)
	{
		float nextX = currentX + .0065f;
		if (g_ElapsedFrameTimeOverLastFrames[i] == 60)
			renderer->draw_quad2d(Vector2(currentX, startY), Vector2(nextX, maxY), Vector2(0.0, 0.0), Vector2(1.0, 1.0), Rgba(1, 1, 1, 1));
		else
		{
			float percentlower = (float)g_ElapsedFrameTimeOverLastFrames[i] / (float)60;
			float currentYValue = startY + yDifference * percentlower;
			renderer->draw_quad2d(Vector2(currentX, startY), Vector2(nextX, currentYValue), Vector2(0.0, 0.0), Vector2(1.0, 1.0), Rgba(1, 1, 1, 1));
		}
		currentX = nextX;
	}
}

//-------------------------Heper Functions---------------------------------------------------

void Profiler::CheckIfInCorrectThread()
{
	if (g_mainThreadID != ::GetCurrentThreadId())
	{
		ASSERT_OR_DIE(true, "NOT IN ACTIVE THREAD");
	}
}

void Profiler::setTreeView()
{
	g_enableTreeView = true;
	g_enableSortByTotalTime = true;
	g_enableSortBySelfTime = false;
}

void Profiler::setFlatView()
{
	g_enableTreeView = false;
	g_enableSortByTotalTime = false;
	g_enableSortBySelfTime = true;
}

void Profiler::setSortByTotalTime()
{
	g_enableSortByTotalTime = true;
	g_enableSortBySelfTime = false;
}

void Profiler::setSortBySelfTime()
{
	g_enableSortByTotalTime = false;
	g_enableSortBySelfTime = true;
}

void Profiler::setElapsedFrameTimeLastFrame()
{
	g_ElapsedFrameTimeLastFrame = g_completedNodesPreviousFrame[g_completedNodesPreviousFrame.size()-1]->m_endTime - g_completedNodesPreviousFrame[g_completedNodesPreviousFrame.size() - 1]->m_startTime;
	g_fps = 1000000.f / (float)TimeOpCountTo_us(g_ElapsedFrameTimeLastFrame);
}