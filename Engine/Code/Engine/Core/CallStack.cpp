#include "Engine/Core/CallStack.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Win32Includes.hpp"
#include <malloc.h>
#include <DbgHelp.h>

#pragma warning( disable : 4091 ) //  warning C4091: 'typedef ': ignored on left of '' when no variable is declared


#define MAX_SYMBOL_NAME_LENGTH 128
#define MAX_FILENAME_LENGTH 1024
#define MAX_DEPTH 128


//-----------Variables and Typedefs---------------------------------------------------------------------------

// SymInitialize()
typedef BOOL(__stdcall *sym_initialize_t)(IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess);
typedef BOOL(__stdcall *sym_cleanup_t)(IN HANDLE hProcess);
typedef BOOL(__stdcall *sym_from_addr_t)(IN HANDLE hProcess, IN DWORD64 Address, OUT PDWORD64 Displacement, OUT PSYMBOL_INFO Symbol);

typedef BOOL(__stdcall *sym_get_line_t)(IN HANDLE hProcess, IN DWORD64 dwAddr, OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE64 Symbol);


static HMODULE gDebugHelp;
static HANDLE gProcess;
static SYMBOL_INFO  *gSymbol;

static sym_initialize_t LSymInitialize;
static sym_cleanup_t LSymCleanup;
static sym_from_addr_t LSymFromAddr;
static sym_get_line_t LSymGetLineFromAddr64;

static int gCallstackCount = 0;



//------------------------------------------------------------------------
Callstack::Callstack()
	: hash(0)
	, frame_count(0) {}


//------------------------Initalize/Deintizalize------------------------------------------------
bool CallstackSystemInit()
{
	// Load the dll, similar to OpenGL function fecthing.
	// This is where these functions will come from.
	gDebugHelp = LoadLibraryA("dbghelp.dll");
	if (gDebugHelp == nullptr) {
		return false;
	}

	// Get pointers to the functions we want from the loded library.
	LSymInitialize = (sym_initialize_t)GetProcAddress(gDebugHelp, "SymInitialize");
	LSymCleanup = (sym_cleanup_t)GetProcAddress(gDebugHelp, "SymCleanup");
	LSymFromAddr = (sym_from_addr_t)GetProcAddress(gDebugHelp, "SymFromAddr");
	LSymGetLineFromAddr64 = (sym_get_line_t)GetProcAddress(gDebugHelp, "SymGetLineFromAddr64");

	// Initialize the system using the current process [see MSDN for details]
	gProcess = ::GetCurrentProcess();
	LSymInitialize(gProcess, NULL, TRUE);

	// Preallocate some memory for loading symbol information. 
	gSymbol = (SYMBOL_INFO *) ::malloc(sizeof(SYMBOL_INFO) + (MAX_FILENAME_LENGTH * sizeof(char)));
	gSymbol->MaxNameLen = MAX_FILENAME_LENGTH;
	gSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	return true;
}

void CallstackSystemDeinit()
{
	// cleanup after ourselves
	free(gSymbol);
	gSymbol = nullptr;

	LSymCleanup(gProcess);

	FreeLibrary(gDebugHelp);
	gDebugHelp = NULL;
}


//---------------------------Creation/Destruction---------------------------------------------
Callstack* CreateCallstack(unsigned int skip_frames)
{
	// Cappture the callstack frames - uses a windows call
	void *stack[MAX_DEPTH];
	DWORD hash;

	// skip_frames:  number of frames to skip [starting at the top - so don't return the frames for "CreateCallstack" (+1), plus "skip_frame_" layers.
	// max_frames to return
	// memory to put this information into.
	// out pointer to back trace hash.
	unsigned int frames = CaptureStackBackTrace(1 + skip_frames, MAX_DEPTH, stack, &hash);

	// create the callstack using an untracked allocation
	Callstack *cs = (Callstack*) ::malloc(sizeof(Callstack));

	// force call the constructor (new in-place0
	cs = new (cs) Callstack();

	// copy the frames to our callstack object
	unsigned int frame_count = min(MAX_FRAMES_PER_CALLSTACK, frames);
	cs->frame_count = frame_count;
	memcpy(cs->frames, stack, sizeof(void*) * frame_count);

	cs->hash = hash;

	return cs;
}


// Can not be static - called when
// the callstack is freed.
void DestroyCallstack(Callstack *ptr)
{
	::free(ptr);
}



//--------------------------------CallStack Utility Functions----------------------------------------


// Fills lines with human readable data for the given callstack
// Fills from top to bottom (top being most recently called, with each next one being the calling function of the previous)
//
// Additional features you can add;
// [ ] If a file exists in yoru src directory, clip the filename
// [ ] Be able to specify a list of function names which will cause this trace to stop.
unsigned int CallstackGetLines(callstack_line_t *line_buffer, unsigned int const max_lines, Callstack *cs)
{
	IMAGEHLP_LINE64 line_info;
	DWORD line_offset = 0; // Displacement from the beginning of the line 
	line_info.SizeOfStruct = sizeof(IMAGEHLP_LINE64);


	unsigned int  count = min(max_lines, cs->frame_count);
	unsigned int  idx = 0;

	for (unsigned int  i = 0; i < count; ++i) {
		callstack_line_t *line = &(line_buffer[idx]);
		DWORD64 ptr = (DWORD64)(cs->frames[i]);
		if (FALSE == LSymFromAddr(gProcess, ptr, 0, gSymbol)) {
			continue;
		}

		std::string demoString;
		std::string s(gSymbol->Name);
		int len = s.length();
		if(len < 128)
			strcpy_s(line->function_name, 128, gSymbol->Name);

		BOOL bRet = LSymGetLineFromAddr64(
			GetCurrentProcess(), // Process handle of the current process 
			ptr, // Address 
			&line_offset, // Displacement will be stored here by the function 
			&line_info);         // File name / line information will be stored here 

		if (bRet) {
			line->line = line_info.LineNumber;

			strcpy_s(line->filename, 128, line_info.FileName);
			line->offset = line_offset;

		}
		else {
			// no information
			line->line = 0;
			line->offset = 0;
			strcpy_s(line->filename, 128, "N/A");
		}

		++idx;
	}

	return idx;
}


void PrintCallstack(Callstack *cs)
{
	char line_buffer[512];
	callstack_line_t lines[128];
	unsigned int  line_count = CallstackGetLines(lines, 128, cs);
	for (unsigned int i = 0; i < line_count; ++i) {
		// this specific format will make it double clickable in an output window 
		// taking you to the offending line.
		//sprintf_s(line_buffer, 512, "%s(%u): %s\n", lines[i].filename, lines[i].line, lines[i].function_name);

		// print to output and console
		DebuggerPrintf(line_buffer);
	}

}


void PrintCallStackList(CallStackNode* cs)
{
	int counter = 0;
	CallStackNode* conductor = cs;
	if (conductor != 0) { //Makes sure there is a place to start

		while (conductor->next != 0)
		{
			PrintBasicStackInfo(counter, conductor);
			PrintCallstack(conductor->data);
			conductor = conductor->next;
			counter++;
		}
		PrintBasicStackInfo(counter, conductor);
		PrintCallstack(conductor->data);
	}
}

void PrintBasicStackInfo(int counter, CallStackNode* conductor)
{
	std::string s = std::to_string(counter);
	char const *counterchar = s.c_str();
	std::string s2 = std::to_string(conductor->frame);
	char const *pchar = s2.c_str();
	DebuggerPrintf("Line ");
	DebuggerPrintf(counterchar);
	DebuggerPrintf("\n");
	DebuggerPrintf("Frame ");
	DebuggerPrintf(pchar);
	DebuggerPrintf("\n");
}
