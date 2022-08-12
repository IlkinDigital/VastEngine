#pragma once

#include "AssetManager/Asset.h"
#include "Board2D/BoardSpriteSheet.h"

namespace Vast {

	class BoardSpriteSheetAsset : public Asset
	{
	public:
		BoardSpriteSheetAsset(const String& name, const Filepath& path, UUID uuid)
			: Asset(AssetType::BoardSpriteSheet, name, path, uuid) {}

		virtual Ref<Asset> Clone() const override;

		void SetSpriteSheet(const Ref<Board2D::SpriteSheet>& spriteSheet) { m_SpriteSheet = spriteSheet; }
		const Ref<Board2D::SpriteSheet>& GetSpriteSheet() const { return m_SpriteSheet; }

		ASSET_TYPE_IMPL(AssetType::BoardSpriteSheet)
	private:
		Ref<Board2D::SpriteSheet> m_SpriteSheet;
	};

}
