#include "vastpch.h"
#include "MathOperations.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace Vast::Math {

	float Radians(float degrees)
	{
		return glm::radians(degrees);
	}

	Mat4 Translate(const Mat4& transform, const Vector3& position)
	{
		return glm::translate(transform, position);
	}

	Mat4 Rotate(const Mat4& transform, float angle, const Vector3& axis)
	{
		return glm::rotate(transform, angle, axis);
	}

	Mat4 Scale(const Mat4& transform, const Vector3& scale)
	{
		return glm::scale(transform, scale);
	}

	Mat4 Inverse(const Mat4& matrix)
	{
		return glm::inverse(matrix);
	}

	Mat4 OrthographicMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		return glm::ortho(left, right, bottom, top, zNear, zFar);
	}

	Mat4 Perspective(float fov, float aspectRatio, float zNear, float zFar)
	{
		return glm::perspective(fov, aspectRatio, zNear, zFar);
	}

}