#pragma once

#include "Math.h"

namespace Vast::Math {

	float Radians(float degrees);

	Mat4 Translate(const Mat4& transform, const Vector3& position);
	Mat4 Rotate(const Mat4& transform, float angle, const Vector3& axis);
	Mat4 Scale(const Mat4& transform, const Vector3& scale);

	Mat4 Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale);

	Mat4 Inverse(const Mat4& matrix);

	Mat4 OrthographicMatrix(float left, float right, float bottom, float top, float zNear, float zFar);
	Mat4 Perspective(float fov, float aspectRatio, float zNear, float zFar);
}