#include "vastpch.h"
#include "FontManager.h"

#include <imgui.h>

namespace Vast {

	struct Font
	{
		ImFont* FontsBySize[3]{};
		Filepath SourcePath;
	};

	static std::unordered_map<FontWeight, Font> s_FontMap;

	float FontManager::ScalarSize(FontSize size)
	{
		switch (size)
		{
		case FontSize::Small:
			return 12.0f;
		case FontSize::Medium:
			return 15.0f;
		case FontSize::Large:
			return 20.0f;
		}

		return 15.0f;
	}

	void FontManager::UploadFont(const Filepath& path, FontWeight type)
	{
		OPTICK_EVENT();

		ImGuiIO& io = ImGui::GetIO();
		auto& newFont = s_FontMap[type];
		newFont.SourcePath = path;
		for (int i = 0; i < 3; i++) 
			newFont.FontsBySize[i] = io.Fonts->AddFontFromFileTTF(path.string().c_str(), ScalarSize((FontSize)i));
		SetDefault(FontSize::Medium, type);
	}

	void FontManager::SetDefault(FontSize size, FontWeight type)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = GetFont(size, type);
	}

	ImFont* FontManager::GetFont(FontSize size, FontWeight type)
	{
		Font& font = s_FontMap.find(type)->second;
		ImFont* result = font.FontsBySize[(int)size];
		return result;
	}

}