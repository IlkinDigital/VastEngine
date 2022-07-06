#include "SubwindowManager.h"

namespace Vast {

	void SubwindowManager::PushSubwindow(const Ref<Subwindow>& sw)
	{
		PushSubwindow(sw, UUID());
	}

	void SubwindowManager::PushSubwindow(const Ref<Subwindow>& sw, UUID uuid)
	{
		sw->Open();
		m_SubwindowStack.emplace_back(Storage({ sw, uuid }));
	}

	void SubwindowManager::RemoveSubwindow(const Ref<Subwindow>& sw)
	{
		for (auto it = m_SubwindowStack.begin(); it != m_SubwindowStack.end();)
		{
			if ((*it).Subwindow->GetUUID() == sw->GetUUID())
			{
				(*it).Subwindow->Close();
				VAST_CORE_INFO("Removed '{0}' subwindow", (*it).Subwindow->GetName());
				it = m_SubwindowStack.erase(it);
			}
			else
				it++;
		}
	}

	bool SubwindowManager::HasStorageWithUUID(UUID uuid)
	{
		for (const auto& storage : m_SubwindowStack)
		{
			if (storage.ID == uuid)
				return true;
		}

		return false;
	}

	void SubwindowManager::OnUpdate(Timestep ts)
	{
		for (auto& sw : m_SubwindowStack)
			sw.Subwindow->OnUpdate(ts);
	}

	void SubwindowManager::OnGUIRender()
	{
		for (auto& sw : m_SubwindowStack)
		{
			if (!sw.Subwindow->IsOpen())
				RemoveSubwindow(sw.Subwindow);
			else
				sw.Subwindow->OnGUIRender();
		}
	}

}