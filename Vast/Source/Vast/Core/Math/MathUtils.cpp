#include "vastpch.h"
#include "MathUtils.h"

#include <glm/gtc/type_ptr.hpp>

namespace Vast::Math {

	const float* ValuePointer(const Mat4& data)
	{
		return glm::value_ptr(data);
	}

	float* MValuePointer(Mat4& data)
	{
		return glm::value_ptr(data);
	}

	const float* ValuePointer(const Vector3& data)
	{
		return glm::value_ptr(data);
	}

	float* MValuePointer(Vector3& data)
	{
		return glm::value_ptr(data);
	}

	const float* ValuePointer(const Vector4& data)
	{
		return glm::value_ptr(data);
	}

	float* MValuePointer(Vector4& data)
	{
		return glm::value_ptr(data);
	}

}