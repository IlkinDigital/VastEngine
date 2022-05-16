#pragma once

#include "Vast.h"

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
	private:

		// TEMPORARY
		Ref<PerspectiveCamera> m_Camera;
		Vector3 m_CameraPosition = { 0.0f, 0.0f, 5.0f };
		Vector3 m_CameraRotation{};
		float m_CameraSpeed = 4.0f;
	
		float m_FPS = 0.0f;
		float m_FPSWait = 0.0f;
	};

}
