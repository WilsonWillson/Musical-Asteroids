#pragma once
#define TRACK_MEMORY_NONE	 (-1)
#define TRACK_MEMORY_BASIC    (0)
#define TRACK_MEMORY_VERBOSE  (1)

#define PROFILE_MEMORY  (1)

// If not defined, we will not track memory at all
// BASIC will track bytes used, and count
// VERBOSE will track individual callstacks
#if defined(_DEBUG)
#define TRACK_MEMORY          TRACK_MEMORY_VERBOSE //TRACK_MEMORY_VERBOSE
#define PROFILED_BUILD        PROFILE_MEMORY
#elif defined(_RELEASE)
#define TRACK_MEMORY         TRACK_MEMORY_BASIC
#else 
#define TRACK_MEMORY          TRACK_MEMORY_NONE
#endif
