#pragma once

#include "Asset.h"
#include "Board2D/BoardFlipbook.h"

namespace Vast {

	class BoardFlipbookAsset : public Asset
	{
	public:
		BoardFlipbookAsset(const String& name, const Filepath& path, UUID uuid)
			: Asset(AssetType::BoardFlipbook, name, path, uuid) {}

		const Ref<Board2D::Flipbook>& GetFlipbook() const { return m_Flipbook; }
		void SetFlipbook(const Ref<Board2D::Flipbook>& flipbook) { m_Flipbook = flipbook; }

		ASSET_TYPE_IMPL(AssetType::BoardFlipbook)
	private:
		Ref<Board2D::Flipbook> m_Flipbook;
	};

}