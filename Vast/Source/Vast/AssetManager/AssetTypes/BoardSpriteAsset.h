#pragma once

#include "AssetManager/Asset.h"
#include "Board2D/Sprite/BoardSprite.h"

namespace Vast {

	class BoardSpriteAsset : public Asset
	{
	public:
		BoardSpriteAsset(const String& name, const Filepath& path, UUID uuid)
			: Asset(AssetType::BoardSprite, name, path, uuid) {}

		virtual Ref<Asset> Clone() const override;

		const Ref<Board2D::Sprite>& GetSprite() const { return m_Sprite; }
		void SetSprite(const Ref<Board2D::Sprite>& sprite) { m_Sprite = sprite; }

		ASSET_TYPE_IMPL(AssetType::BoardSprite)
	private:
		Ref<Board2D::Sprite> m_Sprite;
	};

}
