#pragma once

#ifdef VAST_PLATFORM_WINDOWS
	#define VAST_DEBUGBREAK() __debugbreak()
#else
	#error "Current platform is not supported yet!"
#endif

#ifdef VAST_CONFIG_DEBUG
	#define VAST_ASSERT(flag, msg)      if (!flag) { VAST_FATAL(msg); VAST_DEBUGBREAK(); }
	#define VAST_CORE_ASSERT(flag, msg) if (!flag) { VAST_CORE_FATAL(msg); VAST_DEBUGBREAK(); }
#else
	#define VAST_ASSERT(flag, msg)
	#define VAST_CORE_ASSERT(flag, msg)
#endif
