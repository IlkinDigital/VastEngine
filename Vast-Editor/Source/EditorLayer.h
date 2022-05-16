#pragma once

#include "Vast.h"

namespace Vast {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const String& name)
			: Layer(name) {}

		void OnAttach() override;

		void OnUpdate() override;

		void OnGUIRender() override;

		void OnDetach() override;

		void OnEvent(Event& event) override;
	private:
		bool OnWindowResize(WindowResizeEvent& event);
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<Shader> m_Shader;

		OrthographicCamera m_Camera = OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
		float m_CameraSpeed = 0.05f;
	};

}
