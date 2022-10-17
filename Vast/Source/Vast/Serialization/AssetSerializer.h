#pragma once

#include "Project/Project.h"
#include "AssetManager/Asset.h"

#include "Utils/FileIO/Packager/FilePackager.h"

namespace YAML {
	class Emitter;
}

namespace Vast {

	class AssetSerializer
	{
	public:
		AssetSerializer(const Ref<Asset>& asset);

		void Serialize();
		bool Deserialize(const Filepath& path);

		AssetType SerializationType(const Filepath& path);
		const Ref<Asset>& GetAsset() const { return m_Asset; }

		void ReceivePackage(const Filepath& path);
	private:
		String SerializeHead();
		void DeserializeHead(const String& source, const Filepath& assetPath);

		String SerializeTexture();
		bool DeserializeTexture(const String& source);

		String SerializeScene();
		bool DeserializeScene(const String& source);

		String SerializeBoardSpriteSheet();
		bool DeserializeBoardSpriteSheet(const String& source);

		String SerializeBoardSprite();
		bool DeserializeBoardSprite(const String& source);

		String SerializeBoardFlipbook();
		bool DeserializeBoardFlipbook(const String& source);
	private:
		Package m_Package;
		Ref<Asset> m_Asset;
	};

}
