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

			char buffer[256];
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
	}

}