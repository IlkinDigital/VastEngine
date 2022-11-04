#pragma once

#include "GUI/Panels/LogPanel.h"

#include "GUI/FontManager.h"
#include "Project/Project.h"

#include "Renderer/SceneRenderer.h"
#include "AssetManager/AssetManager.h"
#include "Scripting/ScriptEngine.h"

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

		void OpenFlipbookEditor(const Ref<BoardFlipbookAsset>& bfa);
		void OpenSpriteSheetEditor(const Ref<BoardSpriteSheetAsset>& bssa);
	private:
		void ResizeViewport();

		void NewScene();
		void OpenScene(const Filepath& filepath);
		void SaveScene(const Filepath& filepath);

		void OpenProject(const Filepath& filepath);
		void NewProject(const String& name, const Filepath& filepath);

		void BuildScripts();
		void RunPremake();

		void OnScenePlay();
		void OnSceneStop();

		bool OnFilesDrop(FilesDropEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
		bool OnKeyPressed(KeyPressedEvent& event);
	private:
		// Panels

		static MudboxLayer* s_Instance;

		friend class DebugOutput;
	};

}
