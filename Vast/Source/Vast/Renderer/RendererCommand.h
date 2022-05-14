#pragma once

#include "RendererInterface.h"

namespace Vast {

	class RendererCommand
	{
	public:
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
