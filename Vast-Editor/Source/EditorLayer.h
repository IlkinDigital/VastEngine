#pragma once

#include "Vast.h"

#include <imgui.h>

namespace Vast {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const String& name)
			: Layer(name) {}

		void OnAttach() override
		{
			VAST_WARN("{0} layer has attached", m_Name);
		}

		void OnUpdate() override
		{

		}

		void OnGUIRender() override
		{
			ImGui::Begin("Test");

			ImGui::Text("Testing ImGuiLayer");

			ImGui::End();
		}

		void OnDetach() override
		{
			VAST_WARN("{0} layer has detached", m_Name);
		}

		void OnEvent(Event& event) override
		{
			VAST_TRACE("{0}", event.ToString());
		}
	};

}
