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
		void Deserialize(const Filepath& path);
		
	private:
		void SerializeHead(YAML::Emitter& out);
		void DeserializeHead(const Filepath& path);

		void SerializeTexture();
		bool DeserializeTexture(const Filepath& path);

		void SerializeScene();
		bool DeserializeScene(const Filepath& path);

		void SerializeBoardFlipbook();
		bool DeserializeBoardFlipbook(const Filepath& path);
	private:
		Ref<Asset> m_Asset;
		Ref<Project> m_Project;
	};

}
