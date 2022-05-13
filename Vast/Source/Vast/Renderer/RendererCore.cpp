#include "vastpch.h"
#include "RendererCore.h"

namespace Vast {

	uint32 CalculateSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Int:    return 1 * 4;
		case ShaderDataType::Int2:   return 2 * 4;
		case ShaderDataType::Int3:   return 3 * 4;
		case ShaderDataType::Int4:   return 4 * 4;
		case ShaderDataType::Float:  return 1 * 4;
		case ShaderDataType::Float2: return 2 * 4;
		case ShaderDataType::Float3: return 3 * 4;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3:   return 3 * 3 * 4;
		case ShaderDataType::Mat4:   return 4 * 4 * 4;
		}

		VAST_ASSERT(false, "ShaderDataType provided is not supported");
		return 0;
	}

	uint32 CalculateComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Int:    return 1;
		case ShaderDataType::Int2:   return 2;
		case ShaderDataType::Int3:   return 3;
		case ShaderDataType::Int4:   return 4;
		case ShaderDataType::Float:  return 1;
		case ShaderDataType::Float2: return 2;
		case ShaderDataType::Float3: return 3;
		case ShaderDataType::Float4: return 4;
		case ShaderDataType::Mat3:   return 3 * 3;
		case ShaderDataType::Mat4:   return 4 * 4;
		}

		VAST_ASSERT(false, "ShaderDataType provided is not supported");
		return 0;
	}

}