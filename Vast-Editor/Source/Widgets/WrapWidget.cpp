#include "WrapWidget.h"

#include <imgui.h>

namespace Vast {

	float WrapWidget::s_Padding = 5.0f;
	Vector2 WrapWidget::s_Size = { 128.0f, 128.0f };

	void WrapWidget::Begin(float padding, const Vector2& size)
	{
		s_Padding = padding;
		s_Size = size;
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = panelWidth / (size.x + padding);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);
	}

	void WrapWidget::End()
	{
		ImGui::Columns(1);
	}
}