#pragma once

#include "Math/MathCore.h"
#include "Scene/Entity.h"

#include <imgui_internal.h>

namespace Vast {

	class EditorControl
	{
	public:
		static void DrawVector3(const String& label, Vector3& values, float defaultValue = 0.0f, float columnWidth = 100.0f);

		template<typename Ty, typename Fn>
		static void DrawComponent(const String& name, Entity entity, Fn function);
	};

	/**
	* Template functions definition
	*/

	template<typename Ty, typename Fn>
	inline void EditorControl::DrawComponent(const String& name, Entity entity, Fn function)
	{
		if (entity.HasComponent<Ty>())
		{
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
				| ImGuiTreeNodeFlags_AllowItemOverlap
				| ImGuiTreeNodeFlags_Framed
				| ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_FramePadding;

			auto& component = entity.GetComponent<Ty>();
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(Ty).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);

			if (ImGui::Button(":", ImVec2{ lineHeight, lineHeight }))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				function(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<Ty>();
		}
	}

}

