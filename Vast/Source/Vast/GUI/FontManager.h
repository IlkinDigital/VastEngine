#pragma once

#include "CoreMinimal.h"

#include <imgui.h>

namespace Vast {

	class FontManager
	{
	public:
		enum class WeightType { Regular = 0, Light, Bold };
	public:
		static void UploadFont(const String& filepath, WeightType type);
		static ImFont* GetFont(WeightType type);
		static void SetDefault(WeightType type);
	private:
		static SArray<ImFont*, 3> m_Fonts;
		static float m_FontSize;
	};

}
