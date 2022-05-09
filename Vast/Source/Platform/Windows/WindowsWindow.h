#pragma once

#include "Vast/ApplicationCore/Window.h"

namespace Vast {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
	private:
		void Init(const WindowProps& props);
	};

}
