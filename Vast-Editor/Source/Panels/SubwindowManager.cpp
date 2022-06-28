#include "SubwindowManager.h"

namespace Vast {

	void SubwindowManager::PushSubwindow(const Ref<Subwindow>& sw)
	{
		sw->Open();
		m_SubwindowStack.emplace_back(sw);
	}

	void SubwindowManager::RemoveSubwindow(const Ref<Subwindow>& sw)
	{
		for (auto it = m_SubwindowStack.begin(); it != m_SubwindowStack.end();)
		{
			if ((*it)->GetUUID() == sw->GetUUID())
			{
				(*it)->Close();
				VAST_CORE_INFO("Removed '{0}' subwindow", (*it)->GetName());
				it = m_SubwindowStack.erase(it);
			}
			else
				it++;
		}
	}

	void SubwindowManager::OnUpdate(Timestep ts)
	{
		for (auto& sw : m_SubwindowStack)
			sw->OnUpdate(ts);
	}

	void SubwindowManager::OnGUIRender()
	{
		for (const auto& sw : m_SubwindowStack)
		{
			if (!sw->IsOpen())
				RemoveSubwindow(sw);
			else
				sw->OnGUIRender();
		}
	}

}