#pragma once

#include "Vast/Core/CoreMinimal.h"

#include "spdlog/spdlog.h"

namespace Vast {

	class Logger
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}

#ifndef VAST_CONFIG_DISTRIBUTION
	#define VAST_CORE_TRACE(...)	::Vast::Logger::GetCoreLogger()->trace(__VA_ARGS__)
	#define VAST_CORE_INFO(...)		::Vast::Logger::GetCoreLogger()->info(__VA_ARGS__)
	#define VAST_CORE_WARN(...)		::Vast::Logger::GetCoreLogger()->warn(__VA_ARGS__)
	#define VAST_CORE_ERROR(...)	::Vast::Logger::GetCoreLogger()->error(__VA_ARGS__)
	#define VAST_CORE_FATAL(...)	::Vast::Logger::GetCoreLogger()->critical(__VA_ARGS__)

	#define VAST_TRACE(...)			::Vast::Logger::GetClientLogger()->trace(__VA_ARGS__)
	#define VAST_INFO(...)			::Vast::Logger::GetClientLogger()->info(__VA_ARGS__)
	#define VAST_WARN(...)			::Vast::Logger::GetClientLogger()->warn(__VA_ARGS__)
	#define VAST_ERROR(...)			::Vast::Logger::GetClientLogger()->error(__VA_ARGS__)
	#define VAST_FATAL(...)			::Vast::Logger::GetClientLogger()->critical(__VA_ARGS__)
#else
	#define VAST_CORE_TRACE(...)
	#define VAST_CORE_INFO(...)	
	#define VAST_CORE_WARN(...)	
	#define VAST_CORE_ERROR(...)
	#define VAST_CORE_FATAL(...)

	#define VAST_TRACE(...)		
	#define VAST_INFO(...)		
	#define VAST_WARN(...)		
	#define VAST_ERROR(...)		
	#define VAST_FATAL(...)		
#endif