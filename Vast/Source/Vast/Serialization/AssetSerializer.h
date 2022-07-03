#pragma once

#include "Project/Project.h"
#include "AssetManager/Asset.h"

namespace YAML {
	class Emitter;
}

namespace Vast {

	class AssetSerializer
	{
	public:
		AssetSerializer(const Ref<Project>& project, const Ref<Asset>& asset);

		void Serialize();
		bool Deserialize(const Filepath& path);

		const Ref<Asset>& GetAsset() const { return m_Asset; }
	private:
		String SerializeHead();
		void DeserializeHead(const String& source, const Filepath& assetPath);

		String SerializeTexture();
		bool DeserializeTexture(const String& source);

		String SerializeScene();
		bool DeserializeScene(const String& source);

		String SerializeBoardFlipbook();
		bool DeserializeBoardFlipbook(const String& source);
	private:
		Ref<Asset> m_Asset;
		Ref<Project> m_Project;
	};

}
