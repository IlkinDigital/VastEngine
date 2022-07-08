#pragma once

#include "RenderComponent.h"
#include "AssetManager/AssetTypes/BoardFlipbookAsset.h"
#include "AssetManager/AssetManager.h"

namespace Vast {

	struct SpriteComponent : public RenderComponent
	{
		Ref<BoardFlipbookAsset> Flipbook;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent& other)
			: Flipbook(RefCast<BoardFlipbookAsset>(AssetManager::Get()->GetAsset(other.Flipbook->GetPath())->Clone())) {}
		SpriteComponent(const Ref<BoardFlipbookAsset>& flipbook)
			: Flipbook(flipbook) {}
	};

}