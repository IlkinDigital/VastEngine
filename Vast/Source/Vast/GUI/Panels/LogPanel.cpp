#include "vastpch.h"
#include "LogPanel.h"

namespace Vast {

	static DArray<LogMessage> s_LogBuffer;

	void LogPanel::DrawPanel()
	{
		ImGui::Begin(m_Name.c_str());

		if (ImGui::Button("Clear"))
			Clear();

		ImGui::BeginChild("Logs");

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		for (String message; std::getline(s_LogStream, message); )
		{
			LogCategory category = LogCategory::Trace;

			if (message.contains("[info]: "))
				category = LogCategory::Info;
			else if (message.contains("[warn]: "))
				category = LogCategory::Warn;
			else if (message.contains("[error]: "))
				category = LogCategory::Error;
			else if (message.contains("[fatal]: "))
				category = LogCategory::Fatal;

			s_LogBuffer.push_back({ message, category });
		}
		s_LogStream.clear();

		for (const auto& log : s_LogBuffer)
		{
			ImVec4 color;

			switch (log.Category)
			{
			case LogCategory::Trace:
				color = { 0.9f, 0.9f, 0.9f, 1.0f };
				break;
			case LogCategory::Info:
				color = { 0.2f, 0.8f, 0.3f, 1.0f };
				break;
			case LogCategory::Warn:
				color = { 0.3f, 0.6f, 0.7f, 1.0f };
				break;
			case LogCategory::Error: case LogCategory::Fatal:
				color = { 0.8f, 0.2f, 0.3f, 1.0f };
				break;
			}

			ImGui::TextColored(color, log.Message.c_str());
		}

		ImGui::PopStyleVar();

		ImGui::EndChild();

		ImGui::End();
	}

	void LogPanel::Clear()
	{
		s_LogBuffer.clear();
	}

}