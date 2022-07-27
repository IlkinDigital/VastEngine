#pragma once

#include "Panel.h"

namespace Vast {

	enum class LogCategory { Trace, Info, Warn, Error, Fatal };

	struct LogMessage
	{
		String Message;
		LogCategory Category;
	};

	class LogPanel : public Panel
	{
		friend class Logger;
	public:
		LogPanel()
			: Panel("LogPanel") {}

		virtual void DrawPanel() override;

		static const std::stringstream& GetLogStream() { return s_LogStream; }

		static void AddLog(const String& message, const LogCategory& category);
		void Clear();
	private:
		inline static std::stringstream s_LogStream;
	};

}
