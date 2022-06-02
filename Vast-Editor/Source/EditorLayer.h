#pragma once

#include "Vast.h"

#include "Panels/ViewportPanel.h"
#include "Panels/LineupPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/ContentBrowserPanel.h"

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
		void ResizeViewport();

		void UpdateScriptModule();

		void NewScene();
		void OpenScene(const String& filepath);
		void SaveScene(const String& filepath);

		void OnScenePlay();
		void OnSceneStop();

		bool OnWindowResize(WindowResizeEvent& event);
		bool OnKeyPressed(KeyPressedEvent& event);
	private:
		// Panels
		ViewportPanel m_Viewport;
		LineupPanel m_Lineup;
		PropertiesPanel m_Properties;
		ContentBrowserPanel m_ContentBrowser;

		Gizmo3D m_Gizmo;

		// Scene
		Ref<Scene> m_ActiveScene; // Pointer to current scene - editor/runtime
		Ref<Scene> m_EditorScene, m_RuntimeScene;
		EditorCamera m_EditorCamera;
		String m_SceneFilepath;

		Ref<RuntimeModule> m_ScriptModule;

		ImVec4* m_Colors = ImGui::GetStyle().Colors;

		Ref<Framebuffer> m_Framebuffer;
		Ref<Texture2D> m_PlayIcon;
		Ref<Texture2D> m_StopIcon;
	
		float m_FPS = 0.0f;
		float m_FPSWait = 0.0f;

		enum class SceneState
		{
			Edit = 0, Play
		};

		SceneState m_SceneState = SceneState::Edit;
	};

}
