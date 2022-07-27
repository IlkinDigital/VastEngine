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
				it = m_SubwindowStack.erase(it);
				break;
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
		for (auto it = m_SubwindowStack.begin(); it != m_SubwindowStack.end();)
		{
			auto& sw = *it;
			if (!sw.Subwindow->IsOpen())
			{
				sw.Subwindow->Close();
				VAST_ERROR("Removed '{0}' subwindow", (*it).Subwindow->GetName());
				it = m_SubwindowStack.erase(it);
			}
			else
			{
				sw.Subwindow->OnUpdate(ts);
				it++;
			}
		}
	}

	void SubwindowManager::OnGUIRender()
	{
		for (auto& sw : m_SubwindowStack)
		{
			sw.Subwindow->OnGUIRender();
		}
	}

}