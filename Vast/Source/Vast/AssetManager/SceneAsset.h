#pragma once

#include "Asset.h"
#include "Scene/Scene.h"

namespace Vast {

	class SceneAsset : public Asset
	{
	public:
		SceneAsset(const String& name, const Filepath& path, UUID uuid)
			: Asset(AssetType::Scene, name, path, uuid) {}

		const Ref<Scene>& GetScene() const { return m_Scene; }
		void SetScene(const Ref<Scene>& scene) { m_Scene = scene; }

		ASSET_TYPE_IMPL(AssetType::Scene)
	private:
		Ref<Scene> m_Scene;
	};

}