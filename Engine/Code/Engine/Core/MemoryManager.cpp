#include "Engine/Core/MemoryManager.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Core/CallStack.hpp"
#include "Engine/Core/config.hpp"
#include "Engine/Core/Profiler.hpp"
#include <malloc.h>






//-------------------Update----------------------------------------------
void UpdateMemoryManager()
{
	g_CurrentFrame++;
	g_FrameCounter++;

	if (g_FrameCounter == (unsigned)g_FrameGoal)
	{
		g_FrameCounter = 0;
		g_FrameGoal = -1;
	}
	g_previousFrameAllocs = g_FrameAllocs;
	g_previousFrameFrees = g_FrameFrees;
	g_FrameAllocs = 0;
	g_FrameFrees = 0;

	for (int i = 0; i < 200; i++)
	{
		if (g_AllocatedBytesOverLastFrames[i] == NULL)
		{
			g_AllocatedBytesOverLastFrames[i] = g_TotalMemoryHighwater;
			return;
		}
	}
	
	for (int i = 0; i < 199; i++)
	{
		g_AllocatedBytesOverLastFrames[i] = g_AllocatedBytesOverLastFrames[i + 1];
	}
	g_AllocatedBytesOverLastFrames[199] = g_TotalMemoryHighwater;
		
}


//---------------------------CallStack Memory Functions---------------------------------------------

void AddToCallStack()
{
#pragma warning(push)
#pragma warning(disable:4127)
	CriticalSection m_lock;
	if (TRACK_MEMORY == TRACK_MEMORY_VERBOSE)
	{
		SCOPE_LOCK(&m_lock);
		CallStackNode* conductor;
		Callstack *cs = CreateCallstack(0);
		if (g_frontNode == nullptr)
		{
			g_frontNode = (CallStackNode*) ::malloc(sizeof(CallStackNode));
			// force call the constructor (new in-place0
			g_frontNode = new (g_frontNode) CallStackNode();

			g_endNode = (CallStackNode*) ::malloc(sizeof(CallStackNode));
			// force call the constructor (new in-place0
			g_endNode = new (g_endNode) CallStackNode();

			g_frontNode->data = cs;
			g_frontNode->frame = g_CurrentFrame;
			g_frontNode->next = 0;
			g_endNode = g_frontNode;
		}
		else
		{
			conductor = g_endNode;
			//conductor = conductor->next;

			conductor->next = (CallStackNode*) ::malloc(sizeof(CallStackNode));
			// force call the constructor (new in-place0
			conductor->next = new (conductor->next) CallStackNode();
			conductor = conductor->next; // Points to that node
			conductor->frame = g_CurrentFrame;
			conductor->next = 0;
			conductor->data = cs;

			g_endNode = conductor;
		}
	}
#pragma warning(pop)
}

void MemoryProfilerLogLiveAllocationsForFrameAmount(int frames)
{
	g_FrameGoal = frames;
	g_FrameCounter = 0;
}

void PrintCallStackListinOutput()
{
	PrintCallStackList(g_frontNode);
}

//------------------------Profiler-----------------------------------------

void RenderVisualizer(SimpleRenderer* renderer, DX11TextRenderer* loadedTextFile, DX11Texture2D* graphTexture)
{
#pragma warning(push)
#pragma warning(disable:4127)
	if(TRACK_MEMORY == TRACK_MEMORY_NONE)
		renderer->DrawDX11Text2D(Vector2(-1.f, 0.75f), 1, Rgba(0, 0, 0, 255), *loadedTextFile, "Memory: None");
	if (TRACK_MEMORY == 0)
		renderer->DrawDX11Text2D(Vector2(-1.f, 0.75f), 1, Rgba(0, 0, 0, 255), *loadedTextFile, "Memory[Basic]");
	if (TRACK_MEMORY == 1)
		renderer->DrawDX11Text2D(Vector2(-1.f, 0.75f), 10, Rgba(0, 0, 0, 255), *loadedTextFile, "Memory[Verbose]");
#pragma warning(pop)

	RenderVisualizerTextData(renderer, loadedTextFile);

	renderer->SetTexture2d(graphTexture);
	RenderDataOverTimeGraph(renderer);
}

void RenderVisualizerTextData(SimpleRenderer* renderer, DX11TextRenderer* loadedTextFile)
{
	PROFILE_SCOPE_FUNCTION("RenderVisualText");
	renderer->DrawDX11Text2D(Vector2(-1.f, 0.70f), 10, Rgba(0, 0, 0, 255), *loadedTextFile, "Alloc Count: ");
	char text[30];
	_itoa_s(g_AllocCount, text, 10);
	//ChangeBytesToNiceTextFormat(g_AllocCount, text);
	renderer->DrawDX11Text2D(Vector2(-.80f, 0.70f), 10, Rgba(0, 0, 0, 255), *loadedTextFile, text);

	renderer->DrawDX11Text2D(Vector2(-1.f, 0.65f), 10, Rgba(0, 0, 0, 255), *loadedTextFile, "Bytes Allocated: ");
	ChangeBytesToNiceTextFormat(g_AllocatedByteCount, text);
	renderer->DrawDX11Text2D(Vector2(-.80f, 0.65f), 10, Rgba(0, 0, 0, 255), *loadedTextFile, text);

	renderer->DrawDX11Text2D(Vector2(-1.f, 0.60f), 10, Rgba(0, 0, 0, 255), *loadedTextFile, "Highwater: ");
	ChangeBytesToNiceTextFormat(g_TotalMemoryHighwater, text);
	renderer->DrawDX11Text2D(Vector2(-.80f, 0.60f), 10, Rgba(0, 0, 0, 255), *loadedTextFile, text);

	renderer->DrawDX11Text2D(Vector2(-1.f, 0.55f), 10, Rgba(0, 0, 0, 255), *loadedTextFile, "Frame Allocs: ");
	//ChangeBytesToNiceTextFormat(g_previousFrameAllocs, text);
	_itoa_s(g_previousFrameAllocs, text, 30, 10);
	renderer->DrawDX11Text2D(Vector2(-.80f, 0.55f), 10, Rgba(0, 0, 0, 255), *loadedTextFile, text);


	renderer->DrawDX11Text2D(Vector2(-1.f, 0.50f), 10, Rgba(0, 0, 0, 255), *loadedTextFile, "Last Free Frames: ");
	//ChangeBytesToNiceTextFormat(g_previousFrameFrees, text);
	_itoa_s(g_previousFrameFrees, text, 30, 10);
	renderer->DrawDX11Text2D(Vector2(-.80f, 0.50f), 10, Rgba(0, 0, 0, 255), *loadedTextFile, text);
}

void ChangeBytesToNiceTextFormat(size_t bytesToWriteOut, char* text2)
{
	if (bytesToWriteOut > (2048) && bytesToWriteOut < (2 * 1048576))
	{
		bytesToWriteOut = bytesToWriteOut / 1024;
		_itoa_s(bytesToWriteOut, text2, 30, 10);
		char size[] = " kb";
		std::strncat(text2, size, 30 - strlen(text2) - 1);
		return;
	}

	if (bytesToWriteOut > (2 * 1048576))
	{
		bytesToWriteOut = bytesToWriteOut / 1048576;
		_itoa_s(bytesToWriteOut, text2,30, 10);
		char size[] = " mb";
		std::strncat(text2, size, 30 - strlen(text2) - 1);
		return;
	}
}

void RenderDataOverTimeGraph(SimpleRenderer* renderer)
{
	PROFILE_SCOPE_FUNCTION("RenderTimeGraph");
	//Bounds of whole quad-> mins -.50, .50, maxs .80, .80
		//Example renderer->draw_quad2d(Vector2(-.50, .50), Vector2(.80, .80), Vector2(0.0, 0.0), Vector2(1.0, 1.0), Rgba(1, 1, 1, 1));
	float currentX = -.50f;
	float startY = .50f;
	float maxY = .80f;

	for (int i = 0; i < 200; i++)
	{
		float nextX = currentX + .0065f;
		if(g_AllocatedBytesOverLastFrames[i] == g_TotalMemoryHighwater)
			renderer->draw_quad2d(Vector2(currentX, startY), Vector2( nextX, maxY), Vector2(0.0, 0.0), Vector2(1.0, 1.0), Rgba(1, 1, 1, 1));
		else
		{
			float percentlower = (float)g_AllocatedBytesOverLastFrames[i] / (float)g_TotalMemoryHighwater;
			float currentYValue = .50f + .30f * percentlower;
			renderer->draw_quad2d(Vector2(currentX, startY), Vector2(nextX, currentYValue), Vector2(0.0, 0.0), Vector2(1.0, 1.0), Rgba(1, 1, 1, 1));
		}
		currentX = nextX;
	}

}

//--------------------Getters/Setters-----------------------------------------------

int getFrameGoal()
{
	return g_FrameGoal;
}

void setFrameGoal(int newGoal)
{
	g_FrameGoal = newGoal;
}

unsigned int getFrameCounter()
{
	return g_FrameCounter;
}

unsigned int getAllocCount()
{
	return g_AllocCount;
}

unsigned int  getFrameAllocs()
{
	return g_FrameAllocs;
}

unsigned int getFreeFrames()
{
	return g_FrameFrees;
}

unsigned int getPreviousFrameAllocs()
{
	return g_previousFrameAllocs;
}

unsigned int getPreviousFreeFrames()
{
	return g_previousFrameFrees;
}

size_t getAllocatedByteCount()
{
	return g_AllocatedByteCount;
}

size_t getTotalMemoryHighwater()
{
	return g_TotalMemoryHighwater;
}

bool getProfilerEnabled()
{
	return g_ProfilerEnabled;
}

void setProfilerEnabled(bool setProfilerEnabled)
{
	g_ProfilerEnabled = setProfilerEnabled;
}

//------------------------------Operators-------------------------------------------------------

struct allocation_t
{
	size_t byte_size;
};

/*

void* operator new(size_t const size)
{
	++g_AllocCount;
	++g_FrameAllocs;
	g_AllocatedByteCount += size;
	if (g_AllocatedByteCount > g_TotalMemoryHighwater)
		g_TotalMemoryHighwater = g_AllocatedByteCount;

	//AddToCallStack();

	size_t alloc_size = size + sizeof(allocation_t);
	allocation_t *ptr = (allocation_t*)malloc(alloc_size);
	ptr->byte_size = size;
	return ptr + 1;
}



void operator delete(void *ptr)
{
	--g_AllocCount;
	++g_FrameFrees;

	allocation_t *size_ptr = (allocation_t*)ptr;
	size_ptr--;

	g_AllocatedByteCount -= size_ptr->byte_size;
	free(size_ptr);
}
*/