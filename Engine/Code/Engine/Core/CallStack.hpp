#pragma once


#define MAX_FRAMES_PER_CALLSTACK (128)

struct callstack_line_t
{
	char filename[128];
	char function_name[128];
	unsigned int line;
	unsigned int offset;
};


class Callstack
{
public:
	Callstack();

	unsigned int hash;
	unsigned int frame_count;
	void* frames[MAX_FRAMES_PER_CALLSTACK];
};

struct CallStackNode {
	Callstack* data;
	int			frame;
	CallStackNode *next;
};


bool CallstackSystemInit();
void CallstackSystemDeinit();

// Feel free to reorg this in a way you like - this is very C style.  
// Really, we just want to make sure these callstacks are not allocated on the heap.
// - You could creat them in-place in the meta-data if you prefer (provide memory to fill)
// - You could overload new on the Callstack class, cause "new Callstack(skip_frames)" to call that, a
//   and keeping this functionality.

// As this is - this will create a callstack using malloc (untracked allocation), skipping the first few frames.
Callstack* CreateCallstack(unsigned int skip_frames);
void DestroyCallstack(Callstack *c);

unsigned int CallstackGetLines(callstack_line_t *line_buffer, unsigned int const max_lines, Callstack *cs);

void PrintCallstack(Callstack *cs);
void PrintCallStackList(CallStackNode* cs);
void PrintBasicStackInfo(int counter, CallStackNode* conductor);
