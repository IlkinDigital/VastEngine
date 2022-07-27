#pragma once

#include "Vast/Core/CoreMinimal.h"

#include "spdlog/spdlog.h"

namespace Vast {

	class Logger
	{
	public:
		static void Init();
		
		static Ref<spdlog::logger>& GetBufferLogger() { return s_BufferLogger; }
		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_BufferLogger;
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
	#define VAST_CORE_TRACE(...)	::Vast::Logger::GetBufferLogger()->trace(__VA_ARGS__)
	#define VAST_CORE_INFO(...)		::Vast::Logger::GetBufferLogger()->info(__VA_ARGS__)
	#define VAST_CORE_WARN(...)		::Vast::Logger::GetBufferLogger()->warn(__VA_ARGS__)
	#define VAST_CORE_ERROR(...)	::Vast::Logger::GetBufferLogger()->error(__VA_ARGS__)
	#define VAST_CORE_FATAL(...)	::Vast::Logger::GetBufferLogger()->critical(__VA_ARGS__)

	#define VAST_TRACE(...)			VAST_CORE_TRACE(__VA_ARGS__)
	#define VAST_INFO(...)			VAST_CORE_INFO(__VA_ARGS__)	
	#define VAST_WARN(...)			VAST_CORE_WARN(__VA_ARGS__)
	#define VAST_ERROR(...)			VAST_CORE_ERROR(__VA_ARGS__)
	#define VAST_FATAL(...)			VAST_CORE_FATAL(__VA_ARGS__)
#endif