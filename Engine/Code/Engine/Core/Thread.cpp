#include "Engine/Core/Thread.hpp"


struct thread_pass_data_t
{
	thread_cb cb;
	void *arg;
};

//------------------------------------------------------------------------
static DWORD WINAPI ThreadEntryPointCommon(void *arg)
{
	thread_pass_data_t *pass_ptr = new thread_pass_data_t;
	pass_ptr = (thread_pass_data_t*)arg;

	pass_ptr->cb(pass_ptr->arg);
	delete pass_ptr;
	return 0;
}

//------------------------------------------------------------------------
// Creates a thread with the entry point of cb, passed data
thread_handle_t ThreadCreate(thread_cb cb, void *data)
{
	// handle is like pointer, or reference to a thread
	// thread_id is unique identifier
	thread_pass_data_t *pass = new thread_pass_data_t();
	pass->cb = cb;
	pass->arg = data;

	DWORD thread_id;
	thread_handle_t th = ::CreateThread(nullptr,   // SECURITY OPTIONS
		0,                         // STACK SIZE, 0 is default
		ThreadEntryPointCommon,    // "main" for this thread
		pass,                     // data to pass to it
		0,                         // initial flags
		&thread_id);              // thread_id

	return th;
}

//------------------------------------------------------------------------
void ThreadSleep(unsigned int ms)
{
	::Sleep(ms);
}

//------------------------------------------------------------------------
void ThreadYield()
{
	::SwitchToThread();
}

//------------------------------------------------------------------------
// Releases my hold on this thread.
void ThreadDetach(thread_handle_t th)
{
	::CloseHandle(th);
}

//------------------------------------------------------------------------
void ThreadJoin(thread_handle_t th)
{
	::WaitForSingleObject(th, INFINITE);
	::CloseHandle(th);
}
