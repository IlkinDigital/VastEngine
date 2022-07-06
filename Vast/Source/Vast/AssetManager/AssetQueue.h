#pragma once

#include "Asset.h"

namespace Vast {

	class AssetQueue
	{
	public:
		AssetQueue() = default;

		void Enqueue(const Ref<Asset>& asset);
		void Dequeue();

		const Ref<Asset>& Front() const { return m_Buffer[0]; }
		const Ref<Asset>& Back() const { return m_Buffer[m_Buffer.size() - 1]; }

		DArray<Ref<Asset>>::iterator begin() { return m_Buffer.begin(); }
		DArray<Ref<Asset>>::iterator end() { return m_Buffer.end(); }
	public:
		DArray<Ref<Asset>> m_Buffer;
	};

}
