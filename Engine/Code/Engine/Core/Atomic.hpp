#pragma once

#include "Engine/Core/Win32Includes.hpp"

/************************************************************************/
/*                                                                      */
/* STRUCTS                                                              */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* FUNCTIONS                                                            */
/*                                                                      */
/************************************************************************/

//--------------------------------------------------------------------
// Will return the result of the operation
__forceinline
unsigned int AtomicAdd(unsigned int volatile *ptr, unsigned int const value)
{
	return (unsigned int) ::InterlockedAddNoFence((LONG volatile*)ptr, (LONG)value);
}

//--------------------------------------------------------------------
__forceinline
unsigned int AtomicIncrement(unsigned int *ptr)
{
	return (unsigned int) ::InterlockedIncrementNoFence((LONG volatile*)ptr);
}

//--------------------------------------------------------------------
__forceinline
unsigned int AtomicDecrement(unsigned int *ptr)
{
	return (unsigned int) ::InterlockedDecrementNoFence((LONG volatile*)ptr);
}

//--------------------------------------------------------------------
__forceinline
unsigned int CompareAndSet(unsigned int volatile *ptr, unsigned int const comparand, unsigned int const value)
{
	/*
	unsigned int const old_value = *ptr;
	if (old_value == comparand) {
	*ptr = value;
	}
	return old_value;
	*/

	return ::InterlockedCompareExchange(ptr, value, comparand);
}

//--------------------------------------------------------------------

/*
__forceinline
bool CompareAndSet128(unsigned long long volatile data[2], unsigned long long comparand[2], unsigned long long value[2])
{
	return 1 == ::InterlockedCompareExchange128((long long volatile*)data, value[1], value[0], (long long*)comparand);
}


template <typename T>
__forceinline T* CompareAndSetPointer(T *volatile *ptr, T *comparand, T *value)
{
	return (T*)::InterlockedCompareExchangePointerNoFence((PVOID volatile*)ptr, (PVOID)value, (PVOID)comparand);
}
*/
