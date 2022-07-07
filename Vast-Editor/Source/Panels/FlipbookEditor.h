#pragma once

#include "Panel.h"
#include "ViewportPanel.h"

#include "Board2D/BoardFlipbook.h"
#include "Scene/Scene.h"
#include "Panels/PropertiesPanel.h"
#include "Renderer/SceneRenderer.h"

#include "AssetManager/BoardFlipbookAsset.h"

#include "ApplicationCore/Layer/Layer.h"

namespace Vast {

	class FlipbookSettings : public Panel
	{
	public:
		FlipbookSettings()
			: Panel("Settings") {}

		void SetFlipbook(const Ref<Board2D::Flipbook>& flipbook) { m_Flipbook = flipbook; }

		virtual void DrawPanel();
	private:
		Ref<Board2D::Flipbook> m_Flipbook;
	};

	class FramesPanel : public Panel
	{
	public:
		FramesPanel()
			: Panel("Frames") {}

		void SetFlipbook(const Ref<Board2D::Flipbook>& flipbook) { m_Flipbook = flipbook; }

		virtual void DrawPanel();
	private:
		Ref<Board2D::Flipbook> m_Flipbook;
		bool m_IsDragDropTarget = false;
	};

	class FlipbookEditor : public Subwindow
	{
	public:
		FlipbookEditor();

		virtual void OnAttach() override;
		virtual void OnUpdate(Timestep ts) override;

		void SetFlipbook(const Ref<BoardFlipbookAsset>& flipbook) 
		{ 
			m_Flipbook = flipbook; 
			m_Frames.SetFlipbook(m_Flipbook->GetFlipbook()); 
			m_Settings.SetFlipbook(m_Flipbook->GetFlipbook());
		}

		virtual void DrawPanel() override;
	private:
		ViewportPanel m_Viewport;
		PropertiesPanel m_Properties;
		FlipbookSettings m_Settings;
		FramesPanel m_Frames;

		bool m_InitializedDockspace = false;

		/**
		* For editor visualiazation purposes only,
		* doesn't contain meaningful data
		*/

		SceneRenderer m_SceneRenderer;
		EditorCamera m_EditorCamera;

		Ref<Scene> m_Scene;
		Ref<BoardFlipbookAsset> m_Flipbook;
		Entity m_CurrentFrame;
	};

	

}
