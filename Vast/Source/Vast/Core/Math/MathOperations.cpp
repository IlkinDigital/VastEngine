#include "vastpch.h"
#include "MathOperations.h"

namespace Vast::Math {

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

	Mat4 OrthographicMatrix(float left, float right, float bottom, float top, float oNear, float oFar)
	{
		return glm::ortho(left, right, bottom, top, oNear, oFar);
	}

}