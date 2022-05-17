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
		static Shaders PreprocessSource(const String& source);
		static Shaders PreprocessFile(const String& filepath);
	};

}

