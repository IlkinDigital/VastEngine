#pragma once

/**
* 
* Header that includes main core functionality
* 
*/

#include "CoreTypes.h"
#include "Base.h"

#include "Assertion.h"
#include "Logging/Logger.h"

namespace Vast {

	struct AllocationTracker
	{
		uint32 TotalAllocated = 0;
		uint32 TotalFreed = 0;

		uint32 CurrentUsage() { return TotalAllocated - TotalFreed; }
	};
}

inline static Vast::AllocationTracker s_Tracker;

//inline void* operator new(size_t size) 
//{
//	s_Tracker.TotalAllocated += size;
//	return malloc(size);
//}
//
//inline void operator delete(void* memory, size_t size)
//{
//	s_Tracker.TotalFreed += size;
//	free(memory);
//}