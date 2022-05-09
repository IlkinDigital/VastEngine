#pragma once

#include <memory>

namespace Vast {

	template<typename Ty>
	using Ref = std::shared_ptr<Ty>;
	
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename Ty>
	using Scope = std::unique_ptr<Ty>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}
