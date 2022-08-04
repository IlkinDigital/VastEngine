#pragma once

#include "GUI/Panels/Panel.h"

namespace Vast {

	class SubwindowManager
	{
	public:
		struct Storage
		{
			Ref<Subwindow> Subwindow;
			UUID ID;
		};
	public:
		void PushSubwindow(const Ref<Subwindow>& sw);
		void PushSubwindow(const Ref<Subwindow>& sw, UUID uuid);

		void RemoveSubwindow(const Ref<Subwindow>& sw);
		bool HasStorageWithUUID(UUID uuid);

		void OnUpdate(Timestep ts);
		void OnGUIRender();

		const DArray<Storage>::iterator& begin() { return m_SubwindowStack.begin(); }
		const DArray<Storage>::iterator& end() { return m_SubwindowStack.end(); }
	private:
		DArray<Storage> m_SubwindowStack;
	};

}
