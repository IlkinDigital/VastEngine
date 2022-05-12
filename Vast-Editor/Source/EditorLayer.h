#pragma once

#include "Vast.h"

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
