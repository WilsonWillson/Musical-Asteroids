//-----------------------------------------------------------------------------------------------
// Time.cpp
//	A simple high-precision time utility function for Windows
//	based on code by Squirrel Eiserloh

//-----------------------------------------------------------------------------------------------
#include "Engine/Core/Time.hpp"
#include "Engine/Core/Win32Includes.hpp"
#include <stdio.h>



class InternalTimeSystem
{
public:
	InternalTimeSystem()
	{
		::QueryPerformanceFrequency((LARGE_INTEGER*)&ops_per_second);
		seconds_per_op = 1.0 / (double)ops_per_second;

		::QueryPerformanceCounter((LARGE_INTEGER*)&start_ops);
	}

	unsigned long long start_ops;
	unsigned long long ops_per_second;

	double seconds_per_op;
};

static InternalTimeSystem gTime;


unsigned long long GetCurrentPerformanceCounter()
{
	unsigned long long i;
	QueryPerformanceCounter((LARGE_INTEGER*)&i);
	return i;
}

double GetPeromanceCounterInSeconds()
{
	LARGE_INTEGER  li;
	QueryPerformanceCounter(&li);
	return QueryPerformanceFrequency(&li);
}




//------------------------------------Print Code------------------------------------------------------------

char const * TimeOpCountToString(unsigned long long op_count)
{
	char* buffer = new char[128]();
	unsigned long long us = TimeOpCountTo_us(op_count);

	if (us < 1500) {
		sprintf_s(buffer, 128, "%llu us", us);
	}
	else if (us < 1500000) {
		double ms = (double)us / (double)1000.0;
		sprintf_s(buffer, 128, "%.4f ms", ms);
	}
	else {
		double s = (double)us / (double)(1000000.0);
		sprintf_s(buffer, 128, "%.4f s", s);
	}

	return buffer;
}


unsigned long long TimeOpCountTo_us(unsigned long long op_count)
{
	op_count *= (1000U * 1000U);
	unsigned long long const us = (unsigned long long)(op_count * gTime.seconds_per_op);
	return us;
}

double TimeOpCountTo_ms(unsigned long long op_count)
{
	double seconds = op_count * gTime.seconds_per_op;
	return seconds * 1000.0;
}

unsigned long long TimeOpCountFrom_ms(double ms)
{
	double s = ms / 1000.0;
	unsigned long long const ops = (unsigned long long)(s * gTime.ops_per_second);
	return ops;
}








//----------------------------------------Old SD Code-------------------------------------------------------
double InitializeTime( LARGE_INTEGER& out_initialTime )
{
	LARGE_INTEGER countsPerSecond;
	QueryPerformanceFrequency( &countsPerSecond );
	QueryPerformanceCounter( &out_initialTime );
	return( 1.0 / static_cast< double >( countsPerSecond.QuadPart ) );
}


//-----------------------------------------------------------------------------------------------
double GetCurrentTimeSeconds()
{
	static LARGE_INTEGER initialTime;
	static double secondsPerCount = InitializeTime( initialTime );
	LARGE_INTEGER currentCount;
	QueryPerformanceCounter( &currentCount );
	LONGLONG elapsedCountsSinceInitialTime = currentCount.QuadPart - initialTime.QuadPart;

	double currentSeconds = static_cast< double >( elapsedCountsSinceInitialTime ) * secondsPerCount;
	return currentSeconds;
}

void SleepSeconds(float sleepTime)
{
	int msToSleep = (int)(1000.f * sleepTime);
	Sleep(msToSleep);
}
