#include "vastpch.h"
#include "AssetQueue.h"

namespace Vast {

	void AssetQueue::Enqueue(const Ref<Asset>& asset)
	{
		OPTICK_EVENT();

		for (uint32 i = 0; i < m_Buffer.size(); i++)
		{
			if (m_Buffer[i]->GetType() > asset->GetType())
			{
				m_Buffer.insert(m_Buffer.begin() + i, asset);
				return;
			}
		}

		m_Buffer.emplace_back(asset);
	}

	void AssetQueue::Dequeue()
	{
		OPTICK_EVENT();

		m_Buffer.erase(m_Buffer.begin());
	}

}