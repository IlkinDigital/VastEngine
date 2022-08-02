#pragma once

#include "CoreMinimal.h"

#include <imgui.h>

namespace Vast {

	enum class FontWeight { Regular = 0, Light, Bold };

	class FontManager
	{
	public:
		static void UploadFont(const String& filepath, FontWeight type);
		static ImFont* GetFont(FontWeight type);
		static void SetDefault(FontWeight type);
	private:
		inline static SArray<ImFont*, 3> m_Fonts = {};
		inline static float m_FontSize = 15.0f;
	};

}
