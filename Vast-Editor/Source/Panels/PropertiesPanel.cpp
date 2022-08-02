#include "PropertiesPanel.h"

#include "EditorCore/EditorControl.h"

#include "Scene/Components.h"
#include "Scripting/ScriptEngine.h"

#include "AssetManager/AssetTypes.h"

namespace Vast {

	void PropertiesPanel::DrawPanel()
	{
		ImGui::Begin(m_Name.c_str());

		if (m_Entity->IsValid())
			DrawComponents(*m_Entity);

		ImGui::End();
	}

	void PropertiesPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[64];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				tag = String(buffer);
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				entity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Render Component"))
			{
				entity.AddComponent<RenderComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Board Render Component"))
			{
				entity.AddComponent<BoardRenderComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Component"))
			{
				entity.AddComponent<SpriteComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Native Script"))
			{
				entity.AddComponent<NativeScriptComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		EditorControl::DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component)
			{
				EditorControl::DrawVector3("Translation", component.Translation);
				Vector3 rotation = Math::Degrees(component.Rotation);
				EditorControl::DrawVector3("Rotation", rotation);
				component.Rotation = Math::Radians(rotation);
				EditorControl::DrawVector3("Scale", component.Scale, 1.0f);
			});

		EditorControl::DrawComponent<RenderComponent>("Render Component", entity, [](RenderComponent& component)
			{
				/**
				* Color Picker
				*/

				auto& color = component.Color;

				float buffer[4]
				{
					color.r, color.g, color.b, color.a
				};
				if (ImGui::ColorEdit4("Color", buffer))
					color = { buffer[0], buffer[1], buffer[2], buffer[3] };

				/**
				* Texture
				*/

				if (component.Texture)
				{
					float height = component.Texture->GetHeight();
					float width = component.Texture->GetWidth();
					float tot = height + width;
					float thumbnailSize = 256.0f;
					ImGui::Text("Texture: %s", component.Texture->GetFilepath().filename().stem().string().c_str());
					ImGui::ImageButton((ImTextureID)component.Texture->GetRendererID(),
						{ (width / tot) * thumbnailSize, (height / tot) * thumbnailSize }, { 0, 1 }, { 1, 0 });
				}
				else
					ImGui::Button("Drop Texture");

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Texture2DAsset::GetStaticTypeName()))
					{
						Asset* ta = (Asset*)payload->Data;
						component.Texture = RefCast<Texture2DAsset>(AssetManager::Get()->GetAsset(ta->GetPath()))->GetTexture();
					}
					ImGui::EndDragDropTarget();
				}
			});

		EditorControl::DrawComponent<BoardRenderComponent>("Board Render Component", entity, [](BoardRenderComponent& component)
			{
				if (component.Sprite)
				{
					float height = 1;
					float width = 0;
					float tot = height + width;
					float thumbnailSize = 256.0f;
					auto uvs = component.GetTextureCoords();
					ImVec2 uv0 = { uvs[0].x, uvs[0].y };
					ImVec2 uv1 = { uvs[1].x, uvs[1].y };
					ImGui::ImageButton((ImTextureID)component.GetTexture()->GetRendererID(),
						{ (width / tot) * thumbnailSize, (height / tot) * thumbnailSize }, uv0, uv1);
				}
				else
					ImGui::Button("Drop Board Sprite");
			});


		EditorControl::DrawComponent<SpriteComponent>("Sprite Component", entity, [](SpriteComponent& component)
			{
				if (component.Flipbook)
				{
					const auto& frame = component.Flipbook->GetFlipbook()->GetCurrentFrame();
					float height = frame->GetTexture()->GetHeight();
					float width = frame->GetTexture()->GetWidth();
					float tot = height + width;
					float thumbnailSize = 256.0f;
					auto textureCoords = frame->GetUVCoords();
					ImVec2 uv0 = { textureCoords[0].x, textureCoords[0].y };
					ImVec2 uv1 = { textureCoords[1].x, textureCoords[1].y };
					ImGui::ImageButton((ImTextureID)frame->GetTexture()->GetRendererID(),
						{ (width / tot) * thumbnailSize, (height / tot) * thumbnailSize }, uv0, uv1);
				}
				else
					ImGui::Button("Drop Flipbook");

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(BoardFlipbookAsset::GetStaticTypeName()))
					{
						Asset* ta = (Asset*)payload->Data;
						component.Flipbook = RefCast<BoardFlipbookAsset>(AssetManager::Get()->GetAsset(ta->GetPath()));
					}

					ImGui::EndDragDropTarget();
				}
			});

		EditorControl::DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& component)
			{
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeLabels[] = { "Perspective", "Orthographic" };
				uint16 currProjectionIndex = (uint16)camera.GetProjectionType();

				if (ImGui::BeginCombo("Projection", projectionTypeLabels[currProjectionIndex]))
				{
					for (uint16 i = 0; i < 2; i++)
					{
						bool isSelected = currProjectionIndex == i;

						if (ImGui::Selectable(projectionTypeLabels[i], isSelected))
						{
							currProjectionIndex = i;
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float nearClip = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip))
						camera.SetOrthographicNearClip(nearClip);

					float farClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip))
						camera.SetOrthographicFarClip(farClip);
				}
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float fov = Math::Degrees(camera.GetPerspectiveFOV());
					if (ImGui::DragFloat("FOV", &fov))
						camera.SetPerspectiveFOV(Math::Radians(fov));

					float nearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip))
						camera.SetPerspectiveNearClip(nearClip);

					float farClip = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip))
						camera.SetPerspectiveFarClip(farClip);
				}
			});

		EditorControl::DrawComponent<NativeScriptComponent>("Script", entity, [&](NativeScriptComponent& component)
			{
				uint16 currScriptIndex = 0;
				const char* preview;
				if (component.Name.empty())
					preview = "None";
				else
					preview = component.Name.c_str();

				if (ImGui::BeginCombo("Script", preview))
				{
					for (uint16 i = 0; i < ScriptEngine::Get()->GetScriptBuffer().GetBuffer().size(); i++)
					{
						bool isSelected = currScriptIndex == i;

						if (ImGui::Selectable(ScriptEngine::Get()->GetScriptBuffer().GetBuffer()[i].Name.c_str(), false))
						{
							currScriptIndex = i;
							component = ScriptEngine::Get()->GetScriptBuffer().GetBuffer()[i];
							preview = ScriptEngine::Get()->GetScriptBuffer().GetBuffer()[i].Name.c_str();
						}
					}
					
					ImGui::EndCombo();
				}
			});
	}

}