#include "PropertiesPanel.h"

#include "EditorCore/EditorControl.h"

#include "Scene/Components.h"
#include "Scripting/ScriptBuffer.h"

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
					ImGui::Text("Texture: %s", component.Texture->GetFilepath().filename().string().c_str());
					ImGui::ImageButton((ImTextureID)component.Texture->GetRendererID(),
						{ (width / tot) * thumbnailSize, (height / tot) * thumbnailSize }, { 0, 1 }, { 1, 0 });
				}
				else
					ImGui::Button("Texture");

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ASSET"))
					{
						String path = (const char*)payload->Data;
						component.Texture = Texture2D::Create(path);
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
					for (uint16 i = 0; i < ScriptBuffer::Get().GetBuffer().size(); i++)
					{
						bool isSelected = currScriptIndex == i;

						if (ImGui::Selectable(ScriptBuffer::Get().GetBuffer()[i].Name.c_str(), false))
						{
							currScriptIndex = i;
							component = ScriptBuffer::Get().GetBuffer()[i];
							preview = ScriptBuffer::Get().GetBuffer()[i].Name.c_str();
						}
					}
					
					ImGui::EndCombo();
				}
			});
	}

}