#pragma once

#include "AssetManager/Asset.h"
#include "Renderer/Texture2D.h"

namespace Vast {

	class Texture2DAsset : public Asset
	{
	public:
		Texture2DAsset(const String& name, const Filepath& path, UUID uuid);

		// It's not cloning yet
		virtual Ref<Asset> Clone() const override;

		const Ref<Texture2D>& GetTexture() const { return m_Texture; }
		void SetTexture(const Ref<Texture2D>& texture) { m_Texture = texture; }

		const DArray<char>& GetFileData() const { return m_FileData; }
		void SetFileData(const DArray<char>& fileData) { m_FileData = fileData; }

		ASSET_TYPE_IMPL(AssetType::Texture2D)
	private:
		Ref<Texture2D> m_Texture;
		DArray<char> m_FileData; // Holds data of an imported file
	};

}