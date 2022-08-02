#pragma once

#include "CoreMinimal.h"

struct ImFont;

namespace Vast {

	enum class FontWeight { Regular = 0, Light, Bold };
	enum class FontSize { Small = 0, Medium, Large };

	class FontManager
	{
	public:
		static void UploadFont(const Filepath& path, FontWeight type);
		static void SetDefault(FontSize size, FontWeight type);
		static ImFont* GetFont(FontSize size, FontWeight type);
		static float ScalarSize(FontSize size);
	};

}
