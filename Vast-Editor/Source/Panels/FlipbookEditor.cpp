#include "FlipbookEditor.h"

#include "Widgets/WrapWidget.h"

#include "EditorLayout/Layout.h"

namespace Vast {

	void FlipbookEditor::DrawPanel()
	{
		ImGui::Begin("Flipbook Editor");

		WrapWidget::Begin();

		for (const auto& frame : m_Context->GetKeyFrames())
		{
			WrapWidget::PushItem([&frame](const Vector2& size)
				{
					ImGui::ImageButton((ImTextureID)frame.Texture->GetRendererID(), 
						{ size.x, size.y }, { 0, 1 }, { 1, 0 });
				});
		}

		WrapWidget::End();

		ImGui::End();
	}

}