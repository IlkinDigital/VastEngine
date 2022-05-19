#pragma once

#include "Vast/ApplicationCore/Layer/Layer.h"

namespace Vast {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer()
			: Layer("ImGuiLayer") {}

		void OnAttach() override;
		void OnDetach() override;
		void OnGUIRender() override;
		void OnEvent(Event& event) override;

		void Begin();
		void End();

		bool BlockEvents = false;
	};

}
