#include "vastpch.h"
#include "MathUtils.h"

#include <glm/gtc/type_ptr.hpp>

namespace Vast {

	const float* ValuePointer(const Mat4& data)
	{
		return glm::value_ptr(data);
	}

}