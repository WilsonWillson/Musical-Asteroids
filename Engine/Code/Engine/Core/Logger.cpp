#include "Engine/Core/Logger.hpp"
#include "Engine/Core/Thread.hpp"
#include "Engine/Core/MemoryManager.hpp"
#include "Engine/Core/Event.hpp"
#include "Engine/Threading/Job.hpp"
#include "Engine/Threading/Signal.hpp"
#include "Engine/Core/Win32Includes.hpp"
#include <queue>
#include <string>
#include <vector>
#include <ctime>

ThreadSafeQueue<std::string> g_Messages;
bool g_LoggerThreadRunning = true;
bool g_FlushingThread = false;
CriticalSection g_lock;
EventV0 g_LogEvent;
Signal g_LogSignal;
std::vector<std::string> g_illegalTags;

//-----------------------------GARBAGE FUNCTIONS FOR ASSIGNMENT 2-------------------------------------------
void GenerateGarbageWork(void *data)
{
	FILE *fh = fopen("garbage.log", "w+");
	for (unsigned int i = 0; i < (size_t)data; ++i) {
		fprintf(fh, "Garbage Thread: %i\n",i);
	}
	fclose(fh);
}

void GenerateGarbageLines(void *data)
{
	SCOPE_LOCK(&g_lock);
	int* iptr = (int*)data;
	int threadNumber = iptr[0];
	int lineNumber = iptr[1];


	for (int i = 0; i < lineNumber; ++i) {
		std::string text = "Thread " + std::to_string(threadNumber) + ": writing line " + std::to_string(i);
		const char *msg = text.c_str();
		LogPrint(msg);
	}
}

//-----------------------------TEST FUNCTIONS---------------------------------------------------------------
void ThreadTest(size_t byte_count)
{
	void* passedInData = (void*) byte_count;
	//thread_handle_t th = ThreadCreate(GenerateGarbageWork, passedInData);
	Job *job = JobCreate(JOB_GENERIC, GenerateGarbageWork, &passedInData);
	JobDispatchAndRelease(job);

	//ThreadDetach(th);
}

void LogTest(int thread_count, int line_count)
{
	//CriticalSection m_lock;
	for (int i = 0; i < thread_count; i++)
	{
		int* passedInData = new int[2];
		passedInData[0] = i;
		passedInData[1] = line_count;
		void* data = passedInData;
		thread_handle_t th = ThreadCreate(GenerateGarbageLines, data);
		ThreadDetach(th);
	}
}

void LogFlushTest(char const *text)
{
	LogPrint(text);
	LogFlush();
}


//------------------Logger Code------------------------------------------------------------------
void LoggerThread(void*)
{
	JobConsumer loggerConsumer;
	loggerConsumer.add_category(JOB_LOGGER);
	JobSystemSetCategorySignal(JOB_LOGGER, &g_LogSignal);

	FILE *fh = fopen("log.log", "w+");
	if (nullptr == fh) {
		return;
	}

	g_LogEvent.subscribe(fh, LogWriteToFile);
	g_LogEvent.subscribe(nullptr, LogWriteToDebugger);

	std::string msg;
	while (g_LoggerThreadRunning) {
		//g_LogSignal.wait();
		if (g_FlushingThread == true)
		{
			while (!g_Messages.empty())
			{
				if (g_Messages.pop(&msg)) {
					g_LogEvent.trigger(&msg);
				}
			}
			fflush(fh);
			g_FlushingThread = false;
			__debugbreak();
		}

		if (g_Messages.pop(&msg)) {
			g_LogEvent.trigger(&msg);
		}

		if (g_Messages.empty())
		{
			loggerConsumer.consume_all();
			ThreadSleep(1);
		}
	}

	fclose(fh);
}

void LogFlush()
{
	g_FlushingThread = true;
}

void LogShutDown(thread_handle_t th)
{
	g_LoggerThreadRunning = false;
	ThreadJoin(th);

	Job *job = JobCreate(JOB_LOGGER, TimeStampFile, nullptr);
	JobDispatchAndRelease(job);
}


void TimeStampFile(void* ptr)
{
	ptr;
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	std::string timeStamp = std::to_string((now->tm_year + 1900)) + '_' + std::to_string((now->tm_mon + 1)) + '_' + std::to_string(now->tm_mday) + "__"
		+ std::to_string(now->tm_hour) + "" + std::to_string(now->tm_min) + "" + std::to_string(now->tm_sec);

	std::string logTitle = "log_" + timeStamp + ".txt";
	std::string log = "log.log";
	CopyFileA(log.c_str(), logTitle.c_str(), true);
}

//----------------------------Logger Write--------------------------------------------
void LogWriteToFile(void *user_arg, void *event_arg)
{
	FILE *fh = (FILE*)user_arg;
	std::string *msg = (std::string*)event_arg;

	fprintf(fh, "%s\n", msg->c_str());
}

void LogWriteToDebugger(void *user_arg, void *event_arg)
{
	user_arg;
	std::string *msg = (std::string*)event_arg;
	OutputDebugStringA(msg->c_str());
	OutputDebugStringA("\n");
}


//-----------------------Log Print Functions-------------------------------------------
void LogPrint(char const *msg)
{
	LogTaggedPrint("Default", msg);
}

void LogPrintWarning(char const *msg)
{
	LogTaggedPrint("Warning", msg);
}

void LogPrintError(char const *msg)
{
	//IF YOU HAVE DISABLED ERROR YOU STILL DON'T WANT To FLUSH. ONLY REASON FOR REPEATED CODE
	for (unsigned int illegalStep = 0; illegalStep < g_illegalTags.size(); illegalStep++)
	{
		if (g_illegalTags[illegalStep] == "Error")
			return;
	}
	LogTaggedPrint("Error", msg);
	LogFlush();
}


//-------------------Log Tags Functions-------------------------------------------------
void LogTaggedPrint(char const *tag, char const *format, ...)
{
	for (unsigned int illegalStep = 0; illegalStep < g_illegalTags.size(); illegalStep++)
	{
		if (g_illegalTags[illegalStep] == tag)
			return;
	}
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	std::string timeStamp = std::to_string((now->tm_year + 1900)) + '/' + std::to_string((now->tm_mon + 1)) + '/' + std::to_string(now->tm_mday) + "  " 
							+ std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec);
	std::string ta = tag;
	std::string form = format;
	std::string finalmessage =  "[" + ta + "] " + "[" + timeStamp + "] " + form;
	const char *msg = finalmessage.c_str();
	
	g_Messages.push(msg);
}

void LogTaggedPrintWithCallStack(char const *tag, char const *format, ...)
{
	LogTaggedPrint(tag, format);
	AddToCallStack();
}

void LogDisable(char const *tag)
{
	g_illegalTags.push_back(tag);
}

void LogEnable(char const *tag)
{
	for (unsigned int illegalStep = 0; illegalStep < g_illegalTags.size(); illegalStep++)
	{
		if (g_illegalTags[illegalStep] == tag)
		{
			g_illegalTags.erase(g_illegalTags.begin() + illegalStep);
			return;
		}
	}
}

void LogDisableAll(std::vector<std::string> disableTags)
{
	for (unsigned int disableStep = 1; disableStep < disableTags.size(); disableStep++)
	{
		g_illegalTags.push_back(disableTags[disableStep].c_str());
	}
}

void LogEnableAll()
{
	while (!g_illegalTags.empty())
	{
		g_illegalTags.pop_back();
	}
}