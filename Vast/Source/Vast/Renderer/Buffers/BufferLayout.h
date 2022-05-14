#pragma once

#include "Vast/Renderer/RendererCore.h"

namespace Vast {

	struct BufferElement
	{
		String Name;
		ShaderDataType Type;
		uint32 Size;
		uint32 Offset; // To be calculated in BufferLayout
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const String& name, bool normalized = false)
			: Name(name), Type(type), Size(CalculateSize(Type)), Offset(0), Normalized(normalized) {}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements), m_Stride(0)
		{
			CalculateStrideAndOffsets();
		}

		const DArray<BufferElement>& GetElements() const { return m_Elements; }
		uint32 GetStride() const { return m_Stride; }
	private:
		void CalculateStrideAndOffsets()
		{
			uint32 offset = 0;

			for (auto& e : m_Elements)
			{
				e.Offset = offset;
				m_Stride += e.Size;
				offset += e.Size;
			}
		}
	private:
		DArray<BufferElement> m_Elements;
		uint32 m_Stride; // Byte size of each vertex
	};

}