#include "vastpch.h"
#include "LineupPanel.h"

#include "GUI/EditorLayout/Layout.h"

#include <imgui.h>

namespace Vast {
	LineupPanel::LineupPanel()
		: Panel("Lineup")
	{
		Init();
	}

	LineupPanel::LineupPanel(const Ref<Scene>& context)
		: Panel("Lineup"), m_Context(context)
	{
		Init();
	}

	void LineupPanel::Init()
	{
		m_SelectedEntity = CreateRef<Entity>();
	}

	void LineupPanel::DrawPanel()
	{
		ImGui::Begin(m_Name.c_str());

		m_Context->GetRegistry().each([&](auto entityID)
			{
				Entity entity(entityID, m_Context.get());
				DrawNode(entity);
			});

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
			*m_SelectedEntity = Entity();

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

		ImGuiTreeNodeFlags flags = ((*m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64)entity.GetHandle(), flags, label.c_str());

		if (ImGui::IsItemClicked())
			*m_SelectedEntity = entity;

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
			if (*m_SelectedEntity == entity)
				*m_SelectedEntity = Entity();
		}
		else if (duplicate)
		{
			m_Context->DuplicateEntity(entity);
		}
	}

}