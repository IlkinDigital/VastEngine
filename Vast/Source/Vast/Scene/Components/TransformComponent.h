#pragma once

#include "Math/MathOperations.h"

namespace Vast {

	struct TransformComponent
	{
		Vector3 Translation = { 0.0f, 0.0f, 0.0f };
		Vector3 Rotation = { 0.0f, 0.0f, 0.0f };
		Vector3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		Mat4 Transform() const
		{
			Mat4 rotation = Math::Rotate(Mat4(1.0f), Rotation.x, { 1.0f, 0.0f, 0.0f })
				* Math::Rotate(Mat4(1.0f), Rotation.y, { 0.0f, 1.0f, 0.0f })
				* Math::Rotate(Mat4(1.0f), Rotation.z, { 0.0f, 0.0f, 1.0f });

			return Math::Translate(Mat4(1.0f), Translation)
				* rotation
				* Math::Scale(Mat4(1.0f), Scale);
		}
	};

}