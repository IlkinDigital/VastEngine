#pragma once

#include "CoreMinimal.h"

namespace Vast {

	struct TagComponent
	{
		String Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const String& tag)
			: Tag(tag) {}
	};

}