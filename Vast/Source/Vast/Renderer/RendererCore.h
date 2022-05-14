#pragma once

#include "CoreMinimal.h"

namespace Vast {

	enum class RendererAPI { None = 0, OpenGL };

	using RendererID = uint32;
	
	enum class ShaderDataType
	{
		None = 0,
		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
		Mat3, Mat4
	};

	uint32 CalculateSize(ShaderDataType type);
	uint32 CalculateComponentCount(ShaderDataType type);
}