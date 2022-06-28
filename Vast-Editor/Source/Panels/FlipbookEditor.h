#pragma once

#include "Panel.h"
#include "ViewportPanel.h"

#include "Board2D/BoardFlipbook.h"
#include "Scene/Scene.h"
#include "Panels/PropertiesPanel.h"
#include "Renderer/SceneRenderer.h"

#include "ApplicationCore/Layer/Layer.h"

namespace Vast {

	class FlipbookEditor : public Subwindow
	{
	public:
		FlipbookEditor();

		virtual void OnAttach() override;
		virtual void OnUpdate(Timestep ts) override;

		void SetFlipbook(const Ref<Board2D::Flipbook>& flipbook) { m_Flipbook = flipbook; }

		virtual void DrawPanel() override;
	private:
		ViewportPanel m_Viewport;
		PropertiesPanel m_Properties;

		SceneRenderer m_SceneRenderer;
		EditorCamera m_EditorCamera;

		Ref<Scene> m_Scene;
		Ref<Board2D::Flipbook> m_Flipbook;
		Entity m_CurrentFrame;
	};

}
