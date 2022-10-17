#pragma once

// Creates a bit field
#define VAST_BIT(x) (1 << (x))

// Binds events for dispatcher
#define VAST_BIND_EVENT(fn) [this](auto&&... args)->decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifndef VAST_PROF_ENABLED
	#define OPTICK_APP(NAME)
	#define OPTICK_SHUTDOWN()
	#define OPTICK_EVENT()
	#define OPTICK_EVENT(...)
	#define OPTICK_FRAME(NAME, ...)
#else
#endif