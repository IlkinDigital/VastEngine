#include "MudboxLayer.h"

#include "Renderer/Renderer2D.h"

#include "Clock/FrameTime.h"

#include "Utils/System/System.h"
#include "Utils/FileIO/FileIO.h"

#include "Board2D/BoardFlipbook.h"
#include "Board2D/BoardStateMachine.h"
#include "Board2D/BoardSpriteSheet.h"

#include "Clock/Clock.h"

#include "GUI/EditorLayout/Layout.h"

#include <imgui.h>
#include <fstream>
#include <optick.h>

namespace Vast {

// Returns relative filepath through project's directory
#define PROJDIR(path) m_Project->GetProjectPath() / (path)

	MudboxLayer* MudboxLayer::s_Instance = nullptr;

	static FrameTime s_FrameTime(100);
	static Ref<Cubemap> s_Skybox;

	static Ref<Board2D::SpriteSheet> s_SpriteSheet;
	static Ref<Board2D::Sprite> s_Sprite;
	static Vector3 s_SpritePos;
	static Vector3 s_SpriteScale = { 1.0f, 1.0f, 1.0f };

	void MudboxLayer::OnAttach()
	{
		OPTICK_EVENT();

		m_Framebuffer = Framebuffer::Create({ 1920, 1080 });

		m_Viewport.SetColorAttachment(m_Framebuffer->GetColorAttachment());
		m_Viewport.Open();
		
		s_Skybox = Cubemap::Create("Resources/Cubemap/right.png", "Resources/Cubemap/left.png", "Resources/Cubemap/top.png", "Resources/Cubemap/bottom.png", "Resources/Cubemap/front.png", "Resources/Cubemap/back.png");
	
		s_SpriteSheet = CreateRef<Board2D::SpriteSheet>(Texture2D::Create("Resources/Textures/CampfireSheet.png"));
		s_SpriteSheet->SetStride({ 16.0f, 16.0f });
		s_Sprite = s_SpriteSheet->ExtractSprite(1, 0);

		RendererCommand::SetClearColor({ 0.15, 0.60, 0.9, 1.0 });
	}


	void MudboxLayer::OnUpdate(Timestep ts)
	{
		OPTICK_EVENT();

		s_FrameTime.Update(ts);

		auto spec = m_Framebuffer->GetSpecification();
		if (spec.Width != m_Viewport.GetWidth() || spec.Height != m_Viewport.GetHeight())
		{
			ResizeViewport();
		}

		m_Framebuffer->Bind();
		RendererCommand::Clear();

		Renderer2D::BeginScene(m_EditorCamera);

		Renderer2D::DrawSkybox(s_Skybox);

		Renderer2D::EndScene();

		m_EditorCamera.OnUpdate(ts);

		m_Framebuffer->Unbind();
	}
	void MudboxLayer::OnGUIRender()
	{
		OPTICK_EVENT();

		EditorLayout::BeginDockspace("Editor Dockspace");

		// Editor Camera
		ImGui::Begin("Editor Camera");
		{ 
			float gazeSpeed = m_EditorCamera.GetGazeSpeed();
			if (ImGui::DragFloat("Camera Gazing Speed", &gazeSpeed, 0.25f, 1.0f, 10.0f))
				m_EditorCamera.SetGazeSpeed(gazeSpeed);

			ImGui::DragFloat3("Sprite Translation", Math::MValuePointer(s_SpritePos), 0.1f);
			ImGui::DragFloat3("Sprite Scale", Math::MValuePointer(s_SpriteScale), 0.1f);

			ImGui::End();
		}

		ImGui::Begin("Settings");
		
		ImGui::Text("%d FPS", s_FrameTime.GetFPS());
		ImGui::Text("Frame Time: %.4f ms", s_FrameTime.GetFrameTime());

		ImGui::Text("Total Allocated: %d", s_Tracker.TotalAllocated);
		ImGui::Text("Total Freed: %d", s_Tracker.TotalFreed);
		ImGui::Text("Total In Use: %d", s_Tracker.CurrentUsage());
		
		ImGui::End();

		m_Viewport.OnGUIRender();

		//ImGui::ShowDemoWindow((bool*)1);

		EditorLayout::EndDockspace();
	}

	void MudboxLayer::OnDetach()
	{

	}

	void MudboxLayer::OnEvent(Event& event)
	{
		OPTICK_EVENT();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(VAST_BIND_EVENT(OnKeyPressed));
	}

	void MudboxLayer::ResizeViewport()
	{
		OPTICK_EVENT();

		if (m_Viewport.GetWidth() > 0 && m_Viewport.GetHeight() > 0)
		{
			m_Framebuffer->Resize({m_Viewport.GetWidth(), m_Viewport.GetHeight()});
			m_EditorCamera.SetViewportSize((float)m_Viewport.GetWidth(), (float)m_Viewport.GetHeight());
			RendererCommand::SetViewport(0, 0, m_Viewport.GetWidth(), m_Viewport.GetHeight());
		}
	}

	bool MudboxLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		return false;
	}
}