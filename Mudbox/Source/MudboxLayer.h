#pragma once

#include "GUI/Panels/ViewportPanel.h"
#include "GUI/FontManager.h"

#include "Events/KeyboardEvent.h"

namespace Vast {

	class MudboxLayer : public Layer
	{
	public:
		MudboxLayer(const String& name)
			: Layer(name) 
		{
			s_Instance = this;
		}

		virtual void OnAttach() override;

		virtual void OnUpdate(Timestep ts) override;

		virtual void OnGUIRender() override;

		virtual void OnDetach() override;

		virtual void OnEvent(Event& event) override;

		static MudboxLayer* Get() { return s_Instance; }
	private:
		void ResizeViewport();

		bool OnWindowResize(WindowResizeEvent& event);
		bool OnKeyPressed(KeyPressedEvent& event);
	private:
		// Panels
		ViewportPanel m_Viewport;

		EditorCamera m_EditorCamera;

		Ref<Framebuffer> m_Framebuffer;

		static MudboxLayer* s_Instance;
	};

}
