#pragma once

#ifdef VAST_PLATFORM_WINDOWS
	#define VAST_DEBUGBREAK()
#else
	#error "Current platform is not supported yet!"
#endif

#ifdef VAST_CONFIG_DEBUG
	#define VAST_ASSERT(flag) 
	#define VAST_ASSERT(flag, ...)
#else
	#define VAST_ASSERT(...)
	#define VAST_CORE_ASSERT()
#endif
