#include "FlipbookEditor.h"

#include "Renderer/Renderer2D.h"
#include "GUI/FontManager.h"

#include "Widgets/WrapWidget.h"
#include "EditorLayout/Layout.h"

#include "AssetManager/Texture2DAsset.h"
#include "Serialization/AssetSerializer.h"

#include <imgui_internal.h>

namespace Vast {

	FlipbookEditor::FlipbookEditor()
		: Subwindow("Flipbook Editor"), m_Viewport("Viewport"), m_Properties("Properties")
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
		m_Settings.Open();
		m_Frames.Open();
		m_Frames.SetFlipbook(m_Flipbook->GetFlipbook());
	}

	void FlipbookEditor::OnUpdate(Timestep ts)
	{
		if (m_Flipbook->GetFlipbook()->IsValid())
		{
			m_Flipbook->GetFlipbook()->Update(ts);
			m_CurrentFrame.AddOrReplaceComponent<RenderComponent>().Texture = m_Flipbook->GetFlipbook()->GetCurrentTexture();
		}
		else
		{
			m_CurrentFrame.RemoveComponent<RenderComponent>();
		}

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
		if (!EditorLayout::BeginWindow(*this, &m_IsOpen))
		{
			EditorLayout::EndWindow();
			return;
		}

		// DockBuilder 
		if (!m_InitializedDockspace)
		{
			ImGui::SetWindowSize({ m_DefaultSize.x, m_DefaultSize.y });

			ImGuiID dockspaceID = GetUUID();

			ImGui::DockBuilderRemoveNode(dockspaceID);
			ImGui::DockBuilderAddNode(dockspaceID, 
				ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_PassthruCentralNode);
			ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

			ImGuiID bottomID = -1;
			auto topID = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Up, 0.45f, nullptr, &bottomID);

			ImGuiID viewID = -1;
			auto propertiesID = ImGui::DockBuilderSplitNode(topID, ImGuiDir_Right, 0.31f, nullptr, &viewID);

			ImGuiID framesID = -1;
			auto settingsID = ImGui::DockBuilderSplitNode(bottomID, ImGuiDir_Left, 0.1f, nullptr, &framesID);

			ImGui::DockBuilderDockWindow(m_Properties.GetName().c_str(), propertiesID);
			ImGui::DockBuilderDockWindow(m_Viewport.GetName().c_str(), viewID);
			ImGui::DockBuilderDockWindow(m_Frames.GetName().c_str(), framesID);
			ImGui::DockBuilderDockWindow(m_Settings.GetName().c_str(), settingsID);

			ImGui::DockBuilderFinish(dockspaceID);
			m_InitializedDockspace = true;
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "Ctrl + S"))
				{
					AssetSerializer as(AssetManager::Get()->GetProject(), m_Flipbook);
					as.Serialize();
					AssetManager::Get()->Init();
				}

				ImGui::EndMenu();
			}

			ImGui::Text("%s", m_Flipbook->GetName().c_str());

			ImGui::EndMenuBar();
		}

		m_Viewport.OnGUIRender();
		m_Properties.OnGUIRender();
		m_Settings.OnGUIRender();
		m_Frames.OnGUIRender();

		EditorLayout::EndWindow();
	}

	void FramesPanel::DrawPanel()
	{
		ImGui::Begin(m_Name.c_str());

		//WrapWidget::Begin();

		//uint32 i = 1;
		//ImGui::PushFont(FontManager::GetFont(FontManager::WeightType::Bold));
		//for (const auto& frame : m_Flipbook->GetKeyFrames())
		//{
		//	float accumWidth = 0.0f;
		//	WrapWidget::PushItem([&](const Vector2& size)
		//		{
		//			ImGui::SetCursorPosX(size.x / 2.0f);
		//			ImGui::Text("%d", i);
		//			ImGui::Image((ImTextureID)frame.Texture->GetRendererID(),
		//				{ size.x, size.y }, { 0, 1 }, { 1, 0 });
		//		});
		//	i++;
		//}
		//ImGui::PopFont();

		//WrapWidget::End();

		Vector2 size = { 64.0f, 64.0f };

		uint32 i = 1;
		auto style = ImGui::GetStyle();
		float accumWidth = style.WindowPadding.x;

		ImGui::PushFont(FontManager::GetFont(FontManager::WeightType::Bold));
		for (const auto& frame : m_Flipbook->GetKeyFrames())
		{
			ImGui::BeginGroup();

			ImVec2 textSize = ImGui::CalcTextSize(std::to_string(i).c_str());
			ImGui::SetCursorPosX(accumWidth + size.x * 0.5f - (textSize.x * 0.5f));
			accumWidth += size.x + style.ItemSpacing.x * 2.0f;
			ImGui::Text("%d", i);

			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.7f, 0.7f, 0.7f, 0.4f });
			ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f, 1.0f, 1.0f, 0.0f });
			ImGui::ImageButton((ImTextureID)frame.Texture->GetRendererID(),
				{ size.x, size.y }, { 0, 1 }, { 1, 0 });
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_Flipbook->RemoveKeyFrame(--i);
			}
			ImGui::PopStyleColor(2);

			ImGui::EndGroup();

			ImGui::SameLine();
			i++;
		}
		ImGui::PopFont();

		ImGui::BeginGroup();
		ImGui::Text("");
		ImGui::InvisibleButton("DRAG_AND_DROP_BUTTON", { size.x, size.y });
		ImGui::EndGroup();

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Texture2DAsset::GetStaticTypeName()))
			{
				Texture2DAsset* asset = (Texture2DAsset*)payload->Data;
				if (asset && asset->GetTexture())
				{
					Ref<Texture2DAsset> ta = RefCast<Texture2DAsset>(AssetManager::Get()->GetAsset(asset->GetPath()));
					m_Flipbook->PushKeyFrame({ ta->GetTexture()});
				}
				else
				{
					VAST_ERROR("Invalid Texture2DAsset payload");
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();

	}

	void FlipbookSettings::DrawPanel()
	{
		ImGui::Begin(m_Name.c_str());

		ImGui::Text("Animation FPS");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
		float fps = m_Flipbook->GetFPS();
		if (ImGui::DragFloat("##", &fps, 0.5f, 1.0f, 256.0f, "%.1f"))
			m_Flipbook->SetFPS(fps);
		ImGui::PopItemWidth();

		ImGui::End();
	}

}