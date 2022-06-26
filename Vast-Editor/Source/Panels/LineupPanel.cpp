#include "LineupPanel.h"

#include "EditorLayout/Layout.h"

#include <imgui.h>

namespace Vast {

	LineupPanel::LineupPanel(const Ref<Scene>& context)
		: m_Context(context)
	{
	}

	void LineupPanel::DrawPanel()
	{
		ImGui::Begin("Lineup");

		m_Context->GetRegistry().each([&](auto entityID)
			{
				Entity entity(entityID, m_Context.get());
				DrawNode(entity);
			});

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
			EditorLayout::SetSelectedEntity({});

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void LineupPanel::DrawNode(Entity entity)
	{
		String& label = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((EditorLayout::GetSelectedEntity() == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64)entity.GetHandle(), flags, label.c_str());

		if (ImGui::IsItemClicked())
			EditorLayout::SetSelectedEntity(entity);

		bool deleted = false;
		bool duplicate = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
				deleted = true;
			if (ImGui::MenuItem("Duplicate"))
				duplicate = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (deleted)
		{
			m_Context->DestroyEntity(entity);
			if (EditorLayout::GetSelectedEntity() == entity)
				EditorLayout::SetSelectedEntity({});
		}
		else if (duplicate)
		{
			m_Context->DuplicateEntity(entity);
		}
	}

}