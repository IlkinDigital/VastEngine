#pragma once

#include "RenderComponent.h"
#include "AssetManager/AssetTypes/BoardFlipbookAsset.h"
#include "Project/Project.h"

namespace Vast {

	struct SpriteComponent : public BoardRenderComponent
	{
		Ref<BoardFlipbookAsset> Flipbook;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent& other)
			: Flipbook(RefCast<BoardFlipbookAsset>(Project::GetAssetManager()->GetAsset(other.Flipbook->GetPath())->Clone())) {}
		SpriteComponent(const Ref<BoardFlipbookAsset>& flipbook)
			: Flipbook(flipbook) {}
	};

}