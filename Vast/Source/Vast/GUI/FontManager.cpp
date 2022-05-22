#include "vastpch.h"
#include "FontManager.h"

namespace Vast {

	float FontManager::m_FontSize = 17.0f;
	SArray<ImFont*, 3> FontManager::m_Fonts = {};

	void FontManager::UploadFont(const String& filepath, WeightType type)
	{
		ImGuiIO& io = ImGui::GetIO();
		m_Fonts[(int)type] = io.Fonts->AddFontFromFileTTF(filepath.c_str(), m_FontSize);
		io.FontDefault = m_Fonts[(int)type];
	}

	void FontManager::SetDefault(WeightType type)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = m_Fonts[(int)type];
	}

	ImFont* FontManager::GetFont(WeightType type)
	{
		return m_Fonts[(int)type];
	}

}