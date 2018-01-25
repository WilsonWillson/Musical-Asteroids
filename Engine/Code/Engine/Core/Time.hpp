//-----------------------------------------------------------------------------------------------
// Time.hpp
//	A simple high-precision time utility function for Windows
//	based on code by Squirrel Eiserloh
#pragma once

unsigned long long  GetCurrentPerformanceCounter();
double GetPeromanceCounterInSeconds();

char const * TimeOpCountToString(unsigned long long op_count);
unsigned long long TimeOpCountTo_us(unsigned long long op_count);
double TimeOpCountTo_ms(unsigned long long op_count);
unsigned long long TimeOpCountFrom_ms(double ms);
//-----------------------------------------------------------------------------------------------
double GetCurrentTimeSeconds();

void SleepSeconds(float sleepTime);
