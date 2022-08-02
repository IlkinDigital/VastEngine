#include "vastpch.h"
#include "FontManager.h"

namespace Vast {

	void FontManager::UploadFont(const String& filepath, FontWeight type)
	{
		ImGuiIO& io = ImGui::GetIO();
		m_Fonts[(int)type] = io.Fonts->AddFontFromFileTTF(filepath.c_str(), m_FontSize);
		io.FontDefault = m_Fonts[(int)type];
	}

	void FontManager::SetDefault(FontWeight type)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = m_Fonts[(int)type];
	}

	ImFont* FontManager::GetFont(FontWeight type)
	{
		return m_Fonts[(int)type];
	}

}