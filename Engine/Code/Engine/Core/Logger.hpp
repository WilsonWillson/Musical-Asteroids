#pragma once
#include "Engine/Core/Thread.hpp"
#include	<string>
#include <vector>


//Test functions that are ugly
void LogTest(int thread_count, int line_count);
void LogFlushTest(char const *text);
void ThreadTest(size_t byte_count);

//Log Functions
void LoggerThread(void*);
void LogFlush();
void LogShutDown(thread_handle_t th);

void TimeStampFile(void*);
//Writes
void LogWriteToFile(void *user_arg, void *event_arg);
void LogWriteToDebugger(void *user_arg, void *event_arg);

//Prints
void LogPrint(char const *msg);
void LogPrintWarning(char const *msg);
void LogPrintError(char const *msg);

//Tags
void LogTaggedPrint(char const *tag, char const *format, ...);
void LogTaggedPrintWithCallStack(char const *tag, char const *format, ...);
void LogDisable(char const *tag);
void LogEnable(char const *tag);
void LogDisableAll(std::vector<std::string> disableTags);
void LogEnableAll();



