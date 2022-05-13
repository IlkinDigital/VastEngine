#pragma once

// Creates a bit field
#define VAST_BIT(x) (1 << (x))

// Binds events for dispatcher
#define VAST_BIND_EVENT(fn) [this](auto&&... args)->decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }