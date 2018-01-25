#pragma once
#include "Engine/DX11/DX11TextRenderer.hpp"
#include "Engine/DX11/DX11Texture2D.hpp"
#include "Engine/RHI/SimpleRenderer.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Core/Thread.hpp"
#include "Engine/Core/config.hpp"
#pragma warning(disable : 4996) 


// #define DETECT_MEMORY_OVERRUN



struct CallStackNode;

static bool g_ProfilerEnabled = false;
static unsigned int	g_CurrentFrame = 0;
static unsigned int g_FrameCounter = 0;
static int g_FrameGoal = -1;
static unsigned int g_AllocCount = 0;
static unsigned int g_FrameAllocs = 0;
static unsigned int g_FrameFrees = 0;
static size_t g_AllocatedByteCount = 0;
static size_t g_TotalMemoryHighwater = 0;
static unsigned int g_previousFrameAllocs = 0;
static unsigned int g_previousFrameFrees = 0;
static unsigned int g_AllocatedBytesOverLastFrames[200];
static int g_ArrayPosition = 0;
static CallStackNode* g_frontNode = nullptr;
static CallStackNode* g_endNode = nullptr;

//---------------------------Profiler-----------------------------------------------
void RenderVisualizer(SimpleRenderer* renderer, DX11TextRenderer* loadedTextFile, DX11Texture2D* m_fontSample);
void RenderVisualizerTextData(SimpleRenderer* renderer, DX11TextRenderer* loadedTextFile);
void ChangeBytesToNiceTextFormat(size_t BytesToWriteOut, char* text);
void RenderDataOverTimeGraph(SimpleRenderer* renderer);

//-------------Memory Data----------------------------------------------------
void UpdateMemoryManager();

//---------------------------CallStack Memory Functions-------------------------
void AddToCallStack();
void PrintCallStackListinOutput();
void MemoryProfilerLogLiveAllocationsForFrameAmount(int frames);

//---------------Getters/Setters----------------------------------------------------------
int getFrameGoal();
void setFrameGoal(int goal);
unsigned int getFrameCounter();
unsigned int getAllocCount();
unsigned int getFrameAllocs();
unsigned int getFreeFrames();
unsigned int getPreviousFrameAllocs();
unsigned int getPreviousFreeFrames();
size_t getAllocatedByteCount();
size_t getTotalMemoryHighwater();
bool getProfilerEnabled();
void setProfilerEnabled(bool setProfilerEnabled);

