#pragma once

#include "CoreMinimal.h"

namespace Vast {

	class GLSLPreprocessor
	{
	public:
		struct Shaders
		{
			String VertexShaderSource;
			String FragmentShaderSource;
		};
	public:
		static Shaders Preprocess(const String& GLSLSource);
	};

}

