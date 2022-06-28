#include "FlipbookEditor.h"

#include "Widgets/WrapWidget.h"

#include "EditorLayout/Layout.h"

#include "Renderer/Renderer2D.h"

namespace Vast {

	FlipbookEditor::FlipbookEditor()
		: Subwindow("FlipbookEditor"), m_Viewport("FlipbookViewport"), m_Properties("FlipbookProperties")
	{
	}

	void FlipbookEditor::OnAttach()
	{
		m_Scene = CreateRef<Scene>();
		m_SceneRenderer.Init(400, 400);

		m_EditorCamera.SetPosition({ 0.0f, 2.0f, 5.0f });
		m_EditorCamera.SetPitch(-0.2975f);

		m_CurrentFrame = m_Scene->CreateEntity("Sprite");
		m_CurrentFrame.AddComponent<RenderComponent>();
		auto& ftc = m_CurrentFrame.GetComponent<TransformComponent>();
		ftc.Translation.y = 0.5f;

		Entity grid = m_Scene->CreateEntity("Grid");
		grid.AddComponent<RenderComponent>().Texture = Texture2D::Create("Resources/Textures/Grid.png");
		auto& gtc = grid.GetComponent<TransformComponent>();
		gtc.Rotation.x = Math::Radians(90.0f);
		gtc.Scale = { 5.0f, 5.0f, 5.0f };

		m_Viewport.Open();
		m_Viewport.SetColorAttachment(m_SceneRenderer.GetFramebuffer()->GetColorAttachment());
		m_Properties.Open();
		m_Properties.SetContextEntity(CreateRef<Entity>(m_CurrentFrame));

	}

	void FlipbookEditor::OnUpdate(Timestep ts)
	{
		m_Flipbook->Update(ts);
		m_CurrentFrame.GetComponent<RenderComponent>().Texture = m_Flipbook->GetCurrentTexture();

		auto spec = m_SceneRenderer.GetFramebuffer()->GetSpecification();
		if (spec.Width != m_Viewport.GetWidth() || spec.Height != m_Viewport.GetHeight())
		{
			if (m_Viewport.GetWidth() > 0 && m_Viewport.GetHeight() > 0)
			{
				m_SceneRenderer.GetFramebuffer()->Resize({ m_Viewport.GetWidth(), m_Viewport.GetHeight() });
				m_Scene->OnViewportResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
				m_EditorCamera.SetViewportSize((float)m_Viewport.GetWidth(), (float)m_Viewport.GetHeight());
			}
		}

		m_SceneRenderer.Begin();

		m_EditorCamera.OnUpdate(ts);
		m_Scene->OnUpdate(ts, m_EditorCamera);

		m_SceneRenderer.End();
	}

	void FlipbookEditor::DrawPanel()
	{
		if (!EditorLayout::BeginWindow("Flipbook Editor", &m_IsOpen))
		{
			EditorLayout::EndWindow();
			return;
		}
		
		/**
		* Frames panel
		*/
		ImGui::Begin("Frames");
		
		WrapWidget::Begin();
		for (const auto& frame : m_Flipbook->GetKeyFrames())
		{
			WrapWidget::PushItem([&frame](const Vector2& size)
				{
					ImGui::ImageButton((ImTextureID)frame.Texture->GetRendererID(),
						{ size.x, size.y }, { 0, 1 }, { 1, 0 });
				});
		}
		WrapWidget::End();

		ImGui::End();

		m_Viewport.OnGUIRender();
		m_Properties.OnGUIRender();
		
		EditorLayout::EndWindow();
	}

}