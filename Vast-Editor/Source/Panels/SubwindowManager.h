#pragma once

#include "Panel.h"

namespace Vast {

	class SubwindowManager
	{
	public:
		void PushSubwindow(const Ref<Subwindow>& sw);
		void RemoveSubwindow(const Ref<Subwindow>& sw);
		void OnUpdate(Timestep ts);
		void OnGUIRender();
	private:
		DArray<Ref<Subwindow>> m_SubwindowStack;
	};

}
