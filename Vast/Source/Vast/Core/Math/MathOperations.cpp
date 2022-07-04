#include "vastpch.h"
#include "MathOperations.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Vast::Math {

	float Radians(float degrees)
	{
		return glm::radians(degrees);
	}

	Vector3 Radians(const Vector3& degrees)
	{
		return glm::radians(degrees);
	}

	float Degrees(float radians)
	{
		return glm::degrees(radians);
	}

	Vector3 Degrees(const Vector3& radians)
	{
		return glm::degrees(radians);
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

	Mat4 Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale)
	{
		return Translate(Mat4(1.0f), position)
			* Rotate(Mat4(1.0f), rotation.x, { 1.0f, 0.0f, 0.0f })
			* Rotate(Mat4(1.0f), rotation.y, { 0.0f, 1.0f, 0.0f })
			* Rotate(Mat4(1.0f), rotation.z, { 0.0f, 0.0f, 1.0f })
			* Scale(Mat4(1.0f), scale);
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

	bool DecomposeTransform(const Mat4& transform, Vector3& translation, Vector3& rotation, Vector3& scale)
	{
		// From glm::decompose in matrix_decompose.inl

		using namespace glm;
		using T = float;

		mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		translation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3], Pdum3;

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		scale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
	}
#endif

		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0.0f) {
			rotation.x = atan2(Row[1][2], Row[2][2]);
			rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}


		return true;
	}

}