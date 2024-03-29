#pragma once

#include "RendererInterface.h"

namespace Vast {

	class RendererCommand
	{
	public:
		static void Init()
		{
			s_Interface->Init();
		}

		static void SetViewport(int32 x, int32 y, uint32 width, uint32 height)
		{
			s_Interface->SetViewport(x, y, width, height);
		}

		static void Clear()
		{
			s_Interface->Clear();
		}

		static void SetClearColor(const Vector4& color)
		{
			s_Interface->SetClearColor(color);
		}

		static void DrawIndexed(uint32 indexCount)
		{
			s_Interface->DrawIndexed(indexCount);
		}
	private:
		static RendererInterface* s_Interface;
	};

}
