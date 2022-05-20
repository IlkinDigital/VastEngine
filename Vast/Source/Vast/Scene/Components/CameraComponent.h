#pragma once

#include "Scene/SceneCamera.h"

namespace Vast {

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

}