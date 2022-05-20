#include "PropertiesPanel.h"

#include "EditorCore/EditorControl.h"

namespace Vast {

	void PropertiesPanel::OnGUIRender(Entity entity)
	{
		ImGui::Begin("Properties");

		if (entity.IsValid())
			DrawComponents(entity);

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
				auto& color = component.Color;

				float buffer[4]
				{
					color.r, color.g, color.b, color.a
				};
				if (ImGui::ColorEdit4("Color", buffer))
					color = { buffer[0], buffer[1], buffer[2], buffer[3] };
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
	}

}