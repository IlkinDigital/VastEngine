#pragma once

#include "Vast.h"

#include "Panels/ViewportPanel.h"
#include "Panels/LineupPanel.h"
#include "Panels/PropertiesPanel.h"

namespace Vast {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const String& name)
			: Layer(name) {}

		void OnAttach() override;

		void OnUpdate(Timestep ts) override;

		void OnGUIRender() override;

		void OnDetach() override;

		void OnEvent(Event& event) override;
	private:
		bool OnWindowResize(WindowResizeEvent& event);
		bool OnKeyPressed(KeyPressedEvent& event);
	private:
		// GUI
		ViewportPanel m_Viewport;
		LineupPanel m_Lineup;
		PropertiesPanel m_Properties;

		// Scene
		Ref<Scene> m_ActiveScene;

		Ref<Framebuffer> m_Framebuffer;
		Ref<Texture2D> m_PatrickTexture;
	
		float m_FPS = 0.0f;
		float m_FPSWait = 0.0f;
	};

}
