#include "vastpch.h"
#include "Logger.h"

#include "GUI/Panels/LogPanel.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/ostream_sink.h"

namespace Vast {

	Ref<spdlog::logger> Logger::s_BufferLogger;
	Ref<spdlog::logger> Logger::s_CoreLogger;
	Ref<spdlog::logger> Logger::s_ClientLogger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_BufferLogger = CreateRef<spdlog::logger>("BUFFER", std::make_shared<spdlog::sinks::ostream_sink_mt>(LogPanel::s_LogStream));
		s_BufferLogger->set_pattern("%^[%l]: %v [%T]%$");

		s_CoreLogger = spdlog::stdout_color_mt("VAST");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}