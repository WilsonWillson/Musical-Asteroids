#pragma once
#include <condition_variable>

#include "Engine/Core/Win32Includes.hpp"


class Signal
{
public:
	Signal();
	~Signal();

	// will signal all threads waiting on this object.
	void signal_all();

	// Suspstends calling thread until object is signaled.
	void wait();

	// Suspends calling thread until object is signaled, or 'ms' MS have
	// passed.  Returns true if object was signaled, and fales if it timed out.
	bool wait_for(unsigned int ms);

public:
	HANDLE os_event;
};


/************************************************************************/
/*                                                                      */
/* GLOBAL VARIABLES                                                     */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* FUNCTION PROTOTYPES                                                  */
/*                                                                      */
/************************************************************************/
void SignalTest();

